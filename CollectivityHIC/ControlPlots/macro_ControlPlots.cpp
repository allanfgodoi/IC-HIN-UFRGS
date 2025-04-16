// RUN COMMAND: root -l -b -q 'macro_ControlPlots.cpp'

void macro_ControlPlots(std::string Filename = "/home/allanfgodoi/Desktop/IC-HIN-UFRGS/CollectivityHIC/Data/HiForestAOD_UPC.root"){

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
    const float list_hist_xrange_max[nVar] = {380.0, 500.0, 6.5, 2.5, 3.3, 40.0e-3, 3.02, 3.02, 4.02}; // There is some cuts in max ranges

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