using namespace std;

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

    // Creates histos to store [pT]_A and [pT]_B
    TH1F *hist_pt_A = new TH1F("pt_A", "pT per particle A", nBins, 0.3, 10);
    TH1F *hist_pt_B = new TH1F("pt_B", "pT per particle B", nBins, 0.3, 10);

    const unsigned int nBins = 97;
    Float_t f_pt[nBins];

    // Events loop
    for (Long64_t ievt=0; ievt<fTree->GetEntries(); ievt++){ // Loop over the events

        fTree->GetEntry(ievt);

        // 100 <= eta <= 375 filter
        if (HFsumET < 100.0 || HFsumET > 375.0)
            continue;

        if (ievt%10000 == 0)
            cout << "Processing event: " << ievt << endl;

        for(int iTrk=0; iTrk<Ntrk; iTrk++){
            //for (int j=0; j<nBins; j++){
                //if (trkPt[iTrk] >= (0.3+0.1*j) && trkPt[iTrk] <= (0.4+0.1*j))
            //}
            if (trkEta[iTrk] >= -2.4 && trkEta[iTrk] <= -1.0) // Gets subset A [pT] (-2.4 <= pT <= -1.0)
                hist_pt_A->Fill(trkPt[iTrk]);
            if (trkEta[iTrk] >= 1.0 && trkEta[iTrk] <= 2.4) // Gets subset B [pT] (1.0 <= pT <= 2.4)
                hist_pt_B->Fill(trkPt[iTrk]);
        }
    }

    TCanvas *c1 = new TCanvas("c1", "Track p_{T}", 800, 600);
    hist_pt_A->Draw();
    c1->SaveAs("trkPt_hist.png");
}