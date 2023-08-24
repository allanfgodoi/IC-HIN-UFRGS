#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include <random>
#include <TSystem.h>
#include <TTreeReaderArray.h>

using namespace std;

struct Data{int nCol, nPart; double dist;};

void drawTH2 (TH2 *th2, TCanvas *canvas, const string& xTitle, const string& yTitle, const string& saveAs){
    th2->GetXaxis()->SetTitle(xTitle.c_str());
    th2->GetYaxis()->SetTitle(yTitle.c_str());
    th2->SetContour(10000);
    if (gPad) gPad->SetRightMargin(0.12);
    if (gPad) gPad->SetLeftMargin(0.13);
    if (gPad) gPad->SetBottomMargin(0.12);
    th2->DrawClone("colz");
    canvas->SaveAs(saveAs.c_str());
    canvas->Clear();

}
void drawTH1 (TH1 *h, TCanvas *canvas, const string& xTitle, const string& yTitle, const string& saveAs){
    h->GetXaxis()->SetTitle(xTitle.c_str());
    h->GetYaxis()->SetTitle(yTitle.c_str());
    if (gPad) gPad->SetRightMargin(0.05);
    if (gPad) gPad->SetBottomMargin(0.12);
    h->DrawClone("");
    canvas->SaveAs(saveAs.c_str());
    canvas->Clear();
}

// Main code
void process (const char* dataFile = "./data.root", const string& location = "./graphs"){

    gSystem->mkdir(location.c_str());

    // Config the canvas
    auto *c = new TCanvas("canvas", "canvas", 1000, 1000);

    c->SetTicks();
    c->SetGrid();

    // ColorBlind Friendly palette
    gStyle->SetPalette(kRainBow);
    gStyle->SetOptStat(0);

    // Open the ROOT file containing generated data and get the “save” tree
    auto *file = new TFile(dataFile);
    auto *tree = (TTree*) file->Get("Data");

    // Create variables to hold the data
    Data data{};

    // Set up branches to read the data into the variables
    tree->SetBranchAddress("Collisions", &data);

    auto partXEvent = new TH1I("ppe", "N_{part} vs events", 416, 0, 416);
    auto colXEvent  = new TH1I("cpe", "N_{coll} vs events", 4000, 0, 4000);
    auto distXEvent = new TH1I("dpe", "b vs events", 128, 0, 18);
    auto distXPart  = new TH2I("dpp", "b vs N_{part}", 200,0,18,416,0, 416);
    auto distXCol   = new TH2I("dpc", "b vs N_{coll}", 400,0,18,4000,0,4000);
    auto colXPart   = new TH2I("cpp", "N_{coll} vs N_{part}", 416,0,416,4000,0,4000);

    for (Long64_t i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);
        partXEvent -> Fill(data.nPart);
        colXEvent  -> Fill(data.nCol);
        distXEvent -> Fill(data.dist);
        distXPart  -> Fill(data.dist, data.nPart);
        distXCol   -> Fill(data.dist, data.nCol);
        colXPart   -> Fill(data.nPart, data.nCol);
    }

    // Draw TH2 functions
    c->SetLogz();
    drawTH2(distXPart, c, "Impact parameter (fm)", "Participants", location + "/DistPart.pdf");
    drawTH2(distXCol, c, "Impact parameter (fm)", "Collisions", location + "/DistCol.pdf");
    drawTH2(colXPart, c, "Participants", "Collisions", location + "/ColPart.pdf");

    // Draw TH1 Functions
    if (gPad) gPad->SetLeftMargin(0.14);
    drawTH1(distXEvent, c, "Impact parameter (fm)", "Events", location + "/DistEvent.pdf");
    c->SetLogy();
    if (gPad) gPad->SetLeftMargin(0.13);
    drawTH1(partXEvent, c, "Participants", "Events", location + "/PartEvent.pdf");
    if (gPad) gPad->SetLeftMargin(0.13);
    drawTH1(colXEvent, c, "Collisions", "Events", location + "/ColEvent.pdf");
}
