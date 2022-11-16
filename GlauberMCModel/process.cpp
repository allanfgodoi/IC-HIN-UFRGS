#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TStyle.h"
#include <random>
#include <iostream>

using namespace std;

void drawTH2 (TH2 *th2, TCanvas *canvas, const string& xTitle, const string& yTitle, const string& saveAs){
    th2->GetXaxis()->SetTitle(xTitle.c_str());
    th2->GetYaxis()->SetTitle(yTitle.c_str());
    th2->SetContour(10'000);
    th2->DrawClone("colz");
    canvas->SaveAs(saveAs.c_str());
    canvas->Clear();

}
void drawTH1 (TH1 *th1, TCanvas *canvas, const string& xTitle, const string& yTitle, const string& saveAs){
    th1->GetXaxis()->SetTitle(xTitle.c_str());
    th1->GetYaxis()->SetTitle(yTitle.c_str());
    th1->DrawClone("");
    canvas->SaveAs(saveAs.c_str());
    canvas->Clear();
}

void process (){

    // Config the canvas
    auto *c = new TCanvas("canvas", "canvas", 1'200, 1'000);

    c->SetTicks();
    c->SetGrid();
    gStyle->SetPalette(kRainBow);
    gStyle->SetOptStat(0);

    auto data = new TTree("data", "data");
    data->ReadFile("data.txt.bak", "part:col:dist");
    TTreeReader reader(data);
    TTreeReaderValue<float> part(reader, "part");
    TTreeReaderValue<float> col(reader, "col");
    TTreeReaderValue<float> dist(reader, "dist");

    auto partXEvent = new TH1F("ppe", "nPart vs eventos", 375, 0, ceil(data->GetMaximum("part")));
    auto colXEvent = new TH1F("cpe", "nCol vs eventos", 500, 0, ceil(data->GetMaximum("col")));
    auto distXEvent = new TH1F("dpe", "d vs eventos", 54, 0, ceil(data->GetMaximum("dist")));
    auto distXPart= new TH2F("dpp", "d vs nPart", 200,0,18,200,0,data->GetMaximum("part"));
    auto distXCol= new TH2F("dpc", "d vs nCol", 400,0,18,200,0,data->GetMaximum("col"));
    auto colXPart= new TH2F("cpp", "nCol vs nPart", 208,0,data->GetMaximum("part"),200,0,data->GetMaximum("col"));

    while (reader.Next()){
        partXEvent->Fill(*part);
        colXEvent->Fill(*col);
        distXEvent->Fill(*dist);
        distXPart->Fill(*dist, *part);
        distXCol->Fill(*dist, *col);
        colXPart->Fill(*part, *col);
    }

    // Draw TH2 functions
    c->SetLogz();
    drawTH2(distXPart, c, "Distancia", "Participantes", "./graphs/distpart.png");
    drawTH2(distXCol, c, "Distancia", "Colisoes", "./graphs/distcol.png");
    drawTH2(colXPart, c, "Colisoes", "Participantes", "./graphs/colpart.png");

    // Draw TH1 Functions
    drawTH1(distXEvent, c, "Distancia", "Eventos", "./graphs/dist.png");
    c->SetLogy();
    drawTH1(partXEvent, c, "Participantes", "Eventos", "./graphs/part.png");
    drawTH1(colXEvent, c, "Colisoes", "Eventos", "./graphs/col.png");
}