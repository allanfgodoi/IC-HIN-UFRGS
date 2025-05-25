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
    for (int i=0; i<nBins; i++){ // pT bins
        float acc_f_pt = 0;
        for (int j=0; j<nEvents_A; j++){ // Events
            acc_f_pt += x[j][i];
        }
        vec.push_back(acc_f_pt/nEvents_A);
    }
    return vec;
}

// TGraph creator function
TGraph* create_TGraph(int nPoints, const float* x, const float* y, const char* title, float xmin, int xmax, float ymin, float ymax, int style, int color){
    TGraph* g = new TGraph(nPoints, x, y);
    g->SetTitle(title);
    g->GetXaxis()->SetLimits(xmin, xmax); // X axis range
    g->GetXaxis()->CenterTitle(); // Center axis label
    g->GetYaxis()->CenterTitle(); // Center axis label
    g->SetMinimum(ymin); // Y axis range
    g->SetMaximum(ymax); // Y axis range
    g->SetMarkerStyle(style);
    g->SetMarkerColor(color);
    g->SetMarkerSize(1.2);
    return g;
}

void ObsConstructor(){
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
    const unsigned int nBins = 97; // This way I have 1 bin to a delta pT = 0.1 in the range [0.3-10.0]
    //const unsigned int nBins = 29;
    int Eta_gap = 2;
    vector<vector<float>> vec_f_pt; // This vector will hold the fractions of pT of all events
    vector<float> vec_pt_A;
    vector<float> vec_pt_B;
    vector<float> vec_pt_AB;

    TH1F *hist_all_pt_A = new TH1F("all_pt_A", "All pT from subset A", nBins, 0.3, 10); // Create histogram to calculate <pT_A> (all events)
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
            if (trkEta[iTrk] >= -2.4 && trkEta[iTrk] <= -Eta_gap/2 && !TMath::IsNaN(trkPt[iTrk]) && trkPt[iTrk] >= 0.3){ // Gets subset A [pT]
                h_pt_A += trkPt[iTrk]; // Sum all pT from subset A in the desired range
                nTrk_A += 1; // Counts the number of pT entries from subset A
                hist_pt_A->Fill(trkPt[iTrk]); // Fills the auxiliar histogram to scale f(pT) later
                hist_all_pt_A->Fill(trkPt[iTrk]);
            }
            if (trkEta[iTrk] >= Eta_gap/2 && trkEta[iTrk] <= 2.4 && !TMath::IsNaN(trkPt[iTrk]) && trkPt[iTrk] >= 0.3){ // Gets subset B [pT]
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
        float pT = (i*0.1-0.05+0.4);
        sum2_v0pt_left += pT*vec_v0pt[i]*vec_mean_f_pt[i];
        acc_sum2_v0pt_right += vec_mean_f_pt[i];
    }
    float sum2_v0pt_right = sigma*acc_sum2_v0pt_right;

    // Printing v0(pT) sum rules
    cout << "------------------ v0(pT) ------------------" << endl;
    cout << "Sum rule #1: " << sum1_v0pt << " = 0" << endl;
    cout << "Sum rule #2: " << sum2_v0pt_left << " = " << sum2_v0pt_right << endl << endl;


    // Calculating v0(pT)/v0
    float mean_pt = hist_all_pt_A->GetMean(); // Gets <pT_A> from previously created hist
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
        float pT = (i*0.1-0.05+0.4);
        sum2_v0ptv0_left += pT*vec_v0ptv0[i]*vec_mean_f_pt[i];
        acc_sum2_v0ptv0_right += vec_mean_f_pt[i];
    }
    float sum2_v0ptv0_right = mean_pt*acc_sum2_v0ptv0_right;

    cout << "----------------- v0(pT)/v0 -----------------" << endl;
    cout << "Sum rule #1: " << sum1_v0ptv0 << " = 0" << endl;
    cout << "Sum rule #2: " << sum2_v0ptv0_left << " = " << sum2_v0ptv0_right << endl;

    // Defining arrays for plots
    float arr_pT[nBins];
    float arr_v0pt[nBins];
    float arr_v0ptv0[nBins];
    for (int i=0; i<nBins; i++){
        arr_pT[i] = (i*0.1-0.05+0.4);
        arr_v0pt[i] = vec_v0pt[i];
        arr_v0ptv0[i] = vec_v0ptv0[i];
    }

    // Opening and reading file containing ATLAS results
    // "purple": 50-60% centrality
    // "pink": 60-70% centrality
    const int nPoints_ATLAS = 29;
    float x_v0pt_pink[nPoints_ATLAS], y_v0pt_pink[nPoints_ATLAS];
    float x_v0pt_purple[nPoints_ATLAS], y_v0pt_purple[nPoints_ATLAS];
    float x_v0ptv0_pink[nPoints_ATLAS], y_v0ptv0_pink[nPoints_ATLAS];
    float x_v0ptv0_purple[nPoints_ATLAS], y_v0ptv0_purple[nPoints_ATLAS];
    ifstream v0pt_pink("/home/allanfgodoi/Desktop/IC-HIN-UFRGS/CollectivityHIC/Data/ATLAS_v0pt_pink.txt");
    ifstream v0pt_purple("/home/allanfgodoi/Desktop/IC-HIN-UFRGS/CollectivityHIC/Data/ATLAS_v0pt_purple.txt");
    ifstream v0ptv0_pink("/home/allanfgodoi/Desktop/IC-HIN-UFRGS/CollectivityHIC/Data/ATLAS_v0ptv0_pink.txt");
    ifstream v0ptv0_purple("/home/allanfgodoi/Desktop/IC-HIN-UFRGS/CollectivityHIC/Data/ATLAS_v0ptv0_purple.txt");
    for (int i=0; i<nPoints_ATLAS; i++){
        v0pt_pink >> x_v0pt_pink[i] >> y_v0pt_pink[i];
        v0pt_purple >> x_v0pt_purple[i] >> y_v0pt_purple[i];
        v0ptv0_pink >> x_v0ptv0_pink[i] >> y_v0ptv0_pink[i];
        v0ptv0_purple >> x_v0ptv0_purple[i] >> y_v0ptv0_purple[i];
    }

    // Creates canvas and TGraphs
    auto c_v0pt = new TCanvas("c_v0pt", "Analysis plot", 1000, 500);
    c_v0pt->Divide(2, 1);
    TGraph* gr_v0pt = create_TGraph(nBins, arr_pT, arr_v0pt, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 20, 8);
    TGraph* gr_v0pt_pink = create_TGraph(nPoints_ATLAS, x_v0pt_pink, y_v0pt_pink, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 47, 6);
    TGraph* gr_v0pt_purple = create_TGraph(nPoints_ATLAS, x_v0pt_purple, y_v0pt_purple, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.41, 34, 9);
    TGraph* gr_v0ptv0 = create_TGraph(nBins, arr_pT, arr_v0ptv0, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 20, 8);
    TGraph* gr_v0ptv0_pink = create_TGraph(nPoints_ATLAS, x_v0ptv0_pink, y_v0ptv0_pink, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -4.0, 28.0, 47, 6);
    TGraph* gr_v0ptv0_purple = create_TGraph(nPoints_ATLAS, x_v0ptv0_purple, y_v0ptv0_purple, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -4.0, 28.0, 34, 9);

    // Setting up the legends
    auto legend_v0pt = new TLegend(0.10, 0.75, 0.50, 0.90);
    auto legend_v0ptv0 = new TLegend(0.10, 0.75, 0.50, 0.90);
    legend_v0pt->SetTextSize(0.029);
    legend_v0pt->AddEntry(gr_v0pt, "CMS OpenData 2.76 TeV (50-70%)", "p");
    legend_v0pt->AddEntry(gr_v0pt_purple, "ATLAS 5.02 TeV (50-60%)", "p");
    legend_v0pt->AddEntry(gr_v0pt_pink, "ATLAS 5.02 TeV (60-70%)", "p");
    legend_v0pt->SetBorderSize(0);
    legend_v0pt->SetFillStyle(0);
    legend_v0ptv0->SetTextSize(0.029);
    legend_v0ptv0->AddEntry(gr_v0pt, "CMS OpenData 2.76 TeV (50-70%)", "p");
    legend_v0ptv0->AddEntry(gr_v0pt_purple, "ATLAS 5.02 TeV (50-60%)", "p");
    legend_v0ptv0->AddEntry(gr_v0pt_pink, "ATLAS 5.02 TeV (60-70%)", "p");
    legend_v0ptv0->SetBorderSize(0);
    legend_v0ptv0->SetFillStyle(0);

    // Drawing v0(pT) plot
    c_v0pt->cd(1);
    gr_v0pt->Draw("AP");
    gr_v0pt_pink->Draw("P SAME");
    gr_v0pt_purple->Draw("P SAME");
    legend_v0pt->Draw();
    gPad->SetLogx();

    // Drawing v0(pT)/v0 plot
    c_v0pt->cd(2);
    gr_v0ptv0->Draw("AP");
    gr_v0ptv0_pink->Draw("P SAME");
    gr_v0ptv0_purple->Draw("P SAME");
    legend_v0ptv0->Draw();
    gPad->SetLogx();

    // Saving canvas as pdf
    c_v0pt->Update();
    c_v0pt->SaveAs("plots.pdf");
}