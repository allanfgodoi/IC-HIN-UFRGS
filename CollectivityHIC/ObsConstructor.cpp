// RUN COMMAND: root -l -b -q 'ObsConstructor.cpp'

#include <cmath>
#include "TMath.h"
using namespace std;

// Calculates the mean of a given vector
float mean(vector<float> x){

    int N = 0;
    float acc = 0.0;
    for (int i=0; i<x.size(); i++){
        if (!TMath::IsNaN(x[i])){
            acc += x[i];
            N += 1;
        }
    }
    float m = acc/N;
    return m;
}

// 
vector<float> product(vector<float> x, vector<float> y){

    vector<float> vec;
    for (int i=0; i<x.size(); i++){ // x and y must have the same size
        vec.push_back(x[i]*y[i]);
    }
    return vec;

}

void ObsConstructor(){

    // Open file
    TFile *file(0);
    TString filename = "/home/allanfgodoi/Desktop/IC-HIN-UFRGS/CollectivityHIC/Data/HiForestAOD_UPC.root";
    if (!gSystem->AccessPathName(filename))
        file = TFile::Open(filename);
    if (!file) {
        cout << "ERROR: couldn't open datafile!";
        exit(1);
    }

    const unsigned int nDummy = 10000; // Used to create a dummy len array to hold the tracks

    // Declare arrays and scalars to hold the TTree files
    Float_t HFsumET = 0;
    Int_t Ntrk = 0;
    Float_t trkPt[nDummy];
    Float_t trkEta[nDummy];
    Float_t trkPhi[nDummy];
    Float_t trkPtRes[nDummy];
    Float_t trkDzSig[nDummy];
    Float_t trkDxySig[nDummy];
    Float_t trkNpixLayers[nDummy];

    // Addresses TTree branches to declared variables
    TTree* fTree = (TTree*)file->Get("demo/HBT");
    fTree->SetBranchAddress("HFsumET", &HFsumET);
    fTree->SetBranchAddress("Ntrk", &Ntrk);
    fTree->SetBranchAddress("trkPt", &trkPt);
    fTree->SetBranchAddress("trkEta", &trkEta);
    fTree->SetBranchAddress("trkPhi", &trkPhi);
    fTree->SetBranchAddress("trkPtRes", &trkPtRes);
    fTree->SetBranchAddress("trkDzSig", &trkDzSig);
    fTree->SetBranchAddress("trkDxySig", &trkDxySig);
    fTree->SetBranchAddress("trkNpixLayers", &trkNpixLayers);

    const unsigned int nEvents = fTree->GetEntries();

    // Defining auxiliar constants
    const unsigned int nBins = 97;
    vector<vector<float>> vec_f_pt;
    vector<float> vec_pt_A;
    vector<float> vec_pt_B;
    vector<float> vec_pt_AB;

    TH1F *hist_pt_A = new TH1F("pt_A", "pT from subset A", nBins, 0.3, 10); // Create to scale pT bins and get f(pT)

    for (Long64_t ievt=0; ievt<nEvents; ievt++){ // Loop over the events
        vector<float> f_pt(nBins, 0.0);
        float h_pt_A = 0;
        float h_pt_B = 0;
        int nTrk_A = 0;
        int nTrk_B = 0;

        fTree->GetEntry(ievt);
        if (ievt%10000 == 0)
            cout << "Processing event: " << ievt << endl;

        // 100 <= HFsumET <= 375 filter
        if (HFsumET < 100.0 || HFsumET > 375.0)
            continue;

        // Track loop
        for(int iTrk=0; iTrk<Ntrk; iTrk++){ // Loop over the tracks in a event
            if (trkEta[iTrk] >= -2.4 && trkEta[iTrk] <= -1.0 && !TMath::IsNaN(trkPt[iTrk])){ // Gets subset A [pT] (-2.4 <= pT <= -1.0)
                h_pt_A += trkPt[iTrk];
                nTrk_A += 1;
                hist_pt_A->Fill(trkPt[iTrk]);
            }
            if (trkEta[iTrk] >= 1.0 && trkEta[iTrk] <= 2.4 && !TMath::IsNaN(trkPt[iTrk])){ // Gets subset B [pT] (1.0 <= pT <= 2.4)
                h_pt_B += trkPt[iTrk];
                nTrk_B += 1;
            }
        }

        float pt_A = h_pt_A/nTrk_A;
        float pt_B = h_pt_B/nTrk_B;
        float pt_AB = pt_A*pt_B;

        // Scaling the created hist and taking the bins content to the array
        hist_pt_A->Scale(1/hist_pt_A->Integral()); // Defining a normalized histogram
        for (int i=0; i<nBins; i++){
            f_pt[i] = hist_pt_A->GetBinContent(i+1);
        }
        hist_pt_A->Reset();
        
        // Setting the desired vectors
        vec_f_pt.push_back(f_pt);
        vec_pt_A.push_back(pt_A);
        vec_pt_B.push_back(pt_B);
        vec_pt_AB.push_back(pt_AB);
    }

    // Calculating the std deviation
    float sigma2 = mean(vec_pt_AB) - (mean(vec_pt_A)*mean(vec_pt_B));
    float sigma = sqrt(sigma2);

    vector<float> vec_f_pt_mean;
    for (int i=0; i<vec_f_pt.size(); i++){
        vec_f_pt_mean.push_back(mean(vec_f_pt[i]));
    }

    vector<float> vec_f_pt_mean_times_pt_B;
    vec_f_pt_mean_times_pt_B = product(vec_f_pt_mean, vec_pt_B);

    float v0pt = (mean(vec_f_pt_mean_times_pt_B)-(mean(vec_f_pt_mean)*mean(vec_pt_B)))/(mean(vec_f_pt_mean)*sigma);
    cout << v0pt << endl;
}