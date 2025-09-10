// 50-60% CENTRALITY: HFSUMET = [210, 375] GeV
// 60-70% CENTRALITY: HFSUMET = [100, 210] GeV

#include <cmath>
#include "TMath.h"
using namespace std;

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

struct Gathered_Data{
    vector<float> vec_dPt_A;
    vector<float> vec_dPt_B;
    vector<vector<float>> vec_n_pt_AB;
    vector<vector<float>> vec_dn_pt_A;
    vector<vector<float>> vec_dn_pt_B;
    float mean_pt_ref;
};

Gathered_Data DataGathering(float eta_gap, float HFSET_min, float HFSET_max, float ptr_min, float ptr_max, vector<float> Xaxis_del, vector<float> pT_axis){
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
    const unsigned int nEvents = fTree->GetEntries();
    //const unsigned int nEvents = 100000;
    vector<vector<float>> Matrix_trkPt_A;
    vector<vector<float>> Matrix_trkPt_B;
    vector<vector<float>> Matrix_trkW_A;
    vector<vector<float>> Matrix_trkW_B;
    vector<vector<float>> Vec_n_pt_AB; // This vector will hold the fractions of pT of all events
    vector<vector<float>> Vec_n_pt_A; 
    vector<vector<float>> Vec_n_pt_B;
    vector<float> Vec_trkPt_A;
    vector<float> Vec_trkPt_B;
    vector<float> Vec_trkW_A;
    vector<float> Vec_trkW_B;
    
    TH1F *hist_all_pt_ref_AB = new TH1F("all_pt_ref", "All pT-ref", 100, 0, 100); // Create histogram to calculate <pT-ref_A> (all events)
    TH1F *hist_all_pt_ref_A = new TH1F("all_pt_ref_A", "All pT-ref from subset A", 100, 0, 100); 
    TH1F *hist_all_pt_ref_B = new TH1F("all_pt_ref_B", "All pT-ref from subset B", 100, 0, 100);
    TH1F *hist_all_pt_A = new TH1F("all_pt_A", "All pT from subset A", 100, 0, 100); 
    TH1F *hist_all_pt_B = new TH1F("all_pt_B", "All pT from subset B", 100, 0, 100);
    TH1F *hist_pt_AB = new TH1F("pt", "pT", nBins, Xaxis_del.data()); // Create histogram to scale pT bins and get n(pT)
    TH1F *hist_pt_A = new TH1F("pt_A", "pT from subset A", nBins, Xaxis_del.data());
    TH1F *hist_pt_B = new TH1F("pt_B", "pT from subset B", nBins, Xaxis_del.data());

    for (Long64_t ievt=0; ievt<nEvents; ievt++){ // Loop over the events
        vector<float> n_pt_A(nBins, 0.0); // Define vector to hold the fractions of pT in the event
        vector<float> n_pt_B(nBins, 0.0);
        vector<float> n_pt_AB(nBins, 0.0);

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
                    //float corrFac = getTrkCorrWeight(cFile, trkPt[iTrk], trkEta[iTrk]);
                    float corrFac = 1.0;
                    if (trkPt[iTrk] >= ptr_min && trkPt[iTrk] <= ptr_max){ // ARRUMAR
                        Vec_trkPt_A.push_back(trkPt[iTrk]);
                        Vec_trkW_A.push_back(corrFac);
                        hist_all_pt_ref_AB->Fill(trkPt[iTrk], corrFac);
                        hist_all_pt_ref_A->Fill(trkPt[iTrk], corrFac);
                    }
                    hist_pt_A->Fill(trkPt[iTrk], corrFac);
                    hist_pt_AB->Fill(trkPt[iTrk], corrFac);
                    hist_all_pt_A->Fill(trkPt[iTrk], corrFac);
            }
        
            // Getting subset B: [pT]_B
            if (trkEta[iTrk] >= eta_gap/2 && trkEta[iTrk] <= 2.4 && 
                trkPt[iTrk] >= 0.5 && trkPt[iTrk] <= 10.0 && 
                trkDzSig[iTrk] > -3.0 && trkDzSig[iTrk] < 3.0 && 
                trkDxySig[iTrk] > -3.0 && trkDxySig[iTrk] < 3.0 && 
                trkPtRes[iTrk] < 0.1){
                    //float corrFac = getTrkCorrWeight(cFile, trkPt[iTrk], trkEta[iTrk]);
                    float corrFac = 1.0;
                    if (trkPt[iTrk] >= ptr_min && trkPt[iTrk] <= ptr_max){ // ARRUMAR
                        Vec_trkPt_B.push_back(trkPt[iTrk]);
                        Vec_trkW_B.push_back(corrFac);
                        hist_all_pt_ref_AB->Fill(trkPt[iTrk], corrFac);
                        hist_all_pt_ref_B->Fill(trkPt[iTrk], corrFac);
                    }
                    hist_pt_B->Fill(trkPt[iTrk], corrFac);
                    hist_pt_AB->Fill(trkPt[iTrk], corrFac);
                    hist_all_pt_B->Fill(trkPt[iTrk], corrFac);
            }
        }

         // Scaling the created hist and taking the bins content to the array
        if (hist_pt_A->Integral() != 0) hist_pt_A->Scale(1/hist_pt_A->Integral()); // Defining a normalized histogram
        if (hist_pt_B->Integral() != 0) hist_pt_B->Scale(1/hist_pt_B->Integral());
        if (hist_pt_AB->Integral() != 0) hist_pt_AB->Scale(1/hist_pt_AB->Integral());
        for (int i=0; i<nBins; i++){
            n_pt_A[i] = hist_pt_A->GetBinContent(i+1);
            n_pt_B[i] = hist_pt_B->GetBinContent(i+1);
            n_pt_AB[i] = hist_pt_AB->GetBinContent(i+1);
        }
        hist_pt_A->Reset();
        hist_pt_B->Reset();
        hist_pt_AB->Reset();
        // Setting the desired vectors
        Vec_n_pt_A.push_back(n_pt_A);
        Vec_n_pt_B.push_back(n_pt_B);
        Vec_n_pt_AB.push_back(n_pt_AB);

        Matrix_trkPt_A.push_back(Vec_trkPt_A);
        Matrix_trkPt_B.push_back(Vec_trkPt_B);
        Matrix_trkW_A.push_back(Vec_trkW_A);
        Matrix_trkW_B.push_back(Vec_trkW_B);

        Vec_trkPt_A.clear();
        Vec_trkPt_B.clear();
        Vec_trkW_A.clear();
        Vec_trkW_B.clear();
        Vec_trkPt_A.shrink_to_fit();
        Vec_trkPt_B.shrink_to_fit();
        Vec_trkW_A.shrink_to_fit();
        Vec_trkW_B.shrink_to_fit();
    }

    float Mean_pt_A = hist_all_pt_A->GetMean();
    float Mean_pt_B = hist_all_pt_B->GetMean();
    float Mean_pt_ref = hist_all_pt_ref_AB->GetMean();
    float Mean_pt_ref_A = hist_all_pt_ref_A->GetMean();
    float Mean_pt_ref_B = hist_all_pt_ref_B->GetMean();

    vector<vector<float>> Matrix_dtrkPt_A; // Shape: nEvents x nTrk, but nTrk isn't fixed
    vector<vector<float>> Matrix_dtrkPt_B;
    vector<float> Vec_dtrkPt_A;
    vector<float> Vec_dtrkPt_B;
    for (int i=0; i<Matrix_trkPt_A.size(); i++){ // Events (i)
        for (int j=0; j<Matrix_trkPt_A[i].size(); j++){ // Tracks (j)
            Vec_dtrkPt_A.push_back(Matrix_trkPt_A[i][j]-Mean_pt_A);
        }
        Matrix_dtrkPt_A.push_back(Vec_dtrkPt_A);
        Vec_dtrkPt_A.clear();
    }
    for (int i=0; i<Matrix_trkPt_B.size(); i++){ // Events (i)
        for (int j=0; j<Matrix_trkPt_B[i].size(); j++){ // Tracks (j)
            Vec_dtrkPt_B.push_back(Matrix_trkPt_B[i][j]-Mean_pt_B);
        }
        Matrix_dtrkPt_B.push_back(Vec_dtrkPt_B);
        Vec_dtrkPt_B.clear();
    }

    int nValid_Events = Matrix_trkPt_A.size(); // Non-zero events. Subset A and B have de same number of events

    vector<float> Vec_dPt_A(nValid_Events, 0.0);
    vector<float> Vec_dPt_B(nValid_Events, 0.0);

    for (int i=0; i<nValid_Events; i++){
        float sum_WdPt_A = 0.0; float sum_W_A = 0.0;
        float sum_WdPt_B = 0.0; float sum_W_B = 0.0;
        for (int j=0; j<Matrix_dtrkPt_A[i].size(); j++){
            sum_WdPt_A += Matrix_trkW_A[i][j]*Matrix_dtrkPt_A[i][j];
            sum_W_A += Matrix_trkW_A[i][j];
        }
        for (int j=0; j<Matrix_dtrkPt_B[i].size(); j++){
            sum_WdPt_B += Matrix_trkW_B[i][j]*Matrix_dtrkPt_B[i][j];
            sum_W_B += Matrix_trkW_B[i][j];
        }
        if (sum_W_A == 0) sum_W_A = 1.0;
        if (sum_W_B == 0) sum_W_B = 1.0;
        Vec_dPt_A[i] = sum_WdPt_A/sum_W_A;
        Vec_dPt_B[i] = sum_WdPt_B/sum_W_B;
    }

    cout << Vec_n_pt_A.size() << " " << Vec_n_pt_B.size() << endl;
    cout << transpose(Vec_n_pt_A)[0].size() << endl;

    // Calculating delta n(pT)
    vector<float> Vec_mean_n_pt_A(nBins, 0.0);
    vector<float> Vec_mean_n_pt_B(nBins, 0.0);
    vector<vector<float>> Vec_dn_pt_A(nValid_Events, vector<float>(nBins, 0.0));
    vector<vector<float>> Vec_dn_pt_B(nValid_Events, vector<float>(nBins, 0.0));
    for (int i=0; i<nBins; i++){
        Vec_mean_n_pt_A[i] = TMath::Mean(nValid_Events, transpose(Vec_n_pt_A)[i].data());
        Vec_mean_n_pt_B[i] = TMath::Mean(nValid_Events, transpose(Vec_n_pt_B)[i].data());
    }
    for (int i=0; i<nValid_Events; i++){
        for (int j=0; j<nBins; j++){
            Vec_dn_pt_A[i][j] = Vec_n_pt_A[i][j] - Vec_mean_n_pt_A[j];
            Vec_dn_pt_B[i][j] = Vec_n_pt_B[i][j] - Vec_mean_n_pt_B[j];
        }
    }

    
    delete hist_all_pt_ref_A;
    delete hist_all_pt_ref_B;
    delete hist_all_pt_ref_AB;

    Gathered_Data struct_data;
    struct_data.vec_dPt_A = Vec_dPt_A;
    struct_data.vec_dPt_B = Vec_dPt_B;
    struct_data.vec_n_pt_AB = Vec_n_pt_AB;
    struct_data.vec_dn_pt_A = Vec_dn_pt_A;
    struct_data.vec_dn_pt_B = Vec_dn_pt_B;
    struct_data.mean_pt_ref = Mean_pt_ref;
    return struct_data;
}

// Thats the function we call to construct the observable
void ObsConstructor(float Eta_gap, float HFSET_Min, float HFSET_Max, float pTr_Min, float pTr_Max, TString Name, TString Savename, string PlotType){
    int B = 100; // Number of Poisson bootstrap samples
    // Defining bins and plot's x axes
    vector<float> Xaxis_del = {0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.2, 1.4, 1.6, 1.8, 1.98, 2.2, 2.38, 2.98, 3.18, 6.0, 8.04, 10.0}; // Those are the END of each bin, not the middle
    int nBins = (Xaxis_del.size()-1);
    vector<float> pT_axis;
    for (int i=0; i<nBins; i++){
        pT_axis.push_back((Xaxis_del[i+1]+Xaxis_del[i])/2); // Here we define the x axis points as the middle of each bin
    }

    Gathered_Data gData = DataGathering(Eta_gap, HFSET_Min, HFSET_Max, pTr_Min, pTr_Max, Xaxis_del, pT_axis);
    vector<float> vec_dPt_A = gData.vec_dPt_A;
    vector<float> vec_dPt_B = gData.vec_dPt_B;
    vector<vector<float>> vec_n_pt_AB = gData.vec_n_pt_AB;
    vector<vector<float>> vec_dn_pt_A = gData.vec_dn_pt_A;
    vector<vector<float>> vec_dn_pt_B = gData.vec_dn_pt_B;
    float mean_pt_ref = gData.mean_pt_ref;

    int nEvents = vec_dPt_A.size();

    // v0
    vector<float> vec_dPt_AB(nEvents, 0.0);
    for (int i=0; i<nEvents; i++){
        float h_dPt_AB = vec_dPt_A[i]*vec_dPt_B[i];
        if (TMath::IsNaN(h_dPt_AB) || !TMath::Finite(h_dPt_AB)){
            vec_dPt_AB[i] = 0.0;
        } else{
            vec_dPt_AB[i] = h_dPt_AB;
        }
    }
    float sigma = sqrt(TMath::Mean(nEvents, vec_dPt_AB.data()));
    float v0 = sigma/mean_pt_ref;
    // v0 uncertainty
    float unc_v0 = StdPoissonBootstrap(vec_dPt_AB, B);
    cout << "v0 = " << v0 << " +- " << unc_v0 << endl;
    

    // Calculating n_A(pT)*d[pT]_B + n_B(pT)*d[pT]_A
    vector<vector<float>> vec_sum_dpt_dn_pt(nEvents, vector<float>(nBins, 0.0));
    for (int i=0; i<nEvents; i++){ // Events
        for (int j=0; j<nBins; j++){ // pT bins
            vec_sum_dpt_dn_pt[i][j] = (vec_dn_pt_A[i][j]*vec_dPt_B[i])+(vec_dn_pt_B[i][j]*vec_dPt_A[i]);
        }
    }

    // Defining auxiliar vectors to calculate v0(pT)v0
    vector<float> vec_v0ptv0(nBins, 0.0);
    vector<float> vec_v0ptv0_denom(nBins, 0.0);
    vector<float> vec_mean_n_pt_AB = double_vector_mean(vec_n_pt_AB, nBins);

    // Calculating v0(pT)v0 and its uncertainty
    vector<float> vec_v0ptv0_num = double_vector_mean(vec_sum_dpt_dn_pt, nBins); // v0(pT) numerator
    for (int i=0; i<nBins; i++){
        // Calculating the observable v0(pT)v0
        vec_v0ptv0_denom[i] = vec_mean_n_pt_AB[i]*mean_pt_ref;
        vec_v0ptv0[i] = 0.5*vec_v0ptv0_num[i]/vec_v0ptv0_denom[i];
        cout << vec_v0ptv0[i] << " " << vec_v0ptv0_num[i] << " " << vec_mean_n_pt_AB[i] << " " << mean_pt_ref << endl;
        // Calculating the uncertainty of v0(pT)
    }

    vector<float> vec_v0pt(nBins, 0.0);
    // Calculating v0(pT)
    for (int i=0; i<nBins; i++){
        vec_v0pt[i] = vec_v0ptv0[i]/v0;
    }

     // Calculating v0(pT) sum rules
    float sum1_v0pt = 0;
    float sum2_v0pt_left = 0;
    float acc_sum2_v0pt_right = 0;
    for (int i=0; i<nBins; i++){
        sum1_v0pt += vec_v0pt[i]*vec_mean_n_pt_AB[i];
        float pT = pT_axis[i];
        sum2_v0pt_left += pT*vec_v0pt[i]*vec_mean_n_pt_AB[i];
        acc_sum2_v0pt_right += vec_mean_n_pt_AB[i];
    }
    float sum2_v0pt_right = sigma*acc_sum2_v0pt_right;

    // Printing v0(pT) sum rules
    cout << "------------------ v0(pT) ------------------" << endl;
    cout << "Sum rule #1: " << sum1_v0pt << " = 0" << endl;
    cout << "Sum rule #2: " << sum2_v0pt_left << " = " << sum2_v0pt_right << endl << endl;

    

    TFile *save_file = new TFile(Savename, "UPDATE");

    if (PlotType == "v0ptv0_ptref"){
        vector<float> vec_v0ptv0_plot(nBins, 0.0);
        for (int i=0; i<nBins; i++){
            vec_v0ptv0_plot[i] = 1e3*vec_v0ptv0[i];
        }
        TGraph* gr_v0ptv0_ptref = new TGraph(nBins, pT_axis.data(), vec_v0ptv0_plot.data());
        TString gname = "v0ptv0_ptref_";
        gname += Name;
        gr_v0ptv0_ptref->SetName(gname);
        gr_v0ptv0_ptref->Write();
    }

    if (PlotType == "v0ptv0"){
        vector<float> vec_zeros(nBins, 0.0);
        //TGraphErrors* gr_v0pt = new TGraphErrors(nBins, pT_axis.data(), vec_v0pt.data(), vec_zeros.data(), vec_unc_v0pt.data());
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

    save_file->Close();
}