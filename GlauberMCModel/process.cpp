#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTreeReader.h"
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
    th2->DrawClone("colz");
    canvas->SaveAs(saveAs.c_str());
    canvas->Clear();

}
void drawTH1 (TH1 *h, TCanvas *canvas, const string& xTitle, const string& yTitle, const string& saveAs){
    h->GetXaxis()->SetTitle(xTitle.c_str());
    h->GetYaxis()->SetTitle(yTitle.c_str());
    h->DrawClone("");
    canvas->SaveAs(saveAs.c_str());
    canvas->Clear();
}

void process (const char* dataFile = "./data.root", const string& location = "./graphs"){

    gSystem->mkdir(location.c_str());

    // Config the canvas
    auto *c = new TCanvas("canvas", "canvas", 1'200, 1'000);

    c->SetTicks();
    c->SetGrid();
    gStyle->SetPalette(kRainBow);
    gStyle->SetOptStat(0);

    // Open the ROOT file and get the "save" tree
    auto *file = new TFile("data.root");
    auto *tree = (TTree*) file->Get("save");

    // Create variables to hold the data
    Data data{};

    // Set up branches to read the data into the variables
    tree->SetBranchAddress("Collisions", &data);

    auto partXEvent = new TH1I("ppe", "nPart vs eventos", 375, 0, 416);
    auto colXEvent  = new TH1I("cpe", "nCol vs eventos", 500, 0, 2000);
    auto distXEvent = new TH1I("dpe", "d vs eventos", 54, 0, 18);
    auto distXPart  = new TH2I("dpp", "d vs nPart", 200,0,18,200,0, 416);
    auto distXCol   = new TH2I("dpc", "d vs nCol", 400,0,18,200,0,2000);
    auto colXPart   = new TH2I("cpp", "nCol vs nPart", 208,0,416,200,0,2000);

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
    drawTH1(distXEvent, c, "Distancia", "Eventos", location + "/dist.png");
    c->SetLogy();
    drawTH1(partXEvent, c, "Participantes", "Eventos", location + "/part.png");
    drawTH1(colXEvent, c, "Colisoes", "Eventos", location + "/col.png");
}
