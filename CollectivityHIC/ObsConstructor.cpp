// RUN COMMAND: root -l -b -q 'ObsConstructor.cpp'
// 50-60% CENTRALITY: HFSUMET = [210, 375] GeV
// 60-70% CENTRALITY: HFSUMET = [100, 210] GeV

#include <cmath>
#include "TMath.h"
using namespace std;

// Calculates the mean of a given vector
float mean(vector<float> x){
    int N = 0;
    float acc = 0.0;
    for (int i=0; i<x.size(); i++){
        acc += x[i];
        N += 1;
    }
    float m = acc/N;
    return m;
}

vector<float> double_vector_mean(vector<vector<float>> x, const unsigned int nBins){
    int nEvents_A = x.size();
    vector<float> vec;
    for (int i=0; i<nBins; i++){ // pT bins
        float acc_f_pt = 0;
        for (int j=0; j<nEvents_A; j++){ // Events
            acc_f_pt += x[j][i];
        }
        vec.push_back(acc_f_pt/nEvents_A);
    }
    return vec;
}

struct Gathered_Data{
    vector<vector<float>> vec_f_pt;
    vector<float> vec_pt_A;
    vector<float> vec_pt_B;
    vector<float> vec_pt_AB;
    vector<float> vec_unc_pt_A;
    vector<float> vec_unc_pt_B;
    vector<float> vec_unc_pt_AB;
    float mean_pt;
};

// pT and eta are the transverse momentum and pseudorapidity of the track (considering a 2D histogram where X is eta axis and Y pT axis)
// it returns a efficiency correction to be applied in histograms as -> Fill(variable,this_weight)
float getTrkCorrWeight(TFile *trkeff_file, double pT, double eta){
  if( fabs(eta) > 2.4 ){return 0.0;}
  if( pT < 0 || pT > 500 ){return 0.0;}
  float factor = 1.0;
  TH2 *eff_factor = nullptr;
  trkeff_file->GetObject("rTotalEff3D", eff_factor); // eff / (1 - fake rate)
  float eff = eff_factor->GetBinContent( eff_factor->GetXaxis()->FindBin(eta),eff_factor->GetYaxis()->FindBin(pT) );
  factor = (1.0 / eff);
  delete eff_factor;                // Free memory (sets ptr to "dangling" state)
  eff_factor = nullptr;             // Best practice: set to nullptr after deletion
  return factor;
}

float StdPoissonBootstrap(vector<float> x, int B){
    TRandom3 rndgen;
    int N = x.size();
    float lambda = 1.0;

    vector<float> bootstrapped(B, 0.0); 
    for (int i=0; i<B; i++){
        vector<int> resampled(N, 0);
        for (int j=0; j<N; j++){
            resampled[j] = x[j]*rndgen.Poisson(lambda);
        }
        bootstrapped[i] = TMath::StdDev(N, resampled.data());
    }
    float bootstrapped_mean = TMath::Mean(B, bootstrapped.data());
    return bootstrapped_mean;
}

float UncMean(vector<float> x){
    int N = x.size();
    float acc = 0.0;
    for (int i=0; i<N; i++){
        acc += pow(2, x[i]);
    }
    float uncmean = (1/N)*sqrt(acc);
    return uncmean;
}

float UncPropIndependentAminusB(float sigmaEA, float sigmaEB){
    float s = sqrt(pow(2, sigmaEA) + pow(2, sigmaEB));
    return s;
}

float UncPropIndependentAtimesB(float dEA, float sigmaEA, float dEB, float sigmaEB){
    float s = sqrt(pow(2, dEA*sigmaEA) + pow(2, dEB*sigmaEB));
    return s;
}

Gathered_Data DataGathering(float eta_gap, float HFSET_min, float HFSET_max, float ptr_min, float ptr_max, TString savename, vector<float> Xaxis_del, vector<float> pT_axis, string Correction){
    // Open CMS OpenData 2.76 TeV 50-70% centrality ROOT file
    TFile *file(0);
    TFile *cFile(0);
    TString filename = "/home/allanfgodoi/Desktop/IC-HIN-UFRGS/CollectivityHIC/Data/HiForestAOD_UPC.root";
    TString cFilename = "/home/allanfgodoi/Desktop/IC-HIN-UFRGS/CollectivityHIC/Data/TrackCorrections_HIJING_538_OFFICIAL_Mar24.root";

    file = TFile::Open(filename);
    cFile = TFile::Open(cFilename);

    const unsigned int nDummy = 30000; // Used to create a dummy len array to hold the tracks
    int B = 100;

    // Declare arrays and scalars to hold the TTree files
    float_t HFsumET = 0.0;
    float_t pvZ = 0.0;
    Int_t Ntrk = 0;
    float_t trkPt[nDummy];
    float_t trkEta[nDummy];
    float_t trkPhi[nDummy];
    float_t trkPtRes[nDummy];
    float_t trkDzSig[nDummy];
    float_t trkDxySig[nDummy];
    float_t trkNpixLayers[nDummy];

    // Addresses TTree branches to declared variables
    TTree* fTree = (TTree*)file->Get("demo/HBT");
    fTree->SetBranchAddress("HFsumET", &HFsumET);
    fTree->SetBranchAddress("pvZ", &pvZ);
    fTree->SetBranchAddress("Ntrk", &Ntrk);
    fTree->SetBranchAddress("trkPt", &trkPt);
    fTree->SetBranchAddress("trkEta", &trkEta);
    fTree->SetBranchAddress("trkPhi", &trkPhi);
    fTree->SetBranchAddress("trkPtRes", &trkPtRes);
    fTree->SetBranchAddress("trkDzSig", &trkDzSig);
    fTree->SetBranchAddress("trkDxySig", &trkDxySig);
    fTree->SetBranchAddress("trkNpixLayers", &trkNpixLayers);

    // Defining auxiliar constants
    int nBins = pT_axis.size();
    const unsigned int nEvents = fTree->GetEntries();
    vector<vector<float>> Vec_f_pt; // This vector will hold the fractions of pT of all events
    vector<float> Vec_pt_A;
    vector<float> Vec_pt_B;
    vector<float> Vec_pt_AB;
    vector<float> Vec_unc_pt_A(nEvents, 0.0);
    vector<float> Vec_unc_pt_B(nEvents, 0.0);
    vector<float> Vec_unc_pt_AB(nEvents, 0.0);

    TH1F *hist_all_pt_A = new TH1F("all_pt_A", "All pT from subset A", nBins, Xaxis_del.data()); // Create histogram to calculate <pT_A> (all events)
    TH1F *hist_pt_A = new TH1F("pt_A", "pT from subset A", nBins, Xaxis_del.data()); // Create histogram to scale pT bins and get f(pT)
    for (Long64_t ievt=0; ievt<nEvents; ievt++){ // Loop over the events
        vector<float> f_pt(nBins, 0.0); // Define vector to hold the fractions of pT in the event
        float h_pt_A = 0.0;
        float h_pt_B = 0.0;
        float nTrk_A = 0.0;
        float nTrk_B = 0.0;

        fTree->GetEntry(ievt); // Here I get the events in the opened file
        if (ievt%10000 == 0)
            cout << "Processing event: " << ievt << endl;

        // Filters made in the events
        if (HFsumET < HFSET_min || HFsumET > HFSET_max) // Applying 100 <= HFsumET <= 375 filter (related to centrality)
            continue;
        if (pvZ < -15.0 || pvZ > 15.0) // Applying |pvZ| < 15 cm filter
            continue;

        // Holder vectors from each event to calculate uncertainties
        vector<float> h_unc_vec_pt_A(Ntrk, 0.0);
        vector<float> h_unc_vec_pt_B(Ntrk, 0.0);
        vector<float> h_unc_vec_pt_AB(Ntrk, 0.0);

        // Track loop
        for(int iTrk=0; iTrk<Ntrk; iTrk++){ // Loop over the tracks in a event
            // Getting subset A: [pT]_A
            if (trkEta[iTrk] >= -2.4 && trkEta[iTrk] <= -eta_gap/2 && 
                trkPt[iTrk] >= 0.5 && trkPt[iTrk] <= 10.0 && 
                trkDzSig[iTrk] > -3.0 && trkDzSig[iTrk] < 3.0 && 
                trkDxySig[iTrk] > -3.0 && trkDxySig[iTrk] < 3.0 && 
                trkPtRes[iTrk] < 0.1){
                    float corrFac = 1.0;
                    if (Correction == "Correc") corrFac = getTrkCorrWeight(cFile, trkPt[iTrk], trkEta[iTrk]);
                    if (Correction == "noCorrec") corrFac = 1.0;
                    nTrk_A += corrFac; // Counts the number of pT entries from subset A
                    h_pt_A += corrFac*trkPt[iTrk]; // Sum all pT from subset A in the desired range
                    // These two h_unc_vecs are used for uncertainties
                    h_unc_vec_pt_A[iTrk] = corrFac*trkPt[iTrk];
                    h_unc_vec_pt_AB[iTrk] = corrFac*trkPt[iTrk]; // Define iTrk element as trkPt[iTrk]_A
                    hist_pt_A->Fill(trkPt[iTrk]); // Fills the auxiliar histogram to scale f(pT) later
                    hist_all_pt_A->Fill(trkPt[iTrk]);
            }
        
            // Getting subset B: [pT]_B
            if (trkEta[iTrk] >= eta_gap/2 && trkEta[iTrk] <= 2.4 && 
                trkPt[iTrk] >= ptr_min && trkPt[iTrk] <= ptr_max && 
                trkDzSig[iTrk] > -3.0 && trkDzSig[iTrk] < 3.0 && 
                trkDxySig[iTrk] > -3.0 && trkDxySig[iTrk] < 3.0 && 
                trkPtRes[iTrk] < 0.1){
                    float corrFac = 1.0;
                    if (Correction == "Correc") corrFac = getTrkCorrWeight(cFile, trkPt[iTrk], trkEta[iTrk]);
                    if (Correction == "noCorrec") corrFac = 1.0;
                    nTrk_B += corrFac; // Counts the number of pT entries from subset B
                    h_pt_B += corrFac*trkPt[iTrk]; // Sum all pT from subset B
                    // These two h_unc_vecs are used for uncertainties
                    h_unc_vec_pt_B[iTrk] = corrFac*trkPt[iTrk];
                    h_unc_vec_pt_AB[iTrk] *= corrFac*trkPt[iTrk]; // Multiplies already defined iTrk element trkPt[iTrk]_A by trkPt[íTrk]_B
            }
        }

        if (nTrk_A == 0.0) nTrk_A = 1.0;
        if (nTrk_B == 0.0) nTrk_B = 1.0;
        float pt_A = h_pt_A/nTrk_A; // Calculates the mean pT from subset A of each event
        float pt_B = h_pt_B/nTrk_B; // Calculates the mean pT from subset B of each event
        float pt_AB = pt_A*pt_B; // [pT]_A * [pT]_B

        // Scaling the created hist and taking the bins content to the array
        if (hist_pt_A->Integral() != 0) hist_pt_A->Scale(1/hist_pt_A->Integral()); // Defining a normalized histogram
        for (int i=0; i<nBins; i++){
            f_pt[i] = hist_pt_A->GetBinContent(i+1);
        }
        hist_pt_A->Reset();
        
        // Setting the desired vectors
        Vec_f_pt.push_back(f_pt);
        Vec_pt_A.push_back(pt_A);
        Vec_pt_B.push_back(pt_B);
        Vec_pt_AB.push_back(pt_AB);
        // Saving uncertainties from each event
        Vec_unc_f_pt[ievt] = StdPoissonBootstrap(f_pt, B);
        Vec_unc_pt_A[ievt] = StdPoissonBootstrap(h_unc_vec_pt_A, B);
        Vec_unc_pt_B[ievt] = StdPoissonBootstrap(h_unc_vec_pt_B, B);
        Vec_unc_pt_AB[ievt] = StdPoissonBootstrap(h_unc_vec_pt_AB, B);
    }

    float Mean_pt = hist_all_pt_A->GetMean();

    delete hist_all_pt_A;
    delete hist_pt_A;

    Gathered_Data struct_data;
    struct_data.vec_f_pt = Vec_f_pt;
    struct_data.vec_pt_A = Vec_pt_A;
    struct_data.vec_pt_B = Vec_pt_B;
    struct_data.vec_pt_AB = Vec_pt_AB;
    struct_data.vec_unc_pt_A = Vec_unc_pt_A;
    struct_data.vec_unc_pt_B = Vec_unc_pt_B;
    struct_data.vec_unc_pt_AB = Vec_unc_pt_AB;
    struct_data.vec_unc_f_pt = Vec_unc_f_pt;
    struct_data.mean_pt = Mean_pt;

    return struct_data;
}

// Thats the function we call to construct the observable
void ObsConstructor(float Eta_gap, float HFSET_Min, float HFSET_Max, float pTr_Min, float pTr_Max, TString Name, TString Savename, string PlotType, string Correction){
    // Defining bins and plot's x axes
    vector<float> Xaxis_del = {0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.2, 1.4, 1.6, 1.8, 1.98, 2.2, 2.38, 2.98, 3.18, 6.0, 8.04, 10.0}; // Those are the END of each bin, not the middle
    int nBins = (Xaxis_del.size()-1);
    vector<float> pT_axis;
    for (int i=0; i<nBins; i++){
        pT_axis.push_back((Xaxis_del[i+1]+Xaxis_del[i])/2); // Here we define the x axis points as the middle of each bin
    }

    // Calls DataGathering function to get selected data and get them from it using struct
    Gathered_Data gData = DataGathering(Eta_gap, HFSET_Min, HFSET_Max, pTr_Min, pTr_Max, Savename, Xaxis_del, pT_axis, Correction);
    vector<vector<float>> vec_f_pt = gData.vec_f_pt;
    vector<float> vec_pt_A = gData.vec_pt_A;
    vector<float> vec_pt_B = gData.vec_pt_B;
    vector<float> vec_pt_AB = gData.vec_pt_AB;
    vector<float> vec_unc_pt_A = gData.vec_unc_pt_A;
    vector<float> vec_unc_pt_B = gData.vec_unc_pt_B;
    vector<float> vec_unc_pt_AB = gData.vec_unc_pt_AB;
    vector<float> vec_unc_f_pt = gData.vec_unc_f_pt;
    float mean_pt = gData.mean_pt;

    // Calculating some means
    float vec_mean_pt_A = mean(vec_pt_A);
    float vec_mean_pt_B = mean(vec_pt_B);

    // Calculating the std deviation
    float sigma2 = (mean(vec_pt_AB) - (vec_mean_pt_A*vec_mean_pt_B));
    float sigma = sqrt(sigma2);
    // Std deviation uncertainty
    float unc_sigma_2nd_term = UncPropIndependentAtimesB(vec_mean_pt_B, UncMean(vec_unc_pt_A), vec_mean_pt_A, UncMean(vec_unc_pt_B));
    float unc_sigma = UncPropIndependentAminusB(UncMean(vec_unc_pt_AB), unc_sigma_2nd_term);

    cout << sigma << " +- " << unc_sigma << endl;

    // Calculating <f(pT)[pT]_B> term
    int nEvents_A = vec_f_pt.size();
    vector<vector<float>> vec_pt_B_f_pt(nEvents_A, vector<float>(nBins, 0.0));
    for (int j=0; j<nBins; j++){ // pT bins
        for (int i=0; i<nEvents_A; i++){ // Events
            vec_pt_B_f_pt[i][j] = vec_pt_B[i]*vec_f_pt[i][j];
        }
    }

    // Defining more auxiliar vectors
    vector<float> vec_v0pt;
    vector<float> vec_mean_pt_B_f_pt = double_vector_mean(vec_pt_B_f_pt, nBins);
    vector<float> vec_mean_f_pt = double_vector_mean(vec_f_pt, nBins);

    // Calculating v0(pT)
    for (int i=0; i<nBins; i++){
        float v0pt = ((vec_mean_pt_B_f_pt[i]-(vec_mean_f_pt[i]*vec_mean_pt_B))/(vec_mean_f_pt[i]*sigma));
        if (TMath::IsNaN(v0pt)) v0pt = 0.0;
        vec_v0pt.push_back(v0pt);
    }

    // Calculating v0(pT) uncertainty
    // Denom
    UncPropPartialCorrAtimesB()

    // Calculating v0(pT) sum rules
    float sum1_v0pt = 0;
    float sum2_v0pt_left = 0;
    float acc_sum2_v0pt_right = 0;
    for (int i=0; i<nBins; i++){
        sum1_v0pt += vec_v0pt[i]*vec_mean_f_pt[i];
        float pT = pT_axis[i];
        sum2_v0pt_left += pT*vec_v0pt[i]*vec_mean_f_pt[i];
        acc_sum2_v0pt_right += vec_mean_f_pt[i];
    }
    float sum2_v0pt_right = sigma*acc_sum2_v0pt_right;

    // Calculating Fig2(c)
    vector<float> vec_rel;
    for (int i=0; i<nBins; i++){
        float pT = pT_axis[i];
        float rel = ((vec_mean_pt_B_f_pt[i]-(vec_mean_f_pt[i]*vec_mean_pt_B))/(vec_mean_f_pt[i]*pT))*1e3;
        vec_rel.push_back(rel);
    }

    // Printing v0(pT) sum rules
    cout << "------------------ v0(pT) ------------------" << endl;
    cout << "Sum rule #1: " << sum1_v0pt << " = 0" << endl;
    cout << "Sum rule #2: " << sum2_v0pt_left << " = " << sum2_v0pt_right << endl << endl;


    // Calculating v0(pT)/v0
    // Gets <pT_A> from previously created hist
    float v0 = sigma/mean_pt; // Calculates the scaled v0
    vector<float> vec_v0ptv0;
    for (int i=0; i<nBins; i++){
        vec_v0ptv0.push_back(vec_v0pt[i]/v0); // Calculates v0(pT)/v0
    }

    cout << "v0 = " << v0 << endl;

    // Calculating v0(pT)/v0 sum rules
    float sum1_v0ptv0 = 0;
    float sum2_v0ptv0_left = 0;
    float acc_sum2_v0ptv0_right = 0;
    for (int i=0; i<nBins; i++){
        sum1_v0ptv0 += vec_v0ptv0[i]*vec_mean_f_pt[i];
        float pT = pT_axis[i];
        sum2_v0ptv0_left += pT*vec_v0ptv0[i]*vec_mean_f_pt[i];
        acc_sum2_v0ptv0_right += vec_mean_f_pt[i];
    }
    float sum2_v0ptv0_right = mean_pt*acc_sum2_v0ptv0_right;

    cout << "----------------- v0(pT)/v0 -----------------" << endl;
    cout << "Sum rule #1: " << sum1_v0ptv0 << " = 0" << endl;
    cout << "Sum rule #2: " << sum2_v0ptv0_left << " = " << sum2_v0ptv0_right << endl;    

    TFile *save_file = new TFile(Savename, "UPDATE");

    if (PlotType == "v0ptv0"){
        TGraph* gr_v0pt = new TGraph(nBins, pT_axis.data(), vec_v0pt.data());
        TString v0pt_name = "v0pt_";
        v0pt_name += Name;
        gr_v0pt->SetName(v0pt_name);
        gr_v0pt->Write();
        TGraph* gr_v0ptv0 = new TGraph(nBins, pT_axis.data(), vec_v0ptv0.data());
        TString v0ptv0_name = "v0ptv0_";
        v0ptv0_name += Name;
        gr_v0ptv0->SetName(v0ptv0_name);
        gr_v0ptv0->Write();
    }

    if (PlotType == "v0pt"){
        TGraph* gr_v0pt = new TGraph(nBins, pT_axis.data(), vec_v0pt.data());
        gr_v0pt->SetName(Name);
        gr_v0pt->Write();

    }
    if (PlotType == "v0"){
        float y_v0[1];
        float x_cent[1];
        y_v0[0] = v0;
        TString v0_name = "";
        if (HFSET_Max == 375.0){
            v0_name += "55_";
            x_cent[0] = 55.0;
        }
        if (HFSET_Max == 210.0){
            v0_name += "65_";
            x_cent[0] = 65.0;
        }
        v0_name += Name;
        TGraph* gr_v0 = new TGraph(1, x_cent, y_v0);
        gr_v0->SetName(v0_name);
        gr_v0->Write();
    }

    if (PlotType == "v0ptrel"){
        TGraph* gr_rel = new TGraph(nBins, pT_axis.data(), vec_rel.data());
        TString rel_name = "rel_";
        rel_name += Name;
        gr_rel->SetName(rel_name);
        gr_rel->Write();

        TGraph* gr_v0pt = new TGraph(nBins, pT_axis.data(), vec_v0pt.data());
        TString v0pt_name = "v0pt_";
        v0pt_name += Name;
        gr_v0pt->SetName(v0pt_name);
        gr_v0pt->Write();
    }

    save_file->Close();
}