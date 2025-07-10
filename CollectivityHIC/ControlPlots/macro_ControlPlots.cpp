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

void ControlPlots(std::string Filename = "/home/allanfgodoi/Desktop/IC-HIN-UFRGS/CollectivityHIC/Data/HiForestAOD_UPC.root"){

    auto filename = Filename;
    auto treename = "demo/HBT";

    ROOT::RDataFrame df(treename, filename); // RDataFrame object "df"

    const unsigned int nVar = 9;
    const unsigned int nBins = 100; // Fixing 100 bins for now
    TString hist_yaxis = "Entries";

    // Variables order: (0) HFSumET, (1) Ntrk, (2) trkPt, (3) trkEta, (4) trkPhi, (5) trkPtRes, (6) trkDzSig, (7) trkDxySig, (8) trkNpixLayers
    std::string list_vars_name[nVar] = {"HFsumET", "Ntrk", "trkPt", "trkEta", "trkPhi", "trkPtRes", "trkDzSig", "trkDxySig", "trkNpixLayers"};
    TString list_hist_name[nVar] = {"HFsumET", "Ntrk", "trkPt", "trkEta", "trkPhi", "trkPtRes", "trkDzSig", "trkDxySig", "trkNpixLayers"};
    TString list_hist_title[nVar] = {"HFsumET", "Ntrk", "trkPt", "trkEta", "trkPhi", "trkPtRes", "trkDzSig", "trkDxySig", "trkNpixLayers"};
    TString list_file_title[nVar] = {"HFsumET.png", "Ntrk.png", "trkPt.png", "trkEta.png", "trkPhi.png", "trkPtRes.png", "trkDzSig.png", "trkDxySig.png", "trkNpixLayers.png"};
    TString list_hist_xaxis[nVar] = {"HFsumET", "Ntrk", "trkPt", "trkEta", "trkPhi", "trkPtRes", "trkDzSig", "trkDxySig", "trkNpixLayers"};
    const float list_hist_xrange_min[nVar] = {0.0, 0.0, 0.0, -2.5, -3.3, 5.2e-3, -3.02, -3.02, 0.95};
    const float list_hist_xrange_max[nVar] = {380.0, 500.0, 10.0, 2.5, 3.3, 40.0e-3, 3.02, 3.02, 4.02}; // There is some cuts in max ranges

    for (unsigned int i=0; i<nVar; i++){
        auto h_var = df.Histo1D(TH1D(list_hist_name[i], list_hist_title[i], nBins, list_hist_xrange_min[i], list_hist_xrange_max[i]), list_vars_name[i]);
        h_var->GetXaxis()->SetTitle(list_hist_xaxis[i]);
        h_var->GetYaxis()->SetTitle(hist_yaxis);
        h_var->SetLineWidth(2);

        TCanvas c_var(list_hist_name[i], list_hist_name[i], 500, 500);
        h_var->Draw();
        c_var.Print(list_file_title[i]);
    }
}

void CorrectionPlots(float HFSET_Min, float HFSET_Max){

    TString Filename = "./../Data/HiForestAOD_UPC.root";
    TString cFilename = "./../Data/TrackCorrections_HIJING_538_OFFICIAL_Mar24.root";

    TFile *File(0);
    TFile *cFile(0);
    File = TFile::Open(Filename);
    cFile = TFile::Open(cFilename);

    const unsigned int nBins = 100;
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
    TTree* fTree = (TTree*)File->Get("demo/HBT");
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
    const unsigned int nEvents = fTree->GetEntries();

    TString pT_title = "pT";
    TString Eta_title = "Eta";
    if (HFSET_Max == 375.0){
        pT_title += " 50-60% Centrality";
        Eta_title += " 50-60% Centrality";
    }
    if (HFSET_Max == 210.0){
        pT_title += " 60-70% Centrality";
        Eta_title += " 60-70% Centrality";
    }

    TH1F *hist_noCorr_pT = new TH1F("noCorr_pT", pT_title, nBins, 0.0, 10.0);
    TH1F *hist_Corr_pT = new TH1F("Corr_pT", "", nBins, 0.0, 10.0);
    TH1F *hist_noCorr_Eta = new TH1F("noCorr_Eta", Eta_title, nBins, -2.5, 2.5);
    TH1F *hist_Corr_Eta = new TH1F("Corr_Eta", "", nBins, -2.5, 2.5);

    for (Long64_t ievt=0; ievt<nEvents; ievt++){
        fTree->GetEntry(ievt);
        if (ievt%10000 == 0) cout << "Processing event: " << ievt << endl;

        if (HFsumET < HFSET_Min || HFsumET > HFSET_Max) continue;
        if (pvZ < -15.0 || pvZ > 15.0) continue;

        for (int iTrk=0; iTrk<Ntrk; iTrk++){
            if (trkEta[iTrk] >= -2.4 && trkEta[iTrk] <= 2.4 && 
                trkPt[iTrk] >= 0.5 && trkPt[iTrk] <= 10.0 && 
                trkDzSig[iTrk] > -3.0 && trkDzSig[iTrk] < 3.0 && 
                trkDxySig[iTrk] > -3.0 && trkDxySig[iTrk] < 3.0 && 
                trkPtRes[iTrk] < 0.1){
                    float corrFac = getTrkCorrWeight(cFile, trkPt[iTrk], trkEta[iTrk]);
                    hist_noCorr_pT->Fill(trkPt[iTrk]);
                    hist_Corr_pT->Fill(trkPt[iTrk], corrFac);
                    hist_noCorr_Eta->Fill(trkEta[iTrk]);
                    hist_Corr_Eta->Fill(trkEta[iTrk], corrFac);
            }
        }
    }

    auto leg_pT = new TLegend(0.7, 0.90, 0.92, 0.82);
    leg_pT->AddEntry(hist_noCorr_pT, "Without correction", "l");
    leg_pT->AddEntry(hist_Corr_pT, "With correction", "l");
    leg_pT->SetBorderSize(0);
    leg_pT->SetFillStyle(0);

    auto leg_Eta = new TLegend(0.7, 0.90, 0.92, 0.82);
    leg_Eta->AddEntry(hist_noCorr_Eta, "Without correction", "l");
    leg_Eta->AddEntry(hist_Corr_Eta, "With correction", "l");
    leg_Eta->SetBorderSize(0);
    leg_Eta->SetFillStyle(0);

    auto c_pT = new TCanvas("c_pT", "", 500, 500);
    hist_noCorr_pT->SetLineColor(4);
    hist_Corr_pT->SetLineColor(2);
    hist_noCorr_pT->Draw();
    hist_Corr_pT->Draw("SAME");
    leg_pT->Draw();
    gPad->SetLogy();
    gStyle->SetOptStat(0);
    c_pT->Update();
    if (HFSET_Max == 375.0) c_pT->SaveAs("corr_pT_5060.pdf");
    if (HFSET_Max == 210.0) c_pT->SaveAs("corr_pT_6070.pdf");
    delete c_pT;

    auto c_Eta = new TCanvas("c_Eta", "", 500, 500);
    hist_noCorr_Eta->SetLineColor(4);
    hist_Corr_Eta->SetLineColor(2);
    hist_noCorr_Eta->Draw();
    hist_Corr_Eta->Draw("SAME");
    leg_Eta->Draw();
    gPad->SetLogy();
    gStyle->SetOptStat(0);
    c_Eta->Update();
    if (HFSET_Max == 375.0) c_Eta->SaveAs("corr_Eta_5060.pdf");
    if (HFSET_Max == 210.0) c_Eta->SaveAs("corr_Eta_6070.pdf");
    delete c_Eta;
}