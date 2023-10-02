void BDTSignificance(){

 // Open the ROOT file containing the TTree
    TFile *file = TFile::Open("/home/allanfgodoi/Desktop/TMVA_01.root");

    if (!file || file->IsZombie()) {
        cerr << "Error: Could not open the ROOT file." << endl;
        return;
    }

    // Create a TTreeReader for the "TestTree" inside the file
    TTreeReader reader("dataset_01/TestTree", file);
    TTree* myTree = (TTree*)file->Get("dataset_01/TrainTree");

    // Create a TTreeReaderValue for the "BDT" branch
    TTreeReaderValue<Float_t> reader_bdt(reader, "BDT");
    
    // Set addresses to the branches
    Float_t BDT, DGen;
    myTree->SetBranchAddress("BDT", &BDT);
    myTree->SetBranchAddress("DGen", &DGen);

    // Set a variable to get the entry value
    int entry = 0;

    // Set histograms
    TH1D *h_bdtSignal = new TH1D("BDT_Signal", "BDT_Signal", 100, -0.5, 0.2);
    TH1D *h_bdtBackground = new TH1D("BDT_Background", "BDT_Background", 100, -0.5, 0.2);

    // Loop at every entry, as it found some entry with BDT score > 0, add its value
    // at separed signal and background histograms
    while (reader.Next()) {
        Float_t bdtValue = *reader_bdt.Get();
        entry++;
        if (bdtValue > 0.0){
            myTree->GetEntry(entry);
            if (DGen==23333 || DGen==23344) h_bdtSignal->Fill(BDT);
            else h_bdtBackground->Fill(BDT);
        }
    }

    // Set and print a canvas containing the histograms, for checking purposes
    TCanvas myCanvas("BDT>0.0", "BDT>0.0", 2000, 1000);
    myCanvas.Divide(2, 1, 0.01, 0.01);
    myCanvas.cd(1); h_bdtSignal->Draw();
    myCanvas.cd(2); h_bdtBackground->Draw();
    myCanvas.Print("BDT>0.pdf");

    // Calculate and print the statistic significance
    float eff = h_bdtSignal->Integral()/(TMath::Sqrt(h_bdtSignal->Integral()+h_bdtBackground->Integral()));
    cout << "Statistic significance: " << eff << endl;

    // Close the file
    file->Close();
}