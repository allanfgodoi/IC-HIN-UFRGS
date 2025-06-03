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
    float mean_pt;
};

Gathered_Data DataGathering(int eta_gap, float HFSET_min, float HFSET_max, float ptr_min, float ptr_max, TString savename){
    // Open CMS OpenData 2.76 TeV 50-70% centrality ROOT file
    TFile *file(0);
    TString filename = "/home/allanfgodoi/Desktop/IC-HIN-UFRGS/CollectivityHIC/Data/HiForestAOD_UPC.root";
    if (!gSystem->AccessPathName(filename)) // Verify if the file really exists
        file = TFile::Open(filename);
    if (!file) {
        cout << "ERROR: couldn't open datafile!";
        exit(1);
    }

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
    float pt_min = 0.5;
    float pt_max = 10.0;

    const unsigned int nBins = 95; // This way I have 1 bin to a delta pT = 0.1 in the range
    const unsigned int nEvents = fTree->GetEntries();
    vector<vector<float>> Vec_f_pt; // This vector will hold the fractions of pT of all events
    vector<float> Vec_pt_A;
    vector<float> Vec_pt_B;
    vector<float> Vec_pt_AB;

    TH1F *hist_all_pt_A = new TH1F("all_pt_A", "All pT from subset A", nBins, pt_min, ptr_max); // Create histogram to calculate <pT_A> (all events)
    TH1F *hist_pt_A = new TH1F("pt_A", "pT from subset A", nBins, pt_min, pt_max); // Create histogram to scale pT bins and get f(pT)
    for (Long64_t ievt=0; ievt<nEvents; ievt++){ // Loop over the events
        vector<float> f_pt(nBins, 0.0); // Define vector to hold the fractions of pT in the event
        float h_pt_A = 0;
        float h_pt_B = 0;
        int nTrk_A = 0;
        int nTrk_B = 0;

        fTree->GetEntry(ievt); // Here I get the events in the opened file
        if (ievt%10000 == 0)
            cout << "Processing event: " << ievt << endl;

        
        if (HFsumET < HFSET_min || HFSET_max > 375.0) // Applying 100 <= HFsumET <= 375 filter (related to centrality)
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
                h_pt_A += trkPt[iTrk]; // Sum all pT from subset A in the desired range
                nTrk_A += 1; // Counts the number of pT entries from subset A
                hist_pt_A->Fill(trkPt[iTrk]); // Fills the auxiliar histogram to scale f(pT) later
                hist_all_pt_A->Fill(trkPt[iTrk]);
            }
            // Getting subset B: [pT]_B
            if (trkEta[iTrk] >= eta_gap/2 && trkEta[iTrk] <= 2.4 && 
                trkPt[iTrk] >= ptr_min && trkPt[iTrk] <= ptr_max && 
                trkDzSig[iTrk] > -3.0 && trkDzSig[iTrk] < 3.0 && 
                trkDxySig[iTrk] > -3.0 && trkDxySig[iTrk] < 3.0 && 
                trkPtRes[iTrk] < 0.1){
                h_pt_B += trkPt[iTrk]; // Sum all pT from subset B
                nTrk_B += 1; // Counts the number of pT entries from subset B
            }
        }

        if (nTrk_A == 0) nTrk_A = 1;
        if (nTrk_B == 0) nTrk_B = 1;
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

    delete hist_all_pt_A;
    delete hist_pt_A;

    Gathered_Data struct_data;
    struct_data.vec_f_pt = Vec_f_pt;
    struct_data.vec_pt_A = Vec_pt_A;
    struct_data.vec_pt_B = Vec_pt_B;
    struct_data.vec_pt_AB = Vec_pt_AB;
    struct_data.mean_pt = Mean_pt;

    return struct_data;
}

void ObsConstructor(float Eta_gap, float HFSET_Min, float HFSET_Max, float pTr_Min, float pTr_Max, TString Name, TString Savename, string PlotType){
    constexpr float pt_min = 0.5;
    constexpr float pt_max = 10.0;
    constexpr int nBins = 10*(pt_max-pt_min);

    Gathered_Data gData = DataGathering(Eta_gap, HFSET_Min, HFSET_Max, pTr_Min, pTr_Max, Savename);
    vector<vector<float>> vec_f_pt = gData.vec_f_pt;
    vector<float> vec_pt_A = gData.vec_pt_A;
    vector<float> vec_pt_B = gData.vec_pt_B;
    vector<float> vec_pt_AB = gData.vec_pt_AB;
    float mean_pt = gData.mean_pt;

    // Calculating the std deviation
    float sigma2 = (mean(vec_pt_AB) - (mean(vec_pt_A)*mean(vec_pt_B)));
    float sigma = sqrt(sigma2);

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
    float vec_mean_pt_B = mean(vec_pt_B);

    // Calculating v0(pT)
    for (int i=0; i<nBins; i++){
        float v0pt = ((vec_mean_pt_B_f_pt[i]-(vec_mean_f_pt[i]*vec_mean_pt_B))/(vec_mean_f_pt[i]*sigma));
        if (TMath::IsNaN(v0pt)) v0pt = 0.0;
        vec_v0pt.push_back(v0pt);
    }

    // Calculating v0(pT) sum rules
    float sum1_v0pt = 0;
    float sum2_v0pt_left = 0;
    float acc_sum2_v0pt_right = 0;
    for (int i=0; i<nBins; i++){
        sum1_v0pt += vec_v0pt[i]*vec_mean_f_pt[i];
        float pT = (i*0.1+(pt_min+0.1)-0.05);
        sum2_v0pt_left += pT*vec_v0pt[i]*vec_mean_f_pt[i];
        acc_sum2_v0pt_right += vec_mean_f_pt[i];
    }
    float sum2_v0pt_right = sigma*acc_sum2_v0pt_right;

    // Calculating Fig2(c)
    vector<float> vec_rel;
    for (int i=0; i<nBins; i++){
        float pT = (i*0.1+(pt_min+0.1)-0.05);
        float rel = ((vec_mean_pt_B_f_pt[i]-(vec_mean_f_pt[i]*vec_mean_pt_B))/(vec_mean_f_pt[i]*pT))*1e-3;
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

    // Calculating v0(pT)/v0 sum rules
    float sum1_v0ptv0 = 0;
    float sum2_v0ptv0_left = 0;
    float acc_sum2_v0ptv0_right = 0;
    for (int i=0; i<nBins; i++){
        sum1_v0ptv0 += vec_v0ptv0[i]*vec_mean_f_pt[i];
        float pT = (i*0.1+(pt_min+0.1)-0.05);
        sum2_v0ptv0_left += pT*vec_v0ptv0[i]*vec_mean_f_pt[i];
        acc_sum2_v0ptv0_right += vec_mean_f_pt[i];
    }
    float sum2_v0ptv0_right = mean_pt*acc_sum2_v0ptv0_right;

    cout << "----------------- v0(pT)/v0 -----------------" << endl;
    cout << "Sum rule #1: " << sum1_v0ptv0 << " = 0" << endl;
    cout << "Sum rule #2: " << sum2_v0ptv0_left << " = " << sum2_v0ptv0_right << endl;    

    TFile *save_file = new TFile(Savename, "UPDATE");

    if (PlotType == "v0ptv0"){
        float arr_pT[nBins];
        float arr_v0pt[nBins];
        float arr_v0ptv0[nBins];
        for (int i=0; i<nBins; i++){
            arr_pT[i] = (i*0.1+(pt_min+0.1)-0.05);
            arr_v0pt[i] = vec_v0pt[i];
            arr_v0ptv0[i] = vec_v0ptv0[i];
        }

        TGraph* gr_v0pt = new TGraph(nBins, arr_pT, arr_v0pt);
        TString v0pt_name = "v0pt_";
        v0pt_name += Name;
        gr_v0pt->SetName(v0pt_name);
        gr_v0pt->Write();
        TGraph* gr_v0ptv0 = new TGraph(nBins, arr_pT, arr_v0ptv0);
        TString v0ptv0_name = "v0ptv0_";
        v0ptv0_name += Name;
        gr_v0ptv0->SetName(v0ptv0_name);
        gr_v0ptv0->Write();
    }

    if (PlotType == "v0pt"){
        float arr_pT[nBins];
        float arr_v0pt[nBins];
        for (int i=0; i<nBins; i++){
            arr_pT[i] = (i*0.1+(pt_min+0.1)-0.05);
            arr_v0pt[i] = vec_v0pt[i];
        }

        TGraph* gr_v0pt = new TGraph(nBins, arr_pT, arr_v0pt);
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
        float arr_pT[nBins];
        float arr_rel[nBins];
        float arr_v0pt[nBins];
        for (int i=0; i<nBins; i++){
            arr_pT[i] = (i*0.1+(pt_min+0.1)-0.05);
            arr_rel[i] = vec_rel[i];
            arr_v0pt[i] = vec_v0pt[i];
        }
        TGraph* gr_rel = new TGraph(nBins, arr_pT, arr_rel);
        TString rel_name = "rel_";
        rel_name += Name;
        gr_rel->SetName(rel_name);
        gr_rel->Write();

        TGraph* gr_v0pt = new TGraph(nBins, arr_pT, arr_v0pt);
        TString v0pt_name = "v0pt_";
        v0pt_name += Name;
        gr_v0pt->SetName(v0pt_name);
        gr_v0pt->Write();
        }
    save_file->Close();
}