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

struct Gathered_Data{
    vector<float> vec_dPt_A;
    vector<float> vec_dPt_B;
    float mean_pt;
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
    //const unsigned int nEvents = fTree->GetEntries();
    const unsigned int nEvents = 10000;
    vector<vector<float>> Matrix_trkPt_A;
    vector<vector<float>> Matrix_trkPt_B;
    vector<vector<float>> Matrix_trkW_A;
    vector<vector<float>> Matrix_trkW_B;
    vector<float> Vec_trkPt_A;
    vector<float> Vec_trkPt_B;
    vector<float> Vec_trkW_A;
    vector<float> Vec_trkW_B;

    TH1F *hist_all_pt_AB = new TH1F("all_pt", "All pT", 100, 0, 100); // Create histogram to calculate <pT_A> (all events)
    TH1F *hist_all_pt_A = new TH1F("all_pt_A", "All pT from subset A", 100, 0, 100); 
    TH1F *hist_all_pt_B = new TH1F("all_pt_B", "All pT from subset B", 100, 0, 100);
    for (Long64_t ievt=0; ievt<nEvents; ievt++){ // Loop over the events
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
                trkPt[iTrk] >= ptr_min && trkPt[iTrk] <= ptr_max && 
                trkDzSig[iTrk] > -3.0 && trkDzSig[iTrk] < 3.0 && 
                trkDxySig[iTrk] > -3.0 && trkDxySig[iTrk] < 3.0 && 
                trkPtRes[iTrk] < 0.1){
                    corrFac = getTrkCorrWeight(cFile, trkPt[iTrk], trkEta[iTrk]);
                    Vec_trkPt_A.push_back(trkPt[iTrk]);
                    Vec_trkW_A.push_back(corrFac);
                    hist_all_pt_A->Fill(trkPt[iTrk], corrFac);
                    hist_all_pt_AB->Fill(trkPt[iTrk], corrFac);
            }
        
            // Getting subset B: [pT]_B
            if (trkEta[iTrk] >= eta_gap/2 && trkEta[iTrk] <= 2.4 && 
                trkPt[iTrk] >= ptr_min && trkPt[iTrk] <= ptr_max && 
                trkDzSig[iTrk] > -3.0 && trkDzSig[iTrk] < 3.0 && 
                trkDxySig[iTrk] > -3.0 && trkDxySig[iTrk] < 3.0 && 
                trkPtRes[iTrk] < 0.1){
                    corrFac = getTrkCorrWeight(cFile, trkPt[iTrk], trkEta[iTrk]);
                    Vec_trkPt_B.push_back(trkPt[iTrk]);
                    Vec_trkW_B.push_back(corrFac);
                    hist_all_pt_B->Fill(trkPt[iTrk], corrFac);   
                    hist_all_pt_AB->Fill(trkPt[iTrk], corrFac);
            }
        }
        
        Matrix_trkPt_A.push_back(Vec_trkPt_A);
        Matrix_trkPt_B.push_back(Vec_trkPt_B);
        Matrix_trkW_A.push_back(Vec_trkW_A);
        Matrix_trkW_B.push_back(Vec_trkW_B);

        Vec_trkPt_A.shrink_to_fit();
        Vec_trkPt_B.shrink_to_fit();
        Vec_trkW_A.shrink_to_fit();
        Vec_trkW_B.shrink_to_fit();
    }

    float Mean_pt_A = hist_all_pt_A->GetMean();
    float Mean_pt_B = hist_all_pt_B->GetMean();
    float Mean_pt = hist_all_pt_AB->GetMean();

    vector<vector<float>> Matrix_dtrkPt_A; // Shape: nEvents x nTrk, but nTrk isn't fixed
    vector<vector<float>> Matrix_dtrkPt_B;
    vector<float> Vec_dtrkPt_A;
    vector<float> Vec_dtrkPt_B;
    for (int i=0; i<Matrix_trkPt_A.size(); i++){ // Events (i)
        for (int j=0; j<Matrix_trkPt_A[i].size(); j++){ // Tracks (j)
            Vec_dtrkPt_A.push_back(Matrix_trkPt_A[i][j]-Mean_pt_A);
            Vec_dtrkPt_B.push_back(Matrix_trkPt_B[i][j]-Mean_pt_B);
        }
        Matrix_dtrkPt_A.push_back(Vec_dtrkPt_A);
        Matrix_dtrkPt_B.push_back(Vec_dtrkPt_B);
        Vec_dtrkPt_A.clear();
        Vec_dtrkPt_B.clear();
    }

    int nValid_Events = Matrix_trkPt_A.size(); // Non-zero events

    vector<float> Vec_dPt_A(nValid_Events, 0.0);
    vector<float> Vec_dPt_B(nValid_Events, 0.0);

    for (int i=0; i<nValid_Events; i++){
        float sum_WdPt_A = 0.0; float sum_W_A = 0.0;
        float sum_WdPt_B = 0.0; float sum_W_B = 0.0;
        for (int j=0; j<Matrix_dtrkPt_A[i].size(); j++){
            sum_WdPt_A += Matrix_trkW_A[i][j]*Matrix_dtrkPt_A[i][j];
            sum_WdPt_B += Matrix_trkW_B[i][j]*Matrix_dtrkPt_B[i][j];
            sum_W_A += Matrix_trkW_A[i][j];
            sum_W_B += Matrix_trkW_B[i][j];
        }
        if (sum_W_A == 0) sum_W_A = 1.0;
        if (sum_W_B == 0) sum_W_B = 1.0;
        Vec_dPt_A[i] = sum_WdPt_A/sum_W_A;
        Vec_dPt_B[i] = sum_WdPt_B/sum_W_B;
        cout << Vec_dPt_A[i] << " " << sum_WdPt_A << " " << sum_W_A << endl;
        cout << Vec_dPt_B[i] << " " << sum_WdPt_B << " " << sum_W_B << endl;
    }

    delete hist_all_pt_A;
    delete hist_all_pt_B;
    delete hist_all_pt_AB;

    Gathered_Data struct_data;
    struct_data.vec_dPt_A = Vec_dPt_A;
    struct_data.vec_dPt_B = Vec_dPt_B;
    struct_data.mean_pt = Mean_pt;
    return struct_data;
}

// Thats the function we call to construct the observable
void ObsConstructor(float Eta_gap, float HFSET_Min, float HFSET_Max, float pTr_Min, float pTr_Max){
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
    float mean_pt = gData.mean_pt;

    int nEvents = vec_dPt_A.size();

    // v0
    vector<float> vec_dPt_AB(nEvents, 0.0);
    for (int i=0; i<nEvents; i++){
        vec_dPt_AB[i] = vec_dPt_A[i]*vec_dPt_B[i];
    }
    float v0 = sqrt(TMath::Mean(nEvents, vec_dPt_AB.data()))/mean_pt;

    cout << "v0 = " << v0 << endl;
}