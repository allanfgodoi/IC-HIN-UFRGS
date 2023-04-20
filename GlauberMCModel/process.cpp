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
    th2->SetContour(10'000);
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
    auto *c = new TCanvas("canvas", "canvas", 1'000, 1'000);

    c->SetTicks();
    c->SetGrid();
    gStyle->SetPalette(kRainBow);
    gStyle->SetOptStat(0);

    // Open the ROOT file and get the "save" tree
    auto *file = new TFile(dataFile);
    auto *tree = (TTree*) file->Get("save");

    // Create variables to hold the data
    Data data{};

    // Set up branches to read the data into the variables
    tree->SetBranchAddress("Collisions", &data);

    auto partXEvent = new TH1I("ppe", "N_{part} vs eventos", 416, 0, 416);
    auto colXEvent  = new TH1I("cpe", "N_{coll} vs eventos", 2500, 0, 2500);
    auto distXEvent = new TH1I("dpe", "d vs eventos", 128, 0, 18);
    auto distXPart  = new TH2I("dpp", "d vs N_{part}", 200,0,18,416,0, 416);
    auto distXCol   = new TH2I("dpc", "d vs N_{coll}", 400,0,18,200,0,2000);
    auto colXPart   = new TH2I("cpp", "N_{coll} vs N_{part}", 416,0,416,200,0,2000);

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
    drawTH2(distXPart, c, "Distancia", "Participantes", location + "/distpart.png");
    drawTH2(distXCol, c, "Distancia", "Colisoes", location + "/distcol.png");
    drawTH2(colXPart, c, "Colisoes", "Participantes", location + "/colpart.png");

    // Draw TH1 Functions
    if (gPad) gPad->SetLeftMargin(0.14);
    drawTH1(distXEvent, c, "Distancia", "Eventos", location + "/dist.png");
    c->SetLogy();
    if (gPad) gPad->SetLeftMargin(0.13);
    drawTH1(partXEvent, c, "Participantes", "Eventos", location + "/part.png");
    if (gPad) gPad->SetLeftMargin(0.13);
    drawTH1(colXEvent, c, "Colisoes", "Eventos", location + "/col.png");
}
