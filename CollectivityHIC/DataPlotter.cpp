#include "/home/allanfgodoi/cnpy/cnpy.h"
#include <iostream>
#include <type_traits>

// TGraph creator function
TGraph* create_TGraph(int nPoints, const float* x, const float* y, const char* title, float xmin, int xmax, float ymin, float ymax, int style, int color, float size){
    TGraph* g = new TGraph(nPoints, x, y);
    g->SetTitle(title);
    g->GetXaxis()->SetLimits(xmin, xmax); // X axis range
    g->GetXaxis()->CenterTitle(); // Center axis label
    g->GetYaxis()->CenterTitle(); // Center axis label
    g->SetMinimum(ymin); // Y axis range
    g->SetMaximum(ymax); // Y axis range
    g->SetMarkerStyle(style);
    g->SetMarkerColor(color);
    g->SetMarkerSize(size);
    return g;
}

TGraph* create_TGraphFromTxt(string filePath, int nPoints, const char* title, float xmin, float xmax, float ymin, float ymax, int style, int color, float size){
    float* x = new float[nPoints];
    float* y = new float[nPoints];
    ifstream f(filePath);
    for (int i=0; i<nPoints; i++){
        f >> x[i] >> y[i];
    }
    TGraph* g = create_TGraph(nPoints, x, y, title, xmin, xmax, ymin, ymax, style, color, size);
    delete[] x; delete[] y;
    return g;
}

void customize_TGraph(TGraph *g, const char* title, float xmin, float xmax, float ymin, float ymax, int style, int color, float size){
    g->SetTitle(title);
    g->GetXaxis()->SetLimits(xmin, xmax); // X axis range
    g->GetXaxis()->CenterTitle(); // Center axis label
    g->GetYaxis()->CenterTitle(); // Center axis label
    g->SetMinimum(ymin); // Y axis range
    g->SetMaximum(ymax); // Y axis range
    g->SetMarkerStyle(style);
    g->SetMarkerColor(color);
    g->SetLineColor(color);
    g->SetMarkerSize(size);
}

void customize_TGraphAsymmErrors(TGraphAsymmErrors *g, const char* title, float xmin, float xmax, float ymin, float ymax, int style, int color, float size){
    g->SetTitle(title);
    g->GetXaxis()->SetLimits(xmin, xmax); // X axis range
    g->GetXaxis()->CenterTitle(); // Center axis label
    g->GetYaxis()->CenterTitle(); // Center axis label
    g->SetMinimum(ymin); // Y axis range
    g->SetMaximum(ymax); // Y axis range
    g->SetMarkerStyle(style);
    g->SetMarkerColor(color);
    g->SetLineColor(color);
    g->SetMarkerSize(size);
}

void customize_TGraphErrors(TGraphErrors *g, const char* title, float xmin, float xmax, float ymin, float ymax, int style, int color, float size){
    g->SetTitle(title);
    g->GetXaxis()->SetLimits(xmin, xmax); // X axis range
    g->GetXaxis()->CenterTitle(); // Center axis label
    g->GetYaxis()->CenterTitle(); // Center axis label
    g->SetMinimum(ymin); // Y axis range
    g->SetMaximum(ymax); // Y axis range
    g->SetMarkerStyle(style);
    g->SetMarkerColor(color);
    g->SetLineColor(color);
    g->SetMarkerSize(size);
}

void DoPlotRefsEta(){
    auto c = new TCanvas("c", "c_refs_eta", 1100, 1000);
    c->Divide(2, 2, 0.00000001, 0.00000001);

    // v0 | pT-refs

    TFile *f_v0 = TFile::Open("./Data/Figures/v0.root", "READ");
    TGraphErrors *gr_v0_1 = (TGraphErrors*)f_v0->Get("55_1");
    TGraphErrors *gr_v0_2 = (TGraphErrors*)f_v0->Get("55_2");
    TGraphErrors *gr_v0_3 = (TGraphErrors*)f_v0->Get("55_3");
    TGraphErrors *gr_v0_4 = (TGraphErrors*)f_v0->Get("65_1");
    TGraphErrors *gr_v0_5 = (TGraphErrors*)f_v0->Get("65_2");
    TGraphErrors *gr_v0_6 = (TGraphErrors*)f_v0->Get("65_3");

    TFile *f_v0_ATLAS = TFile::Open("./Data/Figures_ATLAS/v0_ATLAS.root");
    TGraphErrors *gr_v0_ATLAS_1 = (TGraphErrors*)f_v0_ATLAS->Get("Figure 2a/Graph1D_y1");
    TGraphErrors *gr_v0_ATLAS_2 = (TGraphErrors*)f_v0_ATLAS->Get("Figure 2a/Graph1D_y2");
    TGraphErrors *gr_v0_ATLAS_3 = (TGraphErrors*)f_v0_ATLAS->Get("Figure 2a/Graph1D_y3");

    customize_TGraphErrors(gr_v0_1, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 24, 2, 1.2);
    customize_TGraphErrors(gr_v0_2, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 25, 4, 1.2);
    customize_TGraphErrors(gr_v0_3, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 26, 6, 1.2);
    customize_TGraphErrors(gr_v0_4, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 24, 2, 1.2);
    customize_TGraphErrors(gr_v0_5, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 25, 4, 1.2);
    customize_TGraphErrors(gr_v0_6, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 26, 6, 1.2);
    customize_TGraphErrors(gr_v0_ATLAS_1, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 20, 2, 1.0);
    customize_TGraphErrors(gr_v0_ATLAS_2, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 21, 4, 1.0);
    customize_TGraphErrors(gr_v0_ATLAS_3, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 22, 6, 1.0);

    auto leg_v0_title = new TLegend(0.02, 1.1, 0.5, 0.77);
    leg_v0_title->SetTextSize(0.06);
    leg_v0_title->AddEntry((TObject*)0, "Pb+Pb,    #eta_{gap} = 1", "");
    leg_v0_title->SetBorderSize(0);
    leg_v0_title->SetFillStyle(0);

    auto leg_v0_ptref = new TLegend(0.35, 0.41, 0.7, 0.13);
    leg_v0_ptref->SetTextSize(0.030);
    leg_v0_ptref->AddEntry(gr_v0_1, "CMS Open Data 2.76 TeV: 0.5-2 GeV", "p");
    leg_v0_ptref->AddEntry(gr_v0_2, "CMS Open Data 2.76 TeV: 0.5-5 GeV", "p");
    leg_v0_ptref->AddEntry(gr_v0_3, "CMS Open Data 2.76 TeV: 1-5 GeV", "p");
    leg_v0_ptref->AddEntry(gr_v0_ATLAS_1, "ATLAS 5.02 TeV: 0.5-2 GeV", "p");
    leg_v0_ptref->AddEntry(gr_v0_ATLAS_2, "ATLAS 5.02 TeV: 0.5-5 GeV", "p");
    leg_v0_ptref->AddEntry(gr_v0_ATLAS_3, "ATLAS 5.02 TeV: 1-5 GeV", "p");
    leg_v0_ptref->SetBorderSize(0);
    leg_v0_ptref->SetFillStyle(0);

    auto leg_v0_label = new TLegend(0.78, 0.9, 0.93, 0.93);
    leg_v0_label->SetTextSize(0.055);
    leg_v0_label->AddEntry((TObject*)0, "(a)", "");
    leg_v0_label->SetBorderSize(0);
    leg_v0_label->SetFillStyle(0);

    c->cd(1);
    gr_v0_1->Draw("AP");
    gr_v0_2->Draw("P SAME");
    gr_v0_3->Draw("P SAME");
    gr_v0_4->Draw("P SAME");
    gr_v0_5->Draw("P SAME");
    gr_v0_6->Draw("P SAME");
    gr_v0_ATLAS_1->Draw("P SAME");
    gr_v0_ATLAS_2->Draw("P SAME");
    gr_v0_ATLAS_3->Draw("P SAME");

    leg_v0_title->Draw();
    leg_v0_ptref->Draw();
    leg_v0_label->Draw();
    gPad->SetLogy();
    gPad->SetLeftMargin(0.12);
    gPad->SetTopMargin(0.01);

    // READING TGRAPHS FILE FROM OUR DATA AND GETTING THEM
    TFile *f_main = TFile::Open("./Data/Figures/main.root");

    // v0(pT)v0 | pT-refs

    // pT-ref: [0.5, 2.0]
    TGraphErrors *gr_v0ptv0_ptref_1 = (TGraphErrors*)f_main->Get("v0ptv0_ptref_1_5060");
    // pT-ref: [0.5, 5.0]
    TGraphErrors *gr_v0ptv0_ptref_2 = (TGraphErrors*)f_main->Get("v0ptv0_ptref_2_5060");
    // pT-ref: [1.0, 5.0]
    TGraphErrors *gr_v0ptv0_ptref_3 = (TGraphErrors*)f_main->Get("v0ptv0_ptref_3_5060");
    // EDITING OUR DATA TGRAPH
    customize_TGraphErrors(gr_v0ptv0_ptref_1, " ; p_{T} [GeV]; v_{0}(p_{T})v_{0}x10^{3}", 0.0, 10.0, -2.0, 9.0, 24, 2, 1.0);
    customize_TGraphErrors(gr_v0ptv0_ptref_2, " ; p_{T} [GeV]; v_{0}(p_{T})v_{0}x10^{3}", 0.0, 10.0, -2.0, 9.0, 25, 4, 1.0);
    customize_TGraphErrors(gr_v0ptv0_ptref_3, " ; p_{T} [GeV]; v_{0}(p_{T})v_{0}x10^{3}", 0.0, 10.0, -2.0, 9.0, 26, 6, 1.0);

    // READING TGRAPHS FILE FROM ATLAS DATA AND GETTING AND EDITING THEM
    TFile *f_v0ptv0_ptref_ATLAS = TFile::Open("./Data/Figures_ATLAS/v0ptv0_ptref_ATLAS.root");
    // pT-ref: [0.5, 2.0]
    TGraphAsymmErrors *gr_v0ptv0_ptref_ATLAS_1 = (TGraphAsymmErrors*)f_v0ptv0_ptref_ATLAS->Get("Figure 2c_lower panel/Graph1D_y1");
    // pT-ref: [0.5, 5.0]
    TGraphAsymmErrors *gr_v0ptv0_ptref_ATLAS_2 = (TGraphAsymmErrors*)f_v0ptv0_ptref_ATLAS->Get("Figure 2c_lower panel/Graph1D_y2");
    // pT-ref: [1.0, 5.0]
    TGraphAsymmErrors *gr_v0ptv0_ptref_ATLAS_3 = (TGraphAsymmErrors*)f_v0ptv0_ptref_ATLAS->Get("Figure 2c_lower panel/Graph1D_y3");
    // EDITING ATLAS DATA TGRAPH
    customize_TGraphAsymmErrors(gr_v0ptv0_ptref_ATLAS_1, " ; p_{T} [GeV]; v_{0}(p_{T})v_{0}x10^{3}", 0.0, 10.0, -5.0, 9.0, 20, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0ptv0_ptref_ATLAS_2, " ; p_{T} [GeV]; v_{0}(p_{T})v_{0}x10^{3}", 0.0, 10.0, -5.0, 9.0, 21, 4, 1.0);
    customize_TGraphAsymmErrors(gr_v0ptv0_ptref_ATLAS_3, " ; p_{T} [GeV]; v_{0}(p_{T})v_{0}x10^{3}", 0.0, 10.0, -5.0, 9.0, 22, 6, 1.0);

    // CONFIGURING TLEGENDS
    auto leg_v0ptv0_ptref_title = new TLegend(0.025, 0.98, 0.5, 0.825);
    leg_v0ptv0_ptref_title->SetTextSize(0.06);
    leg_v0ptv0_ptref_title->AddEntry((TObject*)0, "Pb+Pb,   #eta_{gap} = 1", "");
    leg_v0ptv0_ptref_title->AddEntry((TObject*)0, "50-60% Centrality", "");
    leg_v0ptv0_ptref_title->SetBorderSize(0);
    leg_v0ptv0_ptref_title->SetFillStyle(0);
    auto leg_v0ptv0_ptref = new TLegend(0.103, 0.82, 0.5, 0.68);
    leg_v0ptv0_ptref->SetTextSize(0.030);
    leg_v0ptv0_ptref->AddEntry(gr_v0ptv0_ptref_1, "CMS OpenData 2.76 TeV: 0.5-2 GeV", "p");
    leg_v0ptv0_ptref->AddEntry(gr_v0ptv0_ptref_2, "CMS OpenData 2.76 TeV: 0.5-5 GeV", "p");
    leg_v0ptv0_ptref->AddEntry(gr_v0ptv0_ptref_3, "CMS OpenData 2.76 TeV: 1-5 GeV", "p");
    leg_v0ptv0_ptref->SetBorderSize(0);
    leg_v0ptv0_ptref->SetFillStyle(0);

    auto leg_v0ptv0_ptref2 = new TLegend(0.4, 0.25, 0.9, 0.12);
    leg_v0ptv0_ptref2->SetTextSize(0.030);
    leg_v0ptv0_ptref2->AddEntry(gr_v0ptv0_ptref_ATLAS_1, "ATLAS 5.02 TeV: 0.5-2 GeV", "p");
    leg_v0ptv0_ptref2->AddEntry(gr_v0ptv0_ptref_ATLAS_2, "ATLAS 5.02 TeV: 0.5-5 GeV", "p");
    leg_v0ptv0_ptref2->AddEntry(gr_v0ptv0_ptref_ATLAS_3, "ATLAS 5.02 TeV: 1-5 GeV", "p");
    leg_v0ptv0_ptref2->SetBorderSize(0);
    leg_v0ptv0_ptref2->SetFillStyle(0);

    auto leg_v0ptv0_label = new TLegend(0.78, 0.9, 0.93, 0.93);
    leg_v0ptv0_label->SetTextSize(0.055);
    leg_v0ptv0_label->AddEntry((TObject*)0, "(b)", "");
    leg_v0ptv0_label->SetBorderSize(0);
    leg_v0ptv0_label->SetFillStyle(0);

    // PLOTING IN IT CD
    c->cd(2);
    gr_v0ptv0_ptref_1->Draw("AP");
    gr_v0ptv0_ptref_2->Draw("P SAME");
    gr_v0ptv0_ptref_3->Draw("P SAME");
    gr_v0ptv0_ptref_ATLAS_1->Draw("P SAME");
    gr_v0ptv0_ptref_ATLAS_2->Draw("P SAME");
    gr_v0ptv0_ptref_ATLAS_3->Draw("P SAME");
    leg_v0ptv0_ptref_title->Draw();
    leg_v0ptv0_ptref->Draw();
    leg_v0ptv0_ptref2->Draw();
    leg_v0ptv0_label->Draw();
    gPad->SetLeftMargin(0.1);
    gPad->SetTopMargin(0.01);
    gPad->SetLogx();

    // v0(pT) | pT-refs

    // pT-ref: [0.5, 2.0]
    TGraphErrors *gr_v0pt_ptref_1 = (TGraphErrors*)f_main->Get("v0pt_ptref_1_5060");
    // pT-ref: [0.5, 5.0]
    TGraphErrors *gr_v0pt_ptref_2 = (TGraphErrors*)f_main->Get("v0pt_ptref_2_5060");
    // pT-ref: [1.0, 5.0]
    TGraphErrors *gr_v0pt_ptref_3 = (TGraphErrors*)f_main->Get("v0pt_ptref_3_5060");
    // EDITING OUR DATA TGRAPH
    customize_TGraphErrors(gr_v0pt_ptref_1, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 24, 2, 1.0);
    customize_TGraphErrors(gr_v0pt_ptref_2, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 25, 4, 1.0);
    customize_TGraphErrors(gr_v0pt_ptref_3, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 26, 6, 1.0);

    // READING TGRAPHS FILE FROM ATLAS DATA AND GETTING AND EDITING THEM
    TFile *f_v0pt_ptref_ATLAS = TFile::Open("./Data/Figures_ATLAS/v0pt_ptref_ATLAS.root");
    // pT-ref: [0.5, 2.0]
    TGraphAsymmErrors *gr_v0pt_ptref_ATLAS_1 = (TGraphAsymmErrors*)f_v0pt_ptref_ATLAS->Get("Figure 2d_lower panel/Graph1D_y1");
    // pT-ref: [0.5, 5.0]
    TGraphAsymmErrors *gr_v0pt_ptref_ATLAS_2 = (TGraphAsymmErrors*)f_v0pt_ptref_ATLAS->Get("Figure 2d_lower panel/Graph1D_y2");
    // pT-ref: [1.0, 5.0]
    TGraphAsymmErrors *gr_v0pt_ptref_ATLAS_3 = (TGraphAsymmErrors*)f_v0pt_ptref_ATLAS->Get("Figure 2d_lower panel/Graph1D_y3");
    // EDITING ATLAS DATA TGRAPH
    customize_TGraphAsymmErrors(gr_v0pt_ptref_ATLAS_1, " ; p_{T} [GeV]; v_{0}(p_{T})v_{0}x10^{3}", 0.0, 10.0, -0.1, 0.5, 20, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ptref_ATLAS_2, " ; p_{T} [GeV]; v_{0}(p_{T})v_{0}x10^{3}", 0.0, 10.0, -0.1, 0.5, 21, 4, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ptref_ATLAS_3, " ; p_{T} [GeV]; v_{0}(p_{T})v_{0}x10^{3}", 0.0, 10.0, -0.1, 0.5, 22, 6, 1.0);

    auto leg_v0pt_ptref_title = new TLegend(0.04, 0.98, 0.5, 0.825);
    leg_v0pt_ptref_title->SetTextSize(0.055);
    leg_v0pt_ptref_title->AddEntry((TObject*)0, "Pb+Pb,   #eta_{gap} = 1", "");
    leg_v0pt_ptref_title->AddEntry((TObject*)0, "50-60% Centrality", "");
    leg_v0pt_ptref_title->SetBorderSize(0);
    leg_v0pt_ptref_title->SetFillStyle(0);

    auto leg_v0pt_ptref = new TLegend(0.12, 0.82, 0.5, 0.68);
    leg_v0pt_ptref->SetTextSize(0.030);
    leg_v0pt_ptref->AddEntry(gr_v0pt_ptref_1, "CMS OpenData 2.76 TeV: 0.5-2 GeV", "p");
    leg_v0pt_ptref->AddEntry(gr_v0pt_ptref_2, "CMS OpenData 2.76 TeV: 0.5-5 GeV", "p");
    leg_v0pt_ptref->AddEntry(gr_v0pt_ptref_3, "CMS OpenData 2.76 TeV: 1-5 GeV", "p");
    leg_v0pt_ptref->SetBorderSize(0);
    leg_v0pt_ptref->SetFillStyle(0);

    auto leg_v0pt_ptref2 = new TLegend(0.4, 0.25, 0.9, 0.12);
    leg_v0pt_ptref2->SetTextSize(0.030);
    leg_v0pt_ptref2->AddEntry(gr_v0pt_ptref_ATLAS_1, "ATLAS 5.02 TeV: 0.5-2 GeV", "p");
    leg_v0pt_ptref2->AddEntry(gr_v0pt_ptref_ATLAS_2, "ATLAS 5.02 TeV: 0.5-5 GeV", "p");
    leg_v0pt_ptref2->AddEntry(gr_v0pt_ptref_ATLAS_3, "ATLAS 5.02 TeV: 1-5 GeV", "p");
    leg_v0pt_ptref2->SetBorderSize(0);
    leg_v0pt_ptref2->SetFillStyle(0);

    c->cd(3);
    gr_v0pt_ptref_1->Draw("AP");
    gr_v0pt_ptref_2->Draw("P SAME");
    gr_v0pt_ptref_3->Draw("P SAME");
    gr_v0pt_ptref_ATLAS_1->Draw("P SAME");
    gr_v0pt_ptref_ATLAS_2->Draw("P SAME");
    gr_v0pt_ptref_ATLAS_3->Draw("P SAME");
    leg_v0pt_ptref_title->Draw();
    leg_v0pt_ptref->Draw();
    leg_v0pt_ptref2->Draw();
    gPad->SetLogx();
    gPad->SetLeftMargin(0.12);
    gPad->SetTopMargin(0.02);

    // v0(pT) | Etas

    TFile *f_etas = TFile::Open("./Data/Figures/etas.root");
    TGraphErrors *gr_v0pt_eta0 = (TGraphErrors*)f_etas->Get("v0pt_ptref_eta0_5060");
    TGraphErrors *gr_v0pt_eta1 = (TGraphErrors*)f_etas->Get("v0pt_ptref_eta1_5060");
    TGraphErrors *gr_v0pt_eta2 = (TGraphErrors*)f_etas->Get("v0pt_ptref_eta2_5060");
    TGraphErrors *gr_v0pt_eta3 = (TGraphErrors*)f_etas->Get("v0pt_ptref_eta3_5060");
    customize_TGraphErrors(gr_v0pt_eta0, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 24, 2, 1.0);
    customize_TGraphErrors(gr_v0pt_eta1, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 25, 4, 1.0);
    customize_TGraphErrors(gr_v0pt_eta2, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 26, 6, 1.0);
    customize_TGraphErrors(gr_v0pt_eta3, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 28, 1, 1.0);

    TFile *f_ATLAS_eta0 = TFile::Open("./Data/Figures_ATLAS/v0pt_ATLAS_eta0.root");
    TGraphAsymmErrors *gr_v0pt_ATLAS_eta0 = (TGraphAsymmErrors*)f_ATLAS_eta0->Get("Figure 3b_etagap0/Graph1D_y1");
    TFile *f_ATLAS_eta1 = TFile::Open("./Data/Figures_ATLAS/v0pt_ATLAS_eta1.root");
    TGraphAsymmErrors *gr_v0pt_ATLAS_eta1 = (TGraphAsymmErrors*)f_ATLAS_eta1->Get("Figure 3b_etagap1/Graph1D_y1");
    TFile *f_ATLAS_eta2 = TFile::Open("./Data/Figures_ATLAS/v0pt_ATLAS_eta2.root");
    TGraphAsymmErrors *gr_v0pt_ATLAS_eta2 = (TGraphAsymmErrors*)f_ATLAS_eta2->Get("Figure 3b_etagap2/Graph1D_y1");
    TFile *f_ATLAS_eta3 = TFile::Open("./Data/Figures_ATLAS/v0pt_ATLAS_eta3.root");
    TGraphAsymmErrors *gr_v0pt_ATLAS_eta3 = (TGraphAsymmErrors*)f_ATLAS_eta3->Get("Figure 3b_etagap3/Graph1D_y1");
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_eta0, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 20, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_eta1, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 21, 4, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_eta2, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 22, 6, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_eta3, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 34, 1, 1.0);

    auto leg_v0pt_eta_title = new TLegend(0.035, 0.98, 0.5, 0.825);
    leg_v0pt_eta_title->SetTextSize(0.055);
    leg_v0pt_eta_title->AddEntry((TObject*)0, "Pb+Pb,   p_{T}^{ref}: 0.5-2 GeV", "");
    leg_v0pt_eta_title->AddEntry((TObject*)0, "50-60% Centrality", "");
    leg_v0pt_eta_title->SetBorderSize(0);
    leg_v0pt_eta_title->SetFillStyle(0);

    auto leg_v0pt_eta = new TLegend(0.12, 0.82, 0.5, 0.64);
    leg_v0pt_eta->SetTextSize(0.03);
    leg_v0pt_eta->AddEntry(gr_v0pt_eta0, "CMS Open Data 2.76 TeV    #eta_{gap} = 0", "p");
    leg_v0pt_eta->AddEntry(gr_v0pt_eta1, "CMS Open Data 2.76 TeV    #eta_{gap} = 1", "p");
    leg_v0pt_eta->AddEntry(gr_v0pt_eta2, "CMS Open Data 2.76 TeV    #eta_{gap} = 2", "p");
    leg_v0pt_eta->AddEntry(gr_v0pt_eta3, "CMS Open Data 2.76 TeV    #eta_{gap} = 3", "p");
    leg_v0pt_eta->SetBorderSize(0);
    leg_v0pt_eta->SetFillStyle(0);

    auto leg_v0pt_eta2 = new TLegend(0.4, 0.29, 0.9, 0.12);
    leg_v0pt_eta2->SetTextSize(0.03);
    leg_v0pt_eta2->AddEntry(gr_v0pt_ATLAS_eta0, "ATLAS 5.02 TeV    #eta_{gap} = 0", "p");
    leg_v0pt_eta2->AddEntry(gr_v0pt_ATLAS_eta1, "ATLAS 5.02 TeV    #eta_{gap} = 1", "p");
    leg_v0pt_eta2->AddEntry(gr_v0pt_ATLAS_eta2, "ATLAS 5.02 TeV    #eta_{gap} = 2", "p");
    leg_v0pt_eta2->AddEntry(gr_v0pt_ATLAS_eta3, "ATLAS 5.02 TeV    #eta_{gap} = 3", "p");
    leg_v0pt_eta2->SetBorderSize(0);
    leg_v0pt_eta2->SetFillStyle(0);

    c->cd(4);
    gr_v0pt_eta0->Draw("AP");
    gr_v0pt_eta1->Draw("P SAME");
    gr_v0pt_eta2->Draw("P SAME");
    gr_v0pt_eta3->Draw("P SAME");
    gr_v0pt_ATLAS_eta0->Draw("P SAME");
    gr_v0pt_ATLAS_eta1->Draw("P SAME");
    gr_v0pt_ATLAS_eta2->Draw("P SAME");
    gr_v0pt_ATLAS_eta3->Draw("P SAME");
    leg_v0pt_eta_title->Draw();
    leg_v0pt_eta->Draw();
    leg_v0pt_eta2->Draw();
    gPad->SetLogx();
    gPad->SetLeftMargin(0.12);
    gPad->SetTopMargin(0.02);

    c->Update();
    c->SaveAs("PlotRefsEta.pdf");
    delete c;
}

void DoPlotCorrection(){
    TFile *f = TFile::Open("./Data/Figures/v0_noCorrec.root", "READ");
    TGraphErrors *gr1 = (TGraphErrors*)f->Get("55_1");
    TGraphErrors *gr2 = (TGraphErrors*)f->Get("55_2");
    TGraphErrors *gr3 = (TGraphErrors*)f->Get("55_3");
    TGraphErrors *gr4 = (TGraphErrors*)f->Get("65_1");
    TGraphErrors *gr5 = (TGraphErrors*)f->Get("65_2");
    TGraphErrors *gr6 = (TGraphErrors*)f->Get("65_3");

    TFile *cf = TFile::Open("./Data/Figures/v0.root", "READ");
    TGraphErrors *grc1 = (TGraphErrors*)cf->Get("55_1");
    TGraphErrors *grc2 = (TGraphErrors*)cf->Get("55_2");
    TGraphErrors *grc3 = (TGraphErrors*)cf->Get("55_3");
    TGraphErrors *grc4 = (TGraphErrors*)cf->Get("65_1");
    TGraphErrors *grc5 = (TGraphErrors*)cf->Get("65_2");
    TGraphErrors *grc6 = (TGraphErrors*)cf->Get("65_3");

    customize_TGraphErrors(gr1, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 47, 52, 1.0);
    customize_TGraphErrors(gr2, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 33, 7, 1.0);
    customize_TGraphErrors(gr3, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 34, 95, 1.0);
    customize_TGraphErrors(gr4, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 47, 52, 1.0);
    customize_TGraphErrors(gr5, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 33, 7, 1.0);
    customize_TGraphErrors(gr6, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 34, 95, 1.0);

    customize_TGraphErrors(grc1, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 20, 2, 1.0);
    customize_TGraphErrors(grc2, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 21, 4, 1.0);
    customize_TGraphErrors(grc3, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 25, 6, 1.0);
    customize_TGraphErrors(grc4, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 20, 2, 1.0);
    customize_TGraphErrors(grc5, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 21, 4, 1.0);
    customize_TGraphErrors(grc6, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 25, 6, 1.0);

    auto leg_title = new TLegend(0.1, 0.89, 0.5, 0.76);
    leg_title->SetTextSize(0.0457);
    leg_title->AddEntry((TObject*)0, "CMS Open Data 2.76 TeV", "");
    leg_title->AddEntry((TObject*)0, "Pb+Pb   #eta_{gap} = 1", "");
    leg_title->SetBorderSize(0);
    leg_title->SetFillStyle(0);

    auto leg_ptref = new TLegend(0.38, 0.43, 0.67, 0.15);
    leg_ptref->SetTextSize(0.032);
    leg_ptref->AddEntry((TObject*)0, "#bf{p_{T}^{ref} range}", "C");
    leg_ptref->AddEntry(gr1, "Without correction   0.5-2 GeV", "p");
    leg_ptref->AddEntry(gr2, "Without correction   0.5-5 GeV", "p");
    leg_ptref->AddEntry(gr3, "Without correction   1-5 GeV", "p");
    leg_ptref->AddEntry(grc1, "With correction   0.5-2 GeV", "p");
    leg_ptref->AddEntry(grc2, "With correction   0.5-5 GeV", "p");
    leg_ptref->AddEntry(grc3, "With correction   1-5 GeV", "p");
    leg_ptref->SetBorderSize(0);
    leg_ptref->SetFillStyle(0);

    auto c = new TCanvas("c", "c", 500, 500);
    grc1->Draw("AP");
    grc2->Draw("P SAME");
    grc3->Draw("P SAME");
    grc4->Draw("P SAME");
    grc5->Draw("P SAME");
    grc6->Draw("P SAME");
    gr1->Draw("P SAME");
    gr2->Draw("P SAME");
    gr3->Draw("P SAME");
    gr4->Draw("P SAME");
    gr5->Draw("P SAME");
    gr6->Draw("P SAME");
    leg_title->Draw();
    leg_ptref->Draw();
    gPad->SetLeftMargin(0.14);
    c->SetLogy();

    c->SaveAs("Corrections.pdf");
    delete c;
}

void DoPlotMain(){
    TFile *f;
    f = TFile::Open("./Data/Figures/main.root", "READ");

    // v0(pT)
    TGraph *gr_v0pt_5060 = (TGraph*)f->Get("v0pt_ptref_1_5060");
    TGraph *gr_v0pt_6070 = (TGraph*)f->Get("v0pt_ptref_1_6070");

    TFile *f_v0pt_ATLAS_5060 = TFile::Open("./Data/Figures_ATLAS/v0pt_ATLAS_5060.root", "READ");
    TGraphAsymmErrors* gr_v0pt_ATLAS_5060 = (TGraphAsymmErrors*)f_v0pt_ATLAS_5060->Get("Figure 4a_cent7/Graph1D_y1");
    TFile *f_v0pt_ATLAS_6070 = TFile::Open("./Data/Figures_ATLAS/v0pt_ATLAS_6070.root", "READ");
    TGraphAsymmErrors* gr_v0pt_ATLAS_6070 = (TGraphAsymmErrors*)f_v0pt_ATLAS_6070->Get("Figure 4a_cent8/Graph1D_y1");

    customize_TGraph(gr_v0pt_5060, "; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.46, 21, 2, 1.0);
    customize_TGraph(gr_v0pt_6070, "; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.46, 20, 4, 1.0);

    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_5060, "; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.46, 25, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_6070, "; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.46, 24, 4, 1.0);

    auto c = new TCanvas("c", "c", 1100, 500);
    c->Divide(2, 1);

    // v0(pT) legend
    auto legend_v0pt_text = new TLegend(0.025, 0.98, 0.5, 0.82);
    legend_v0pt_text->SetTextSize(0.055);
    legend_v0pt_text->AddEntry((TObject*)0, "Pb+Pb,   #eta_{gap} = 1", "");
    legend_v0pt_text->AddEntry((TObject*)0, "p_{T}^{ref} 0.5-2 GeV", "");
    legend_v0pt_text->SetBorderSize(0);
    legend_v0pt_text->SetFillStyle(0);

    auto legend_v0pt_cents = new TLegend(0.103, 0.81, 0.5, 0.61);
    legend_v0pt_cents->SetTextSize(0.034);
    legend_v0pt_cents->AddEntry(gr_v0pt_5060, "CMS OpenData 2.76 TeV (50-60%)", "p");
    legend_v0pt_cents->AddEntry(gr_v0pt_6070, "CMS OpenData 2.76 TeV (60-70%)", "p");
    legend_v0pt_cents->AddEntry(gr_v0pt_ATLAS_5060, "ATLAS 5.02 TeV (50-60%)", "p");
    legend_v0pt_cents->AddEntry(gr_v0pt_ATLAS_6070, "ATLAS 5.02 TeV (60-70%)", "p");
    legend_v0pt_cents->SetBorderSize(0);
    legend_v0pt_cents->SetFillStyle(0);

    auto legend_v0pt_label = new TLegend(0.78, 0.9, 0.93, 0.93);
    legend_v0pt_label->SetTextSize(0.055);
    legend_v0pt_label->AddEntry((TObject*)0, "(a)", "");
    legend_v0pt_label->SetBorderSize(0);
    legend_v0pt_label->SetFillStyle(0);

    // Drawing v0(pT) plot
    c->cd(1);
    gr_v0pt_ATLAS_5060->Draw("AP");
    gr_v0pt_ATLAS_6070->Draw("P SAME");
    gr_v0pt_5060->Draw("P SAME");
    gr_v0pt_6070->Draw("P SAME");
    legend_v0pt_text->Draw();
    legend_v0pt_cents->Draw();
    legend_v0pt_label->Draw();
    gPad->SetLogx();
    gPad->SetTopMargin(0.01);

    // v0(pT)/v0
    TGraph *gr_sv0pt_5060 = (TGraph*)f->Get("sv0pt_ptref_1_5060");
    TGraph *gr_sv0pt_6070 = (TGraph*)f->Get("sv0pt_ptref_1_6070");

    TFile *f_sv0pt_ATLAS_5060 = TFile::Open("./Data/Figures_ATLAS/sv0pt_ATLAS_5060.root", "READ");
    TGraphAsymmErrors* gr_sv0pt_ATLAS_5060 = (TGraphAsymmErrors*)f_sv0pt_ATLAS_5060->Get("Figure 4b_cent7/Graph1D_y1");
    TFile *f_sv0pt_ATLAS_6070 = TFile::Open("./Data/Figures_ATLAS/sv0pt_ATLAS_6070.root", "READ");
    TGraphAsymmErrors* gr_sv0pt_ATLAS_6070 = (TGraphAsymmErrors*)f_sv0pt_ATLAS_6070->Get("Figure 4b_cent8/Graph1D_y1");

    customize_TGraph(gr_sv0pt_5060, "; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 21, 2, 1.0);
    customize_TGraph(gr_sv0pt_6070, "; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 20, 4, 1.0);

    customize_TGraphAsymmErrors(gr_sv0pt_ATLAS_5060, "; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 25, 2, 1.0);
    customize_TGraphAsymmErrors(gr_sv0pt_ATLAS_6070, "; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 24, 4, 1.0);

    // v0(pT)/v0 legend
    auto legend_sv0pt_text = new TLegend(0.025, 0.98, 0.5, 0.82);
    legend_sv0pt_text->SetTextSize(0.055);
    legend_sv0pt_text->AddEntry((TObject*)0, "Pb+Pb,   #eta_{gap} = 1", "");
    legend_sv0pt_text->AddEntry((TObject*)0, "p_{T}^{ref} 0.5-2 GeV", "");
    legend_sv0pt_text->SetBorderSize(0);
    legend_sv0pt_text->SetFillStyle(0);

    auto legend_sv0pt_cents = new TLegend(0.103, 0.81, 0.5, 0.61);
    legend_sv0pt_cents->SetTextSize(0.034);
    legend_sv0pt_cents->AddEntry(gr_sv0pt_5060, "CMS OpenData 2.76 TeV (50-60%)", "p");
    legend_sv0pt_cents->AddEntry(gr_sv0pt_6070, "CMS OpenData 2.76 TeV (60-70%)", "p");
    legend_sv0pt_cents->AddEntry(gr_sv0pt_ATLAS_5060, "ATLAS 5.02 TeV (50-60%)", "p");
    legend_sv0pt_cents->AddEntry(gr_sv0pt_ATLAS_6070, "ATLAS 5.02 TeV (60-70%)", "p");
    legend_sv0pt_cents->SetBorderSize(0);
    legend_sv0pt_cents->SetFillStyle(0);

    auto legend_sv0pt_label = new TLegend(0.78, 0.9, 0.93, 0.93);
    legend_sv0pt_label->SetTextSize(0.055);
    legend_sv0pt_label->AddEntry((TObject*)0, "(b)", "");
    legend_sv0pt_label->SetBorderSize(0);
    legend_sv0pt_label->SetFillStyle(0);
   
    // Drawing v0(pT)/v0 plot
    c->cd(2);
    gr_sv0pt_ATLAS_5060->Draw("AP");
    gr_sv0pt_ATLAS_6070->Draw("P SAME");
    gr_sv0pt_5060->Draw("P SAME");
    gr_sv0pt_6070->Draw("P SAME");
    legend_sv0pt_text->Draw();
    legend_sv0pt_cents->Draw();
    legend_sv0pt_label->Draw();
    gPad->SetLogx();
    gPad->SetTopMargin(0.01);
    // Saving canvas as pdf
    c->Update();
    c->SaveAs("PlotMain.pdf");
    delete c;
}