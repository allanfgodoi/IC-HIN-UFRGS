// RUN COMMAND: root -l -b -q 'ObsConstructor.cpp'

#include <cmath>
#include "TMath.h"
using namespace std;

// Calculates the mean of a given vector
double mean(vector<double> x){

    int N = 0;
    double acc = 0.0;
    for (int i=0; i<x.size(); i++){
        if (!TMath::IsNaN(x[i])){
            acc += x[i];
            N += 1;
        }
    }
    double m = acc/N;
    return m;
}

vector<double> double_vector_mean(vector<vector<double>> x){

    const unsigned int nBins = 97;
    int nEvents_A = x.size();
    vector<double> vec;
    for (int i=0; i<nBins; i++){ // pT bins
        double acc_f_pt = 0;
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
    double_t trkPt[nDummy];
    double_t trkEta[nDummy];
    double_t trkPhi[nDummy];
    double_t trkPtRes[nDummy];
    double_t trkDzSig[nDummy];
    double_t trkDxySig[nDummy];
    double_t trkNpixLayers[nDummy];

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
    const unsigned int nEvents = fTree->GetEntries(); //MUST BACK TO ORIGINAL VALUE

    // Defining auxiliar constants
    const unsigned int nBins = 97; // This way I have 1 bin to a delta pT = 0.1
    vector<vector<double>> vec_f_pt; // This vector will hold the fractions of pT of all events
    vector<double> vec_pt_A;
    vector<double> vec_pt_B;
    vector<double> vec_pt_AB;

    TH1F *hist_pt_A = new TH1F("pt_A", "pT from subset A", nBins, 0.3, 10); // Create histogram to scale pT bins and get f(pT)
    for (Long64_t ievt=0; ievt<nEvents; ievt++){ // Loop over the events
        vector<double> f_pt(nBins, 0.0); // Define vector to hold the fractions of pT in the event
        double h_pt_A = 0;
        double h_pt_B = 0;
        int nTrk_A = 0;
        int nTrk_B = 0;

        fTree->GetEntry(ievt); // Here I get the events in the opened file
        if (ievt%10000 == 0)
            cout << "Processing event: " << ievt << endl;

        
        if (HFsumET < 100.0 || HFsumET > 375.0) // Applying 100 <= HFsumET <= 375 filter (related to centrality)
            continue;

        // Track loop
        for(int iTrk=0; iTrk<Ntrk; iTrk++){ // Loop over the tracks in a event
            if (trkEta[iTrk] >= -2.4 && trkEta[iTrk] <= -1.0 && !TMath::IsNaN(trkPt[iTrk])){ // Gets subset A [pT] (-2.4 <= pT <= -1.0)
                h_pt_A += trkPt[iTrk]; // Sum all pT from subset A in the desired range
                nTrk_A += 1; // Counts the number of pT entries from subset A
                hist_pt_A->Fill(trkPt[iTrk]); // Fills the auxiliar histogram to scale f(pT) later
            }
            if (trkEta[iTrk] >= 1.0 && trkEta[iTrk] <= 2.4 && !TMath::IsNaN(trkPt[iTrk])){ // Gets subset B [pT] (1.0 <= pT <= 2.4)
                h_pt_B += trkPt[iTrk]; // Sum all pT from subset B
                nTrk_B += 1; // Counts the number of pT entries from subset B
            }
        }

        if (nTrk_B == 0){ // I'm doing this to avoid division by zero (IDK IF IT'S NEEDED)
            nTrk_B = 1;
        }
        double pt_A = h_pt_A/nTrk_A; // Calculates the mean pT from subset A of each event
        double pt_B = h_pt_B/nTrk_B; // Calculates the mean pT from subset B of each event
        double pt_AB = pt_A*pt_B; // [pT]_A * [pT]_B

        // Scaling the created hist and taking the bins content to the array
        if (hist_pt_A->Integral() != 0){ // I'm doing this to avoid division by zero in empty fractions bins
        hist_pt_A->Scale(1/hist_pt_A->Integral()); // Defining a normalized histogram
        }
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
    double sigma2 = mean(vec_pt_AB) - (mean(vec_pt_A)*mean(vec_pt_B));
    double sigma = sqrt(sigma2);

    int nEvents_A = vec_f_pt.size();
    vector<vector<double>> vec_pt_B_f_pt(nEvents_A, vector<double>(nBins, 0.0));
    for (int j=0; j<nBins; j++){
        for (int i=0; i<nEvents_A; i++){
            vec_pt_B_f_pt[i][j] = vec_pt_B[i]*vec_f_pt[i][j];
        }
    }

    vector<double> vec_v0pt;
    vector<double> vec_mean_pt_B_f_pt = double_vector_mean(vec_pt_B_f_pt);
    vector<double> vec_mean_f_pt = double_vector_mean(vec_f_pt);

    for (int i=0; i<nBins; i++){
        double v0pt = ((vec_mean_pt_B_f_pt[i]-(vec_mean_f_pt[i]*vec_pt_B[i]))/(vec_mean_f_pt[i]*sigma));
        vec_v0pt.push_back(v0pt);
    }
}

//PROBLEMA EM sigma2 < 0