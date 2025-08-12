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
        vector<float> resampled(N, 0);
        float n = 0;
        for (int j=0; j<N; j++){
            float rd = rndgen.Poisson(lambda);
            n += rd;
            resampled[j] = x[j]*rd;
        }
        bootstrapped[i] = TMath::Mean(n, resampled.data());
    }
    float bootstrapped_error = TMath::StdDev(B, bootstrapped.data());
    return bootstrapped_error;
}

vector<vector<float>> transpose(vector<vector<float>> x){
    int rows = x.size();
    int cols = x[0].size();
    vector<vector<float>> transposed(cols, vector<float>(rows, 0.0));
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            transposed[j][i] = x[i][j];
        }
    }
    return transposed;
}

float UncPropIndependentAminusB(float sigmaEA, float sigmaEB){
    float s = sqrt(pow(sigmaEA, 2) + pow(sigmaEB, 2));
    return s;
}

float UncPropTotCorrAminusB(float sigmaEA, float sigmaEB){
    float s = sqrt(fabs(pow(sigmaEA, 2) - pow(sigmaEB, 2)));
    return s;
}

float UncPropIndependentAtimesB(float EA, float sigmaEA, float EB, float sigmaEB){
    float s = sqrt(pow(EB*sigmaEA, 2) + pow(EA*sigmaEB, 2));
    return s;
}

float UncPropTotCorrAoverB(float EA, float sigmaEA, float EB, float sigmaEB){
    float s = abs(EA/EB)*sqrt(pow(sigmaEA/EA, 2) + pow(sigmaEB/EB, 2) - 2*pow(sigmaEA, 2)/(EA*EB));
    return s;
}

float UncPropTotCorrAtimesB(float EA, float sigmaEA, float EB, float sigmaEB){
    float s = sqrt(pow(EB*sigmaEA, 2) + pow(EA*sigmaEB, 2) + 2*EB*EA*pow(sigmaEA, 2));
    return s;
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

Gathered_Data DataGathering(float eta_gap, float HFSET_min, float HFSET_max, float ptr_min, float ptr_max, TString savename, vector<float> Xaxis_del, vector<float> pT_axis, string Correction){
    // Open CMS OpenData 2.76 TeV 50-70% centrality ROOT file
    TFile *file(0);
    TFile *cFile(0);
    TString filename = "/home/allanfgodoi/Desktop/IC-HIN-UFRGS/CollectivityHIC/Data/HiForestAOD_DATA2011_MB_ppReReco.root";
    TString cFilename = "/home/allanfgodoi/Desktop/IC-HIN-UFRGS/CollectivityHIC/Data/TrackCorrections_HIJING_538_OFFICIAL_Mar24.root";

    file = TFile::Open(filename);
    cFile = TFile::Open(cFilename);

    const unsigned int nDummy = 30000; // Used to create a dummy len array to hold the tracks

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
    TTree* fTree = (TTree*)file->Get("demo/TreeMBUCC");
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
    //const unsigned int nEvents = fTree->GetEntries();
    const unsigned int nEvents = 560000;
    vector<vector<float>> Vec_f_pt; // This vector will hold the fractions of pT of all events
    vector<float> Vec_pt_A;
    vector<float> Vec_pt_B;
    vector<float> Vec_pt_AB;

    TH1F *hist_all_pt_A = new TH1F("all_pt_A", "All pT from subset A", nBins, Xaxis_del.data()); // Create histogram to calculate <pT_A> (all events)
    TH1F *hist_all_pt_B = new TH1F("all_pt_B", "All pT from subset B", nBins, Xaxis_del.data());
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
                    hist_pt_A->Fill(trkPt[iTrk], corrFac); // Fills the auxiliar histogram to scale f(pT) later
                    hist_all_pt_A->Fill(trkPt[iTrk], corrFac);
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
                    hist_all_pt_B->Fill(trkPt[iTrk], corrFac);
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
    }

    float Mean_pt = hist_all_pt_A->GetMean();
    float Mean_ptB = hist_all_pt_B->GetMean();

    cout << "<pT>_A = " << Mean_pt << endl;
    cout << "<pT>_B = " << Mean_ptB << endl;

    delete hist_all_pt_A;
    delete hist_all_pt_B;
    delete hist_pt_A;

    Gathered_Data struct_data;
    struct_data.vec_f_pt = Vec_f_pt;
    struct_data.vec_pt_A = Vec_pt_A;
    struct_data.vec_pt_B = Vec_pt_B;
    struct_data.vec_pt_AB = Vec_pt_AB;
    struct_data.mean_pt = Mean_pt;

    return struct_data;
}

// Thats the function we call to construct the observable
void ObsConstructor(float Eta_gap, float HFSET_Min, float HFSET_Max, float pTr_Min, float pTr_Max, TString Name, TString Savename, string PlotType, string Correction){
    int B = 100;
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
    float mean_pt = gData.mean_pt;

    // Calculating some means
    float mean_pt_A = mean(vec_pt_A);
    float mean_pt_B = mean(vec_pt_B);
    float mean_pt_AB = mean(vec_pt_AB);
    
    // Bootstrapping samples
    float unc_mean_pt_A = StdPoissonBootstrap(vec_pt_A, B);
    float unc_mean_pt_B = StdPoissonBootstrap(vec_pt_B, B);
    float unc_mean_pt_AB = StdPoissonBootstrap(vec_pt_AB, B);

    // Calculating sigma
    float sigma = sqrt(mean_pt_AB - (mean_pt_A*mean_pt_B));
    // Calculating sigma uncertainty
    float unc_sigma2_2nd_term = UncPropIndependentAtimesB(mean_pt_A, unc_mean_pt_A, mean_pt_B, unc_mean_pt_B);
    float unc_sigma2 = UncPropTotCorrAminusB(unc_mean_pt_AB, unc_sigma2_2nd_term);
    float unc_sigma = unc_sigma2/(2*sigma);

    // Calculating f(pT)[pT]_B term to calculate v0(pT)
    int nEvents_A = vec_f_pt.size();
    vector<vector<float>> vec_pt_B_f_pt(nEvents_A, vector<float>(nBins, 0.0));
    for (int j=0; j<nBins; j++){ // pT bins
        for (int i=0; i<nEvents_A; i++){ // Events
            vec_pt_B_f_pt[i][j] = vec_pt_B[i]*vec_f_pt[i][j];
        }
    }

    // Defining auxiliar vectors to calculate v0(pT)
    vector<float> vec_v0pt(nBins, 0.0);
    vector<float> vec_v0pt_num(nBins, 0.0);
    vector<float> vec_v0pt_denom(nBins, 0.0);
    vector<float> vec_mean_f_pt = double_vector_mean(vec_f_pt, nBins);
    vector<float> vec_mean_pt_B_f_pt = double_vector_mean(vec_pt_B_f_pt, nBins);

    // Bootstrapping more samples
    vector<float> vec_unc_mean_f_pt(nBins, 0.0);
    vector<float> vec_unc_mean_pt_B_f_pt(nBins, 0.0);
    for (int i=0; i<nBins; i++){
        vec_unc_mean_f_pt[i] = StdPoissonBootstrap(transpose(vec_f_pt)[i], B);
        vec_unc_mean_pt_B_f_pt[i] = StdPoissonBootstrap(transpose(vec_pt_B_f_pt)[i], B);
    }
    
    // Defining auxiliar vectors to calculate v0(pT) uncertainty
    vector<float> vec_unc_v0pt_denom(nBins, 0.0);
    vector<float> vec_unc_v0pt_num_2(nBins, 0.0);
    vector<float> vec_unc_v0pt_num(nBins, 0.0);
    vector<float> vec_unc_v0pt(nBins, 0.0);

    // Calculating v0(pT) and its uncertainty
    for (int i=0; i<nBins; i++){
        // Calculating the observable v0(pT)
        vec_v0pt_num[i] = vec_mean_pt_B_f_pt[i]-vec_mean_f_pt[i]*mean_pt_B;
        vec_v0pt_denom[i] = vec_mean_f_pt[i]*sigma;
        vec_v0pt[i] = vec_v0pt_num[i]/vec_v0pt_denom[i];
        // Calculating the uncertainty of v0(pT)
        vec_unc_v0pt_num_2[i] = UncPropIndependentAtimesB(vec_mean_f_pt[i], vec_unc_mean_f_pt[i], mean_pt_B, unc_mean_pt_B);
        vec_unc_v0pt_num[i] = UncPropTotCorrAminusB(vec_unc_mean_pt_B_f_pt[i], vec_unc_v0pt_num_2[i]);
        vec_unc_v0pt_denom[i] = UncPropIndependentAtimesB(vec_mean_f_pt[i], vec_unc_mean_f_pt[i], sigma, unc_sigma); // Try both independent and tot. correlated
        vec_unc_v0pt[i] = UncPropTotCorrAoverB(vec_v0pt_num[i], vec_unc_v0pt_num[i], vec_v0pt_denom[i], vec_unc_v0pt_denom[i]);
    }


    cout << "<[pT]_A> = " << mean_pt_A << " +- " << unc_mean_pt_A << " ; <[pT]_B> = " << mean_pt_B << " +- " << unc_mean_pt_B << "<[pT]_A><[pT]_B> = " << mean_pt_A*mean_pt_B << " +- " << unc_sigma2_2nd_term << " ; <[pT]_A[pT]_B> = " << mean_pt_AB << " +- " << unc_mean_pt_AB << endl;
    cout << "sigma = " << sigma << " +- " << unc_sigma << endl;
    for (int i=0; i<nBins; i++){
        cout << "-------- i = " << i << " --------" << endl;
        cout << "<f(pT)> = " << vec_mean_f_pt[i] << "+-" << vec_unc_mean_f_pt[i] << " ; <f(pT)[pT]_B> = " << vec_mean_pt_B_f_pt[i] << " +- " << vec_unc_mean_pt_B_f_pt[i] << "<f(pT)><[pT]_B> = " << vec_mean_f_pt[i]*mean_pt_B << " +- " << vec_unc_v0pt_num_2[i] << endl;
        cout << "<f(pT)[pT]_B> - <f(pT)><[pT]_B> = " << vec_v0pt_num[i] << " +- " << vec_unc_v0pt_num[i] << " ; <f(pT)>sigma = " << vec_v0pt_denom[i] << " +- " <<  vec_unc_v0pt_denom[i] << " ; v0(pT) = " << vec_v0pt[i] << " +- " << vec_unc_v0pt[i] << endl;
    }


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
        float rel = ((vec_mean_pt_B_f_pt[i]-(vec_mean_f_pt[i]*mean_pt_B))/(vec_mean_f_pt[i]*pT))*1e3;
        vec_rel.push_back(rel);
    }

    // Printing v0(pT) sum rules
    cout << "------------------ v0(pT) ------------------" << endl;
    cout << "Sum rule #1: " << sum1_v0pt << " = 0" << endl;
    cout << "Sum rule #2: " << sum2_v0pt_left << " = " << sum2_v0pt_right << endl << endl;


    // Calculating v0(pT)/v0 and its uncertainty
    float v0 = sigma/mean_pt; // Calculates the scaled v0
    vector<float> vec_v0ptv0;
    vector<float> vec_unc_v0ptv0(nBins, 0.0);
    for (int i=0; i<nBins; i++){
        // Calculating v0(pT)/v0
        vec_v0ptv0.push_back(vec_v0pt[i]/v0); // Calculates v0(pT)/v0
        // Calculating v0(pT)/v0 uncertainty (USING unc_sigma AS V0 UNCERTAINTY!!!)
        vec_unc_v0ptv0[i] = UncPropTotCorrAoverB(vec_v0pt[i], vec_unc_v0pt[i], v0, unc_sigma);
    }

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

    // Creating TGraphErrors and saving in ROOT file
    TFile *save_file = new TFile(Savename, "UPDATE");

    if (PlotType == "v0ptv0"){
        vector<float> vec_zeros(nBins, 0.0);
        TGraphErrors* gr_v0pt = new TGraphErrors(nBins, pT_axis.data(), vec_v0pt.data(), vec_zeros.data(), vec_unc_v0pt.data());
        //TGraph* gr_v0pt = new TGraph(nBins, pT_axis.data(), vec_v0pt.data());
        TString v0pt_name = "v0pt_";
        v0pt_name += Name;
        gr_v0pt->SetName(v0pt_name);
        gr_v0pt->Write();
        TGraphErrors* gr_v0ptv0 = new TGraphErrors(nBins, pT_axis.data(), vec_v0ptv0.data(), vec_zeros.data(), vec_unc_v0ptv0.data());
        //TGraph* gr_v0ptv0 = new TGraph(nBins, pT_axis.data(), vec_v0ptv0.data());
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
