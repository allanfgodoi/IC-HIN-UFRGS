// RUN COMMAND: root -l -b -q 'ObsConstructor.cpp'

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
    for (int i=2; i<nBins; i++){ // pT bins
        float acc_f_pt = 0;
        for (int j=0; j<nEvents_A; j++){ // Events
            acc_f_pt += x[j][i];
        }
        vec.push_back(acc_f_pt/nEvents_A);
    }
    return vec;
}

void ObsConstructor(){

    // Open file
    TFile *file(0);
    TString filename = "/home/allanfgodoi/Desktop/IC-HIN-UFRGS/CollectivityHIC/Data/HiForestAOD_UPC.root";
    if (!gSystem->AccessPathName(filename)) // Verify if the file really exists
        file = TFile::Open(filename);
    if (!file) {
        cout << "ERROR: couldn't open datafile!";
        exit(1);
    }

    const unsigned int nDummy = 10000; // Used to create a dummy len array to hold the tracks

    // Declare arrays and scalars to hold the TTree files
    float_t HFsumET = 0;
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
    fTree->SetBranchAddress("Ntrk", &Ntrk);
    fTree->SetBranchAddress("trkPt", &trkPt);
    fTree->SetBranchAddress("trkEta", &trkEta);
    fTree->SetBranchAddress("trkPhi", &trkPhi);
    fTree->SetBranchAddress("trkPtRes", &trkPtRes);
    fTree->SetBranchAddress("trkDzSig", &trkDzSig);
    fTree->SetBranchAddress("trkDxySig", &trkDxySig);
    fTree->SetBranchAddress("trkNpixLayers", &trkNpixLayers);

    //const unsigned int nEvents = 10000;
    const unsigned int nEvents = fTree->GetEntries(); // MUST BACK TO THIS ORIGINAL VALUE

    // Defining auxiliar constants
    const unsigned int nBins = 97; // This way I have 1 bin to a delta pT = 0.1
    vector<vector<float>> vec_f_pt; // This vector will hold the fractions of pT of all events
    vector<float> vec_pt_A;
    vector<float> vec_pt_B;
    vector<float> vec_pt_AB;

    TH1F *hist_pt_A = new TH1F("pt_A", "pT from subset A", nBins, 0.3, 10); // Create histogram to scale pT bins and get f(pT)
    for (Long64_t ievt=0; ievt<nEvents; ievt++){ // Loop over the events
        vector<float> f_pt(nBins, 0.0); // Define vector to hold the fractions of pT in the event
        float h_pt_A = 0;
        float h_pt_B = 0;
        int nTrk_A = 0;
        int nTrk_B = 0;

        fTree->GetEntry(ievt); // Here I get the events in the opened file
        if (ievt%10000 == 0)
            cout << "Processing event: " << ievt << endl;

        
        if (HFsumET < 100.0 || HFsumET > 375.0) // Applying 100 <= HFsumET <= 375 filter (related to centrality)
            continue;

        // Track loop
        for(int iTrk=0; iTrk<Ntrk; iTrk++){ // Loop over the tracks in a event
            if (trkEta[iTrk] >= -2.4 && trkEta[iTrk] <= -1.0 && !TMath::IsNaN(trkPt[iTrk]) && trkPt[iTrk] >= 0.3){ // Gets subset A [pT] (-2.4 <= Eta <= -1.0)
                h_pt_A += trkPt[iTrk]; // Sum all pT from subset A in the desired range
                nTrk_A += 1; // Counts the number of pT entries from subset A
                hist_pt_A->Fill(trkPt[iTrk]); // Fills the auxiliar histogram to scale f(pT) later
                if (trkPt[iTrk] <= 0.1){
                    cout << trkPt[iTrk] << endl;
                }
            }
            if (trkEta[iTrk] >= 1.0 && trkEta[iTrk] <= 2.4 && !TMath::IsNaN(trkPt[iTrk]) && trkPt[iTrk] >= 0.3){ // Gets subset B [pT] (1.0 <= Eta <= 2.4)
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
        vec_f_pt.push_back(f_pt);
        vec_pt_A.push_back(pt_A);
        vec_pt_B.push_back(pt_B);
        vec_pt_AB.push_back(pt_AB);
    }

    // Calculating the std deviation
    float sigma2 = (mean(vec_pt_AB) - (mean(vec_pt_A)*mean(vec_pt_B)));
    float sigma = sqrt(sigma2);

    int nEvents_A = vec_f_pt.size();
    vector<vector<float>> vec_pt_B_f_pt(nEvents_A, vector<float>(nBins, 0.0));
    for (int j=0; j<nBins; j++){
        for (int i=0; i<nEvents_A; i++){
            vec_pt_B_f_pt[i][j] = vec_pt_B[i]*vec_f_pt[i][j];
        }
    }

    vector<float> vec_v0pt;
    vector<float> vec_mean_pt_B_f_pt = double_vector_mean(vec_pt_B_f_pt, nBins);
    vector<float> vec_mean_f_pt = double_vector_mean(vec_f_pt, nBins);

    for (int i=2; i<nBins; i++){
        float v0pt = ((vec_mean_pt_B_f_pt[i]-(vec_mean_f_pt[i]*vec_pt_B[i]))/(vec_mean_f_pt[i]*sigma));
        if (TMath::IsNaN(v0pt)) v0pt = 0.0;
        vec_v0pt.push_back(v0pt);
    }

    float sum1 = 0;
    for (int i=0; i<nBins; i++){
        sum1 += vec_v0pt[i]*vec_mean_f_pt[i];
    }

    cout << sum1 << endl;
}