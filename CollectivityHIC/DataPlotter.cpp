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
    g->SetFillColorAlpha(color, 0.3);
    g->SetFillStyle(1001);
}

void customize_TGraphAsymmErrors(TGraphAsymmErrors *g, const char* title, float xmin, float xmax, float ymin, float ymax, int style, int color, float size){
    g->SetTitle(title);
    g->GetXaxis()->SetLimits(xmin, xmax); // X axis range
    g->GetXaxis()->CenterTitle(); // Center axis label
    g->GetYaxis()->CenterTitle(); // Center axis label
    g->GetYaxis()->SetTitleFont(42);
    g->SetMinimum(ymin); // Y axis range
    g->SetMaximum(ymax); // Y axis range
    g->SetMarkerStyle(style);
    g->SetMarkerColor(color);
    g->SetLineColor(color);
    g->SetMarkerSize(size);
    g->SetFillColorAlpha(color, 0.3);
    g->SetFillStyle(1001);
}

void customize_TGraphErrors(TGraphErrors *g, const char* title, float xmin, float xmax, float ymin, float ymax, int style, int color, float size){
    g->SetTitle(title);
    g->GetXaxis()->SetLimits(xmin, xmax); // X axis range
    g->GetXaxis()->CenterTitle(); // Center axis label
    g->GetYaxis()->CenterTitle(); // Center axis label
    g->GetYaxis()->SetTitleFont(42);
    g->SetMinimum(ymin); // Y axis range
    g->SetMaximum(ymax); // Y axis range
    g->SetMarkerStyle(style);
    g->SetMarkerColor(color);
    g->SetLineColor(color);
    g->SetMarkerSize(size);
    g->SetFillColorAlpha(color, 0.3);
    g->SetFillStyle(1001);
}

pair<TGraphAsymmErrors*, TGraphAsymmErrors*> csvToTGraph(TString filepath){
    ifstream f(filepath);

    TGraphAsymmErrors *g_stat = new TGraphAsymmErrors();
    TGraphAsymmErrors *g_syst = new TGraphAsymmErrors();

    double x, y, ystat_low, ystat_high, ysyst_low, ysyst_high, xsyst_low, xsyst_high;
    char c;
    int i = 0;

    while (f >> x >> c >> y >> c >> ystat_high >> c >> ystat_low >> c >> ysyst_high >> c >> ysyst_low){
        g_stat->SetPoint(i, x, y);
        g_syst->SetPoint(i, x, y);

        g_stat->SetPointError(i, 0, 0, abs(ystat_low), abs(ystat_high));
        if (filepath.BeginsWith("./Data/Figures_ATLAS/v0_")) g_syst->SetPointError(i, 0.3, 0.3, abs(ysyst_low), abs(ysyst_high));
        else g_syst->SetPointError(i, (x-(x/1.05)), ((x*1.05)-x), abs(ysyst_low), abs(ysyst_high)); // log scale in x axis

        i++;
    }

    f.close();
    return {g_stat, g_syst};
}

void DoPlotRefsEta(){
    auto c = new TCanvas("c", "c_refs_eta", 1100, 1000);
    c->Divide(2, 2, 0.00000001, 0.00000001);

    // I open take pT-ref uncertainties files first because they'll be repeated for v0, v0(pT)v0 and v0(pT)
    TFile *f_unc_ptref1 = TFile::Open("./Data/Systematics/SystUncs.root");
    TFile *f_unc_ptref2 = TFile::Open("./Data/Systematics/Variations/pTref2/SystUncs.root");
    TFile *f_unc_ptref3 = TFile::Open("./Data/Systematics/Variations/pTref3/SystUncs.root");

    // v0 | pT-refs

    TFile *f_v0 = TFile::Open("./Data/Figures/main.root", "READ");
    TGraphErrors *gr_v0_55_1 = (TGraphErrors*)f_v0->Get("v0_55_1");
    TGraphErrors *gr_v0_55_2 = (TGraphErrors*)f_v0->Get("v0_55_2");
    TGraphErrors *gr_v0_55_3 = (TGraphErrors*)f_v0->Get("v0_55_3");
    TGraphErrors *gr_v0_65_1 = (TGraphErrors*)f_v0->Get("v0_65_1");
    TGraphErrors *gr_v0_65_2 = (TGraphErrors*)f_v0->Get("v0_65_2");
    TGraphErrors *gr_v0_65_3 = (TGraphErrors*)f_v0->Get("v0_65_3");

    TGraphErrors *gr_v0_1_unc = (TGraphErrors*)f_unc_ptref1->Get("gr_v0_total");
    TGraphErrors *gr_v0_2_unc = (TGraphErrors*)f_unc_ptref2->Get("gr_v0_total");
    TGraphErrors *gr_v0_3_unc = (TGraphErrors*)f_unc_ptref3->Get("gr_v0_total");

    auto p_v0_ATLAS_1 = csvToTGraph("./Data/Figures_ATLAS/v0_ATLAS_1.csv");
        TGraphAsymmErrors *gr_v0_ATLAS_1 = p_v0_ATLAS_1.first;
        TGraphAsymmErrors *gr_v0_ATLAS_1_unc = p_v0_ATLAS_1.second;
    auto p_v0_ATLAS_2 = csvToTGraph("./Data/Figures_ATLAS/v0_ATLAS_2.csv");
        TGraphAsymmErrors *gr_v0_ATLAS_2 = p_v0_ATLAS_2.first;
        TGraphAsymmErrors *gr_v0_ATLAS_2_unc = p_v0_ATLAS_2.second;
    auto p_v0_ATLAS_3 = csvToTGraph("./Data/Figures_ATLAS/v0_ATLAS_3.csv");
        TGraphAsymmErrors *gr_v0_ATLAS_3 = p_v0_ATLAS_3.first;
        TGraphAsymmErrors *gr_v0_ATLAS_3_unc = p_v0_ATLAS_3.second;

    customize_TGraphErrors(gr_v0_55_1, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 20, 2, 1.2);
    customize_TGraphErrors(gr_v0_55_2, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 21, 4, 1.2);
    customize_TGraphErrors(gr_v0_55_3, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 22, 6, 1.2);
    customize_TGraphErrors(gr_v0_65_1, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 20, 2, 1.2);
    customize_TGraphErrors(gr_v0_65_2, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 21, 4, 1.2);
    customize_TGraphErrors(gr_v0_65_3, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 22, 6, 1.2);

    customize_TGraphErrors(gr_v0_1_unc, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 20, 2, 1.2);
    customize_TGraphErrors(gr_v0_2_unc, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 21, 4, 1.2);
    customize_TGraphErrors(gr_v0_3_unc, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 22, 6, 1.2);

    customize_TGraphAsymmErrors(gr_v0_ATLAS_1, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 24, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0_ATLAS_2, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 25, 4, 1.0);
    customize_TGraphAsymmErrors(gr_v0_ATLAS_3, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 26, 6, 1.0);

    customize_TGraphAsymmErrors(gr_v0_ATLAS_1_unc, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 24, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0_ATLAS_2_unc, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 25, 4, 1.0);
    customize_TGraphAsymmErrors(gr_v0_ATLAS_3_unc, "; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 26, 6, 1.0);

    auto leg_v0_title = new TLegend(0.02, 1.1, 0.5, 0.77);
    leg_v0_title->SetTextSize(0.06);
    leg_v0_title->AddEntry((TObject*)0, "Pb+Pb,    #eta_{gap} = 1", "");
    leg_v0_title->SetBorderSize(0);
    leg_v0_title->SetFillStyle(0);

    auto leg_v0_ptref = new TLegend(0.35, 0.41, 0.7, 0.13);
    leg_v0_ptref->SetTextSize(0.030);
    leg_v0_ptref->AddEntry(gr_v0_55_1, "CMS Open Data 2.76 TeV: 0.5-2 GeV", "pfl");
    leg_v0_ptref->AddEntry(gr_v0_55_2, "CMS Open Data 2.76 TeV: 0.5-5 GeV", "pfl");
    leg_v0_ptref->AddEntry(gr_v0_55_3, "CMS Open Data 2.76 TeV: 1-5 GeV", "pfl");
    leg_v0_ptref->AddEntry(gr_v0_ATLAS_1, "ATLAS 5.02 TeV: 0.5-2 GeV", "pfl");
    leg_v0_ptref->AddEntry(gr_v0_ATLAS_2, "ATLAS 5.02 TeV: 0.5-5 GeV", "pfl");
    leg_v0_ptref->AddEntry(gr_v0_ATLAS_3, "ATLAS 5.02 TeV: 1-5 GeV", "pfl");
    leg_v0_ptref->SetBorderSize(0);
    leg_v0_ptref->SetFillStyle(0);

    auto leg_v0_label = new TLegend(0.78, 0.9, 0.93, 0.93);
    leg_v0_label->SetTextSize(0.055);
    leg_v0_label->AddEntry((TObject*)0, "(a)", "");
    leg_v0_label->SetBorderSize(0);
    leg_v0_label->SetFillStyle(0);

    c->cd(1);
    gr_v0_ATLAS_1_unc->Draw("A2");
    gr_v0_ATLAS_2_unc->Draw("2 SAME");
    gr_v0_ATLAS_3_unc->Draw("2 SAME");
    gr_v0_1_unc->Draw("2 SAME");
    gr_v0_2_unc->Draw("2 SAME");
    gr_v0_3_unc->Draw("2 SAME");
    gr_v0_ATLAS_1->Draw("P SAME");
    gr_v0_ATLAS_2->Draw("P SAME");
    gr_v0_ATLAS_3->Draw("P SAME");
    gr_v0_55_1->Draw("P SAME");
    gr_v0_55_2->Draw("P SAME");
    gr_v0_55_3->Draw("P SAME");
    gr_v0_65_1->Draw("P SAME");
    gr_v0_65_2->Draw("P SAME");
    gr_v0_65_3->Draw("P SAME");
    
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
    customize_TGraphErrors(gr_v0ptv0_ptref_1, " ; p_{T} [GeV]; (#LT#deltan(p_{T})#delta[p_{T}]#GT/#LTn(p_{T})#GT#LTp_{T}#GT)x10^{3}", 0.0, 10.0, -2.0, 11.0, 20, 2, 1.0);
    customize_TGraphErrors(gr_v0ptv0_ptref_2, " ; p_{T} [GeV]; (#LT#deltan(p_{T})#delta[p_{T}]#GT/#LTn(p_{T})#GT#LTp_{T}#GT)x10^{3}", 0.0, 10.0, -2.0, 11.0, 21, 4, 1.0);
    customize_TGraphErrors(gr_v0ptv0_ptref_3, " ; p_{T} [GeV]; (#LT#deltan(p_{T})#delta[p_{T}]#GT/#LTn(p_{T})#GT#LTp_{T}#GT)x10^{3}", 0.0, 10.0, -2.0, 11.0, 22, 6, 1.0);

    // READING TGRAPHS FILE FROM ATLAS DATA
    auto p_v0ptv0_ptref_ATLAS_1 = csvToTGraph("./Data/Figures_ATLAS/v0ptv0_ptref_ATLAS_1.csv");
        TGraphAsymmErrors *gr_v0ptv0_ptref_ATLAS_1 = p_v0ptv0_ptref_ATLAS_1.first;
        TGraphAsymmErrors *gr_v0ptv0_ptref_ATLAS_1_unc = p_v0ptv0_ptref_ATLAS_1.second;
    auto p_v0ptv0_ptref_ATLAS_2 = csvToTGraph("./Data/Figures_ATLAS/v0ptv0_ptref_ATLAS_2.csv");
        TGraphAsymmErrors *gr_v0ptv0_ptref_ATLAS_2 = p_v0ptv0_ptref_ATLAS_2.first;
        TGraphAsymmErrors *gr_v0ptv0_ptref_ATLAS_2_unc = p_v0ptv0_ptref_ATLAS_2.second;
    auto p_v0ptv0_ptref_ATLAS_3 = csvToTGraph("./Data/Figures_ATLAS/v0ptv0_ptref_ATLAS_3.csv");
        TGraphAsymmErrors *gr_v0ptv0_ptref_ATLAS_3 = p_v0ptv0_ptref_ATLAS_3.first;
        TGraphAsymmErrors *gr_v0ptv0_ptref_ATLAS_3_unc = p_v0ptv0_ptref_ATLAS_3.second;
    // EDITING ATLAS DATA TGRAPH
    customize_TGraphAsymmErrors(gr_v0ptv0_ptref_ATLAS_1, " ; p_{T} [GeV]; (#LT#deltan(p_{T})#delta[p_{T}]#GT/#LTn(p_{T})#GT#LTp_{T}#GT)x10^{3}", 0.0, 10.0, -2.0, 11.0, 24, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0ptv0_ptref_ATLAS_2, " ; p_{T} [GeV]; (#LT#deltan(p_{T})#delta[p_{T}]#GT/#LTn(p_{T})#GT#LTp_{T}#GT)x10^{3}", 0.0, 10.0, -2.0, 11.0, 25, 4, 1.0);
    customize_TGraphAsymmErrors(gr_v0ptv0_ptref_ATLAS_3, " ; p_{T} [GeV]; (#LT#deltan(p_{T})#delta[p_{T}]#GT/#LTn(p_{T})#GT#LTp_{T}#GT)x10^{3}", 0.0, 10.0, -2.0, 11.0, 26, 6, 1.0);

    customize_TGraphAsymmErrors(gr_v0ptv0_ptref_ATLAS_1_unc, " ; p_{T} [GeV]; (#LT#deltan(p_{T})#delta[p_{T}]#GT/#LTn(p_{T})#GT#LTp_{T}#GT)x10^{3}", 0.0, 10.0, -2.0, 11.0, 24, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0ptv0_ptref_ATLAS_2_unc, " ; p_{T} [GeV]; (#LT#deltan(p_{T})#delta[p_{T}]#GT/#LTn(p_{T})#GT#LTp_{T}#GT)x10^{3}", 0.0, 10.0, -2.0, 11.0, 25, 4, 1.0);
    customize_TGraphAsymmErrors(gr_v0ptv0_ptref_ATLAS_3_unc, " ; p_{T} [GeV]; (#LT#deltan(p_{T})#delta[p_{T}]#GT/#LTn(p_{T})#GT#LTp_{T}#GT)x10^{3}", 0.0, 10.0, -2.0, 11.0, 26, 6, 1.0);

    TGraphErrors *gr_v0ptv0_ptref_1_unc = (TGraphErrors*)f_unc_ptref1->Get("gr_v0ptv0_5060_total");
    TGraphErrors *gr_v0ptv0_ptref_2_unc = (TGraphErrors*)f_unc_ptref2->Get("gr_v0ptv0_5060_total");
    TGraphErrors *gr_v0ptv0_ptref_3_unc = (TGraphErrors*)f_unc_ptref3->Get("gr_v0ptv0_5060_total");
    customize_TGraphErrors(gr_v0ptv0_ptref_1_unc, " ; p_{T} [GeV]; (#LT#deltan(p_{T})#delta[p_{T}]#GT/#LTn(p_{T})#GT#LTp_{T}#GT)x10^{3}", 0.0, 10.0, -2.0, 11.0, 20, 2, 1.0);
    customize_TGraphErrors(gr_v0ptv0_ptref_2_unc, " ; p_{T} [GeV]; (#LT#deltan(p_{T})#delta[p_{T}]#GT/#LTn(p_{T})#GT#LTp_{T}#GT)x10^{3}", 0.0, 10.0, -2.0, 11.0, 21, 4, 1.0);
    customize_TGraphErrors(gr_v0ptv0_ptref_3_unc, " ; p_{T} [GeV]; (#LT#deltan(p_{T})#delta[p_{T}]#GT/#LTn(p_{T})#GT#LTp_{T}#GT)x10^{3}", 0.0, 10.0, -2.0, 11.0, 22, 6, 1.0);


    // CONFIGURING TLEGENDS
    auto leg_v0ptv0_ptref_title = new TLegend(0.025, 0.98, 0.5, 0.825);
    leg_v0ptv0_ptref_title->SetTextSize(0.06);
    leg_v0ptv0_ptref_title->AddEntry((TObject*)0, "Pb+Pb,   #eta_{gap} = 1", "");
    leg_v0ptv0_ptref_title->AddEntry((TObject*)0, "50-60% Centrality", "");
    leg_v0ptv0_ptref_title->SetBorderSize(0);
    leg_v0ptv0_ptref_title->SetFillStyle(0);
    auto leg_v0ptv0_ptref = new TLegend(0.12, 0.82, 0.5, 0.68);
    leg_v0ptv0_ptref->SetTextSize(0.030);
    leg_v0ptv0_ptref->AddEntry(gr_v0ptv0_ptref_1, "CMS OpenData 2.76 TeV: 0.5-2 GeV", "pfl");
    leg_v0ptv0_ptref->AddEntry(gr_v0ptv0_ptref_2, "CMS OpenData 2.76 TeV: 0.5-5 GeV", "pfl");
    leg_v0ptv0_ptref->AddEntry(gr_v0ptv0_ptref_3, "CMS OpenData 2.76 TeV: 1-5 GeV", "pfl");
    leg_v0ptv0_ptref->SetBorderSize(0);
    leg_v0ptv0_ptref->SetFillStyle(0);

    auto leg_v0ptv0_ptref2 = new TLegend(0.46, 0.25, 0.85, 0.12);
    leg_v0ptv0_ptref2->SetTextSize(0.030);
    leg_v0ptv0_ptref2->AddEntry(gr_v0ptv0_ptref_ATLAS_1, "ATLAS 5.02 TeV: 0.5-2 GeV", "pfl");
    leg_v0ptv0_ptref2->AddEntry(gr_v0ptv0_ptref_ATLAS_2, "ATLAS 5.02 TeV: 0.5-5 GeV", "pfl");
    leg_v0ptv0_ptref2->AddEntry(gr_v0ptv0_ptref_ATLAS_3, "ATLAS 5.02 TeV: 1-5 GeV", "pfl");
    leg_v0ptv0_ptref2->SetBorderSize(0);
    leg_v0ptv0_ptref2->SetFillStyle(0);

    auto leg_v0ptv0_label = new TLegend(0.78, 0.9, 0.93, 0.93);
    leg_v0ptv0_label->SetTextSize(0.055);
    leg_v0ptv0_label->AddEntry((TObject*)0, "(b)", "");
    leg_v0ptv0_label->SetBorderSize(0);
    leg_v0ptv0_label->SetFillStyle(0);

    // PLOTING IN IT CD
    c->cd(2);
    gr_v0ptv0_ptref_ATLAS_2_unc->Draw("A2");
    gr_v0ptv0_ptref_ATLAS_1_unc->Draw("2 SAME");
    gr_v0ptv0_ptref_ATLAS_3_unc->Draw("2 SAME");
    gr_v0ptv0_ptref_2_unc->Draw("2 SAME");
    gr_v0ptv0_ptref_1_unc->Draw("2 SAME");
    gr_v0ptv0_ptref_3_unc->Draw("2 SAME");
    gr_v0ptv0_ptref_ATLAS_2->Draw("P SAME");
    gr_v0ptv0_ptref_ATLAS_1->Draw("P SAME");
    gr_v0ptv0_ptref_ATLAS_3->Draw("P SAME");
    gr_v0ptv0_ptref_2->Draw("P SAME");
    gr_v0ptv0_ptref_1->Draw("P SAME");
    gr_v0ptv0_ptref_3->Draw("P SAME");
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
    customize_TGraphErrors(gr_v0pt_ptref_1, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.6, 20, 2, 1.0);
    customize_TGraphErrors(gr_v0pt_ptref_2, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.6, 21, 4, 1.0);
    customize_TGraphErrors(gr_v0pt_ptref_3, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.6, 22, 6, 1.0);

    TGraphErrors *gr_v0pt_ptref_1_unc = (TGraphErrors*)f_unc_ptref1->Get("gr_v0pt_5060_total");
    TGraphErrors *gr_v0pt_ptref_2_unc = (TGraphErrors*)f_unc_ptref2->Get("gr_v0pt_5060_total");
    TGraphErrors *gr_v0pt_ptref_3_unc = (TGraphErrors*)f_unc_ptref3->Get("gr_v0pt_5060_total");

    customize_TGraphErrors(gr_v0pt_ptref_1_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.6, 20, 2, 1.0);
    customize_TGraphErrors(gr_v0pt_ptref_2_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.6, 21, 4, 1.0);
    customize_TGraphErrors(gr_v0pt_ptref_3_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.6, 22, 6, 1.0);

    // READING TGRAPHS FILE FROM ATLAS DATA
    auto p_v0pt_ptref_ATLAS_1 = csvToTGraph("./Data/Figures_ATLAS/v0pt_ptref_1_ATLAS_5060.csv");
        TGraphAsymmErrors *gr_v0pt_ptref_ATLAS_1 = p_v0pt_ptref_ATLAS_1.first;
        TGraphAsymmErrors *gr_v0pt_ptref_ATLAS_1_unc = p_v0pt_ptref_ATLAS_1.second;
    auto p_v0pt_ptref_ATLAS_2 = csvToTGraph("./Data/Figures_ATLAS/v0pt_ptref_2_ATLAS_5060.csv");
        TGraphAsymmErrors *gr_v0pt_ptref_ATLAS_2 = p_v0pt_ptref_ATLAS_2.first;
        TGraphAsymmErrors *gr_v0pt_ptref_ATLAS_2_unc = p_v0pt_ptref_ATLAS_2.second;
    auto p_v0pt_ptref_ATLAS_3 = csvToTGraph("./Data/Figures_ATLAS/v0pt_ptref_3_ATLAS_5060.csv");
        TGraphAsymmErrors *gr_v0pt_ptref_ATLAS_3 = p_v0pt_ptref_ATLAS_3.first;
        TGraphAsymmErrors *gr_v0pt_ptref_ATLAS_3_unc = p_v0pt_ptref_ATLAS_3.second;
    // EDITING ATLAS DATA TGRAPH
    customize_TGraphAsymmErrors(gr_v0pt_ptref_ATLAS_1, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.6, 24, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ptref_ATLAS_2, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.6, 25, 4, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ptref_ATLAS_3, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.6, 26, 6, 1.0);

    customize_TGraphAsymmErrors(gr_v0pt_ptref_ATLAS_1_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.6, 24, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ptref_ATLAS_2_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.6, 25, 4, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ptref_ATLAS_3_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.6, 26, 6, 1.0);

    auto leg_v0pt_ptref_title = new TLegend(0.04, 0.98, 0.5, 0.825);
    leg_v0pt_ptref_title->SetTextSize(0.055);
    leg_v0pt_ptref_title->AddEntry((TObject*)0, "Pb+Pb,   #eta_{gap} = 1", "");
    leg_v0pt_ptref_title->AddEntry((TObject*)0, "50-60% Centrality", "");
    leg_v0pt_ptref_title->SetBorderSize(0);
    leg_v0pt_ptref_title->SetFillStyle(0);

    auto leg_v0pt_ptref = new TLegend(0.14, 0.82, 0.485, 0.68);
    leg_v0pt_ptref->SetTextSize(0.030);
    leg_v0pt_ptref->AddEntry(gr_v0pt_ptref_1, "CMS OpenData 2.76 TeV: 0.5-2 GeV", "pfl");
    leg_v0pt_ptref->AddEntry(gr_v0pt_ptref_2, "CMS OpenData 2.76 TeV: 0.5-5 GeV", "pfl");
    leg_v0pt_ptref->AddEntry(gr_v0pt_ptref_3, "CMS OpenData 2.76 TeV: 1-5 GeV", "pfl");
    leg_v0pt_ptref->SetBorderSize(0);
    leg_v0pt_ptref->SetFillStyle(0);

    auto leg_v0pt_ptref2 = new TLegend(0.47, 0.25, 0.84, 0.12);
    leg_v0pt_ptref2->SetTextSize(0.030);
    leg_v0pt_ptref2->AddEntry(gr_v0pt_ptref_ATLAS_1, "ATLAS 5.02 TeV: 0.5-2 GeV", "pfl");
    leg_v0pt_ptref2->AddEntry(gr_v0pt_ptref_ATLAS_2, "ATLAS 5.02 TeV: 0.5-5 GeV", "pfl");
    leg_v0pt_ptref2->AddEntry(gr_v0pt_ptref_ATLAS_3, "ATLAS 5.02 TeV: 1-5 GeV", "pfl");
    leg_v0pt_ptref2->SetBorderSize(0);
    leg_v0pt_ptref2->SetFillStyle(0);

    auto leg_v0pt_ptref_label = new TLegend(0.78, 0.9, 0.93, 0.93);
    leg_v0pt_ptref_label->SetTextSize(0.055);
    leg_v0pt_ptref_label->AddEntry((TObject*)0, "(a)", "");
    leg_v0pt_ptref_label->SetBorderSize(0);
    leg_v0pt_ptref_label->SetFillStyle(0);

    c->cd(3);
    gr_v0pt_ptref_ATLAS_2_unc->Draw("A2");
    gr_v0pt_ptref_ATLAS_1_unc->Draw("2 SAME");
    gr_v0pt_ptref_ATLAS_3_unc->Draw("2 SAME");
    gr_v0pt_ptref_2_unc->Draw("2 SAME");
    gr_v0pt_ptref_1_unc->Draw("2 SAME");
    gr_v0pt_ptref_3_unc->Draw("2 SAME");
    gr_v0pt_ptref_ATLAS_2->Draw("P SAME");
    gr_v0pt_ptref_ATLAS_1->Draw("P SAME");
    gr_v0pt_ptref_ATLAS_3->Draw("P SAME");
    gr_v0pt_ptref_2->Draw("P SAME");
    gr_v0pt_ptref_1->Draw("P SAME");
    gr_v0pt_ptref_3->Draw("P SAME");
    leg_v0pt_ptref_title->Draw();
    leg_v0pt_ptref->Draw();
    leg_v0pt_ptref2->Draw();
    leg_v0pt_ptref_label->Draw();
    gPad->SetLogx();
    gPad->SetLeftMargin(0.12);
    gPad->SetTopMargin(0.02);

    f_unc_ptref1->Close(); f_unc_ptref2->Close(); f_unc_ptref3->Close();

    // v0(pT) | Etas

    TFile *f_etas = TFile::Open("./Data/Figures/etas.root");
    TGraphErrors *gr_v0pt_eta0 = (TGraphErrors*)f_etas->Get("v0pt_ptref_eta0_6070");
    TGraphErrors *gr_v0pt_eta1 = (TGraphErrors*)f_etas->Get("v0pt_ptref_eta1_6070");
    TGraphErrors *gr_v0pt_eta2 = (TGraphErrors*)f_etas->Get("v0pt_ptref_eta2_6070");
    TGraphErrors *gr_v0pt_eta3 = (TGraphErrors*)f_etas->Get("v0pt_ptref_eta3_6070");
    customize_TGraphErrors(gr_v0pt_eta0, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 20, 2, 1.0);
        gr_v0pt_eta0->SetFillColorAlpha(2, 0.3); gr_v0pt_eta0->SetFillStyle(1001);
    customize_TGraphErrors(gr_v0pt_eta1, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 21, 4, 1.0);
        gr_v0pt_eta1->SetFillColorAlpha(4, 0.3); gr_v0pt_eta1->SetFillStyle(1001);
    customize_TGraphErrors(gr_v0pt_eta2, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 22, 6, 1.0);
        gr_v0pt_eta2->SetFillColorAlpha(6, 0.3); gr_v0pt_eta2->SetFillStyle(1001);
    customize_TGraphErrors(gr_v0pt_eta3, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 34, 209, 1.0);
        gr_v0pt_eta3->SetFillColorAlpha(209, 0.3); gr_v0pt_eta3->SetFillStyle(1001);

    TFile *f_eta0 = TFile::Open("./Data/Systematics/Variations/eta0_6070/SystUncs.root");
    TFile *f_eta1 = TFile::Open("./Data/Systematics/Variations/eta1_6070/SystUncs.root");
    TFile *f_eta2 = TFile::Open("./Data/Systematics/Variations/eta2_6070/SystUncs.root");
    TFile *f_eta3 = TFile::Open("./Data/Systematics/Variations/eta3_6070/SystUncs.root");
    TGraphErrors *gr_v0pt_eta0_unc = (TGraphErrors*)f_eta0->Get("gr_v0pt_6070_total");
    TGraphErrors *gr_v0pt_eta1_unc = (TGraphErrors*)f_eta1->Get("gr_v0pt_6070_total");
    TGraphErrors *gr_v0pt_eta2_unc = (TGraphErrors*)f_eta2->Get("gr_v0pt_6070_total");
    TGraphErrors *gr_v0pt_eta3_unc = (TGraphErrors*)f_eta3->Get("gr_v0pt_6070_total");
    customize_TGraphErrors(gr_v0pt_eta0_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 20, 2, 1.0);
        gr_v0pt_eta0_unc->SetFillColorAlpha(2, 0.3); gr_v0pt_eta0_unc->SetFillStyle(1001);
    customize_TGraphErrors(gr_v0pt_eta1_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 21, 4, 1.0);
        gr_v0pt_eta1_unc->SetFillColorAlpha(4, 0.3); gr_v0pt_eta1_unc->SetFillStyle(1001);
    customize_TGraphErrors(gr_v0pt_eta2_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 22, 6, 1.0);
        gr_v0pt_eta2_unc->SetFillColorAlpha(6, 0.3); gr_v0pt_eta2_unc->SetFillStyle(1001);
    customize_TGraphErrors(gr_v0pt_eta3_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 34, 209, 1.0);
        gr_v0pt_eta3_unc->SetFillColorAlpha(209, 0.3); gr_v0pt_eta3_unc->SetFillStyle(1001);

    auto p_v0pt_ATLAS_eta0 = csvToTGraph("./Data/Figures_ATLAS/v0pt_ATLAS_eta0.csv");
        TGraphAsymmErrors *gr_v0pt_ATLAS_eta0 = p_v0pt_ATLAS_eta0.first;
        TGraphAsymmErrors *gr_v0pt_ATLAS_eta0_unc = p_v0pt_ATLAS_eta0.second;
    auto p_v0pt_ATLAS_eta1 = csvToTGraph("./Data/Figures_ATLAS/v0pt_ATLAS_eta1.csv");
        TGraphAsymmErrors *gr_v0pt_ATLAS_eta1 = p_v0pt_ATLAS_eta1.first;
        TGraphAsymmErrors *gr_v0pt_ATLAS_eta1_unc = p_v0pt_ATLAS_eta1.second;
    auto p_v0pt_ATLAS_eta2 = csvToTGraph("./Data/Figures_ATLAS/v0pt_ATLAS_eta2.csv");
        TGraphAsymmErrors *gr_v0pt_ATLAS_eta2 = p_v0pt_ATLAS_eta2.first;
        TGraphAsymmErrors *gr_v0pt_ATLAS_eta2_unc = p_v0pt_ATLAS_eta2.second;
    auto p_v0pt_ATLAS_eta3 = csvToTGraph("./Data/Figures_ATLAS/v0pt_ATLAS_eta3.csv");
        TGraphAsymmErrors *gr_v0pt_ATLAS_eta3 = p_v0pt_ATLAS_eta3.first;
        TGraphAsymmErrors *gr_v0pt_ATLAS_eta3_unc = p_v0pt_ATLAS_eta3.second;
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_eta0, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 24, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_eta1, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 25, 4, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_eta2, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 26, 6, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_eta3, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 28, 209, 1.0);

    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_eta0_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 24, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_eta1_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 25, 4, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_eta2_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 26, 6, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_eta3_unc, " ; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 28, 209, 1.0);


    auto leg_v0pt_eta_title = new TLegend(0.035, 0.98, 0.5, 0.825);
    leg_v0pt_eta_title->SetTextSize(0.055);
    leg_v0pt_eta_title->AddEntry((TObject*)0, "Pb+Pb,   p_{T}^{ref}: 0.5-2 GeV", "");
    leg_v0pt_eta_title->AddEntry((TObject*)0, "60-70% Centrality", "");
    leg_v0pt_eta_title->SetBorderSize(0);
    leg_v0pt_eta_title->SetFillStyle(0);

    auto leg_v0pt_eta = new TLegend(0.14, 0.82, 0.49, 0.64);
    leg_v0pt_eta->SetTextSize(0.03);
    leg_v0pt_eta->AddEntry(gr_v0pt_eta0, "CMS Open Data 2.76 TeV    #eta_{gap} = 0", "pfl");
    leg_v0pt_eta->AddEntry(gr_v0pt_eta1, "CMS Open Data 2.76 TeV    #eta_{gap} = 1", "pfl");
    leg_v0pt_eta->AddEntry(gr_v0pt_eta2, "CMS Open Data 2.76 TeV    #eta_{gap} = 2", "pfl");
    leg_v0pt_eta->AddEntry(gr_v0pt_eta3, "CMS Open Data 2.76 TeV    #eta_{gap} = 3", "pfl");
    leg_v0pt_eta->SetBorderSize(0);
    leg_v0pt_eta->SetFillStyle(0);

    auto leg_v0pt_eta2 = new TLegend(0.47, 0.29, 0.86, 0.12);
    leg_v0pt_eta2->SetTextSize(0.03);
    leg_v0pt_eta2->AddEntry(gr_v0pt_ATLAS_eta0, "ATLAS 5.02 TeV    #eta_{gap} = 0", "pfl");
    leg_v0pt_eta2->AddEntry(gr_v0pt_ATLAS_eta1, "ATLAS 5.02 TeV    #eta_{gap} = 1", "pfl");
    leg_v0pt_eta2->AddEntry(gr_v0pt_ATLAS_eta2, "ATLAS 5.02 TeV    #eta_{gap} = 2", "pfl");
    leg_v0pt_eta2->AddEntry(gr_v0pt_ATLAS_eta3, "ATLAS 5.02 TeV    #eta_{gap} = 3", "pfl");
    leg_v0pt_eta2->SetBorderSize(0);
    leg_v0pt_eta2->SetFillStyle(0);

    auto leg_v0pt_eta_label = new TLegend(0.78, 0.9, 0.93, 0.93);
    leg_v0pt_eta_label->SetTextSize(0.055);
    leg_v0pt_eta_label->AddEntry((TObject*)0, "(b)", "");
    leg_v0pt_eta_label->SetBorderSize(0);
    leg_v0pt_eta_label->SetFillStyle(0);

    c->cd(4);
    gr_v0pt_ATLAS_eta1_unc->Draw("A2");
    gr_v0pt_ATLAS_eta0_unc->Draw("2 SAME");
    gr_v0pt_ATLAS_eta3_unc->Draw("2 SAME");
    gr_v0pt_ATLAS_eta2_unc->Draw("2 SAME");
    gr_v0pt_eta1_unc->Draw("2 SAME");
    gr_v0pt_eta0_unc->Draw("2 SAME");
    gr_v0pt_eta3_unc->Draw("2 SAME");
    gr_v0pt_eta2_unc->Draw("2 SAME");
    gr_v0pt_ATLAS_eta1->Draw("P SAME");
    gr_v0pt_ATLAS_eta0->Draw("P SAME");
    gr_v0pt_ATLAS_eta3->Draw("P SAME");
    gr_v0pt_ATLAS_eta2->Draw("P SAME");
    gr_v0pt_eta1->Draw("P SAME");
    gr_v0pt_eta0->Draw("P SAME");
    gr_v0pt_eta3->Draw("P SAME");
    gr_v0pt_eta2->Draw("P SAME");
    leg_v0pt_eta_title->Draw();
    leg_v0pt_eta->Draw();
    leg_v0pt_eta2->Draw();
    leg_v0pt_eta_label->Draw();
    gPad->SetLogx();
    gPad->SetLeftMargin(0.12);
    gPad->SetTopMargin(0.02);

    c->Update();
    c->SaveAs("./Plots/PlotRefsEta.pdf");
    delete c;

}

void DoPlotMain(){
    
    TFile *f = TFile::Open("./Data/Figures/main.root", "READ");
    TFile *f_unc = TFile::Open("./Data/Systematics/SystUncs.root", "READ");

    // v0(pT)
    TGraph *gr_v0pt_5060 = (TGraph*)f->Get("v0pt_ptref_1_5060");
    TGraph *gr_v0pt_6070 = (TGraph*)f->Get("v0pt_ptref_1_6070");
    TGraphErrors *gr_v0pt_5060_unc = (TGraphErrors*)f_unc->Get("gr_v0pt_5060_total");
    TGraphErrors *gr_v0pt_6070_unc = (TGraphErrors*)f_unc->Get("gr_v0pt_6070_total");

    auto p_v0pt_ATLAS_5060 = csvToTGraph("./Data/Figures_ATLAS/v0pt_ptref_1_ATLAS_5060.csv");
        TGraphAsymmErrors *gr_v0pt_ATLAS_5060 = p_v0pt_ATLAS_5060.first;
        TGraphAsymmErrors *gr_v0pt_ATLAS_5060_unc = p_v0pt_ATLAS_5060.second;
    auto p_v0pt_ATLAS_6070 = csvToTGraph("./Data/Figures_ATLAS/v0pt_ptref_1_ATLAS_6070.csv");
        TGraphAsymmErrors *gr_v0pt_ATLAS_6070 = p_v0pt_ATLAS_6070.first;
        TGraphAsymmErrors *gr_v0pt_ATLAS_6070_unc = p_v0pt_ATLAS_6070.second;

    customize_TGraph(gr_v0pt_5060, "; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.46, 21, 2, 1.0);
    customize_TGraph(gr_v0pt_6070, "; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.46, 20, 4, 1.0);
    customize_TGraphErrors(gr_v0pt_5060_unc, "; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.46, 21, 2, 1.0);
    customize_TGraphErrors(gr_v0pt_6070_unc, "; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.46, 20, 4, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_5060, "; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.46, 25, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_6070, "; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.46, 24, 4, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_5060_unc, "; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.46, 25, 2, 1.0);
    customize_TGraphAsymmErrors(gr_v0pt_ATLAS_6070_unc, "; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.46, 24, 4, 1.0);

    auto c = new TCanvas("c", "c", 1100, 500);
    c->Divide(2, 1);

    // v0(pT) legend
    auto legend_v0pt_text = new TLegend(0.025, 0.98, 0.5, 0.82);
    legend_v0pt_text->SetTextSize(0.055);
    legend_v0pt_text->AddEntry((TObject*)0, "Pb+Pb,   #eta_{gap} = 1", "");
    legend_v0pt_text->AddEntry((TObject*)0, "p_{T}^{ref} 0.5-2 GeV", "");
    legend_v0pt_text->SetBorderSize(0);
    legend_v0pt_text->SetFillStyle(0);

    auto legend_v0pt_cents = new TLegend(0.12, 0.81, 0.47, 0.61);
    legend_v0pt_cents->SetTextSize(0.034);
    legend_v0pt_cents->AddEntry(gr_v0pt_5060, "CMS OpenData 2.76 TeV (50-60%)", "pfl");
    legend_v0pt_cents->AddEntry(gr_v0pt_6070, "CMS OpenData 2.76 TeV (60-70%)", "pfl");
    legend_v0pt_cents->AddEntry(gr_v0pt_ATLAS_5060, "ATLAS 5.02 TeV (50-60%)", "pfl");
    legend_v0pt_cents->AddEntry(gr_v0pt_ATLAS_6070, "ATLAS 5.02 TeV (60-70%)", "pfl");
    legend_v0pt_cents->SetBorderSize(0);
    legend_v0pt_cents->SetFillStyle(0);

    auto legend_v0pt_label = new TLegend(0.78, 0.9, 0.93, 0.93);
    legend_v0pt_label->SetTextSize(0.055);
    legend_v0pt_label->AddEntry((TObject*)0, "(a)", "");
    legend_v0pt_label->SetBorderSize(0);
    legend_v0pt_label->SetFillStyle(0);

    // Drawing v0(pT) plot
    c->cd(1);
    gr_v0pt_ATLAS_5060_unc->Draw("A2");
    gr_v0pt_ATLAS_6070_unc->Draw("2 SAME");
    gr_v0pt_5060_unc->Draw("2 SAME");
    gr_v0pt_6070_unc->Draw("2 SAME");
    gr_v0pt_ATLAS_5060->Draw("P SAME");
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
    TGraphErrors *gr_sv0pt_5060_unc = (TGraphErrors*)f_unc->Get("gr_sv0pt_5060_total");
    TGraphErrors *gr_sv0pt_6070_unc = (TGraphErrors*)f_unc->Get("gr_sv0pt_6070_total");

    auto p_sv0pt_ATLAS_5060 = csvToTGraph("./Data/Figures_ATLAS/sv0pt_ptref_1_ATLAS_5060.csv");
        TGraphAsymmErrors *gr_sv0pt_ATLAS_5060 = p_sv0pt_ATLAS_5060.first;
        TGraphAsymmErrors *gr_sv0pt_ATLAS_5060_unc = p_sv0pt_ATLAS_5060.second;
    auto p_sv0pt_ATLAS_6070 = csvToTGraph("./Data/Figures_ATLAS/sv0pt_ptref_1_ATLAS_6070.csv");
        TGraphAsymmErrors *gr_sv0pt_ATLAS_6070 = p_sv0pt_ATLAS_6070.first;
        TGraphAsymmErrors *gr_sv0pt_ATLAS_6070_unc = p_sv0pt_ATLAS_6070.second;

    customize_TGraph(gr_sv0pt_5060, "; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 23.7, 21, 2, 1.0);
    customize_TGraph(gr_sv0pt_6070, "; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 23.7, 20, 4, 1.0);
    customize_TGraphErrors(gr_sv0pt_5060_unc, "; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 23.7, 21, 2, 1.0);
    customize_TGraphErrors(gr_sv0pt_6070_unc, "; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 23.7, 20, 4, 1.0);
    customize_TGraphAsymmErrors(gr_sv0pt_ATLAS_5060, "; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 23.7, 25, 2, 1.0);
    customize_TGraphAsymmErrors(gr_sv0pt_ATLAS_6070, "; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 23.7, 24, 4, 1.0);
    customize_TGraphAsymmErrors(gr_sv0pt_ATLAS_5060_unc, "; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 23.7, 25, 2, 1.0);
    customize_TGraphAsymmErrors(gr_sv0pt_ATLAS_6070_unc, "; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 23.7, 24, 4, 1.0);

    // v0(pT)/v0 legend
    auto legend_sv0pt_text = new TLegend(0.025, 0.98, 0.5, 0.82);
    legend_sv0pt_text->SetTextSize(0.055);
    legend_sv0pt_text->AddEntry((TObject*)0, "Pb+Pb,   #eta_{gap} = 1", "");
    legend_sv0pt_text->AddEntry((TObject*)0, "p_{T}^{ref} 0.5-2 GeV", "");
    legend_sv0pt_text->SetBorderSize(0);
    legend_sv0pt_text->SetFillStyle(0);

    auto legend_sv0pt_cents = new TLegend(0.12, 0.81, 0.47, 0.61);
    legend_sv0pt_cents->SetTextSize(0.034);
    legend_sv0pt_cents->AddEntry(gr_sv0pt_5060, "CMS OpenData 2.76 TeV (50-60%)", "pfl");
    legend_sv0pt_cents->AddEntry(gr_sv0pt_6070, "CMS OpenData 2.76 TeV (60-70%)", "pfl");
    legend_sv0pt_cents->AddEntry(gr_sv0pt_ATLAS_5060, "ATLAS 5.02 TeV (50-60%)", "pfl");
    legend_sv0pt_cents->AddEntry(gr_sv0pt_ATLAS_6070, "ATLAS 5.02 TeV (60-70%)", "pfl");
    legend_sv0pt_cents->SetBorderSize(0);
    legend_sv0pt_cents->SetFillStyle(0);

    auto legend_sv0pt_label = new TLegend(0.78, 0.9, 0.93, 0.93);
    legend_sv0pt_label->SetTextSize(0.055);
    legend_sv0pt_label->AddEntry((TObject*)0, "(b)", "");
    legend_sv0pt_label->SetBorderSize(0);
    legend_sv0pt_label->SetFillStyle(0);
   
    // Drawing v0(pT)/v0 plot
    c->cd(2);
    gr_sv0pt_ATLAS_5060_unc->Draw("A2");
    gr_sv0pt_ATLAS_6070_unc->Draw("2 SAME");
    gr_sv0pt_5060_unc->Draw("2 SAME");
    gr_sv0pt_6070_unc->Draw("2 SAME");
    gr_sv0pt_ATLAS_5060->Draw("P SAME");
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
    c->SaveAs("./Plots/PlotMain.pdf");
    delete c;
}

void DoPlotTrkSelectSyst(){

    TFile *f = TFile::Open("./Data/Systematics/TrackSelectionUnc.root");

    // v0 plot
        TGraphErrors* gd_v0_loose = (TGraphErrors*)f->Get("gd_v0_loose");
        TGraphErrors* gd_v0_tight = (TGraphErrors*)f->Get("gd_v0_tight");

        customize_TGraphErrors(gd_v0_loose, "Track Selection v_{0}; Centrality [%]; Diff", 50.0, 70.0, -500e-6, 700e-6, 20, 2, 1.0);
        customize_TGraphErrors(gd_v0_tight, "Track Selection v_{0}; Centrality [%]; Diff", 50.0, 70.0, -500e-6, 700e-6, 21, 4, 1.0);

        auto c_v0 = new TCanvas("c_v0", "c_v0", 500, 500);

        auto l_v0 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_v0->SetTextSize(0.034);
        l_v0->AddEntry(gd_v0_loose, "Loose", "lp");
        l_v0->AddEntry(gd_v0_tight, "Tight", "lp");
        l_v0->SetBorderSize(0);
        l_v0->SetFillStyle(0);

        gd_v0_loose->Draw("ALP");
        gd_v0_tight->Draw("LP SAME");
        l_v0->Draw();
        //gPad->SetLogy();

        c_v0->Update();
        c_v0->SaveAs("./Plots/Systematics/trackselection_v0.pdf");

    // v0(pT) plot
        TGraphErrors* gd_v0pt_5060_loose = (TGraphErrors*)f->Get("gd_v0pt_5060_loose");
        TGraphErrors* gd_v0pt_6070_loose = (TGraphErrors*)f->Get("gd_v0pt_6070_loose");
        TGraphErrors* gd_v0pt_5060_tight = (TGraphErrors*)f->Get("gd_v0pt_5060_tight");
        TGraphErrors* gd_v0pt_6070_tight = (TGraphErrors*)f->Get("gd_v0pt_6070_tight");

        customize_TGraphErrors(gd_v0pt_5060_loose, "Track Selection v_{0}(p_{T}) 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.008, 0.012, 20, 2, 1.0);
        customize_TGraphErrors(gd_v0pt_6070_loose, "Track Selection v_{0}(p_{T}) 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.02, 0.02, 20, 2, 1.0);
        customize_TGraphErrors(gd_v0pt_5060_tight, "Track Selection v_{0}(p_{T}) 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.008, 0.012, 21, 4, 1.0);
        customize_TGraphErrors(gd_v0pt_6070_tight, "Track Selection v_{0}(p_{T}) 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.02, 0.02, 21, 4, 1.0);

        auto c_v0pt = new TCanvas("c_v0pt", "c_v0pt", 1100, 500);
        c_v0pt->Divide(2, 1);
        
        c_v0pt->cd(1);
        auto l_v0pt_5060 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_v0pt_5060->SetTextSize(0.034);
        l_v0pt_5060->AddEntry(gd_v0pt_5060_loose, "Loose", "lp");
        l_v0pt_5060->AddEntry(gd_v0pt_5060_tight, "Tight", "lp");
        l_v0pt_5060->SetBorderSize(0);
        l_v0pt_5060->SetFillStyle(0);

        gd_v0pt_5060_loose->Draw("ALP");
        gd_v0pt_5060_tight->Draw("LP SAME");
        l_v0pt_5060->Draw();
        gPad->SetLogx();

        c_v0pt->cd(2);
        auto l_v0pt_6070 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_v0pt_6070->SetTextSize(0.034);
        l_v0pt_6070->AddEntry(gd_v0pt_6070_loose, "Loose", "lp");
        l_v0pt_6070->AddEntry(gd_v0pt_6070_tight, "Tight", "lp");
        l_v0pt_6070->SetBorderSize(0);
        l_v0pt_6070->SetFillStyle(0);

        

        gd_v0pt_6070_loose->Draw("ALP");
        gd_v0pt_6070_tight->Draw("LP SAME");
        l_v0pt_6070->Draw();
        gPad->SetLogx();

        c_v0pt->Update();
        c_v0pt->SaveAs("./Plots/Systematics/trackselection_v0pt.pdf");

        // v0(pT)/v0 plot
        TGraphErrors* gd_sv0pt_5060_loose = (TGraphErrors*)f->Get("gd_sv0pt_5060_loose");
        TGraphErrors* gd_sv0pt_6070_loose = (TGraphErrors*)f->Get("gd_sv0pt_6070_loose");
        TGraphErrors* gd_sv0pt_5060_tight = (TGraphErrors*)f->Get("gd_sv0pt_5060_tight");
        TGraphErrors* gd_sv0pt_6070_tight = (TGraphErrors*)f->Get("gd_sv0pt_6070_tight");

        customize_TGraphErrors(gd_sv0pt_5060_loose, "Track Selection v_{0}(p_{T})/v_{0} 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.5, 0.7, 20, 2, 1.0);
        customize_TGraphErrors(gd_sv0pt_6070_loose, "Track Selection v_{0}(p_{T})/v_{0} 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.8, 1.05, 20, 2, 1.0);
        customize_TGraphErrors(gd_sv0pt_5060_tight, "Track Selection v_{0}(p_{T})/v_{0} 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.5, 0.7, 21, 4, 1.0);
        customize_TGraphErrors(gd_sv0pt_6070_tight, "Track Selection v_{0}(p_{T})/v_{0} 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.8, 1.05, 21, 4, 1.0);

        auto c_sv0pt = new TCanvas("c_sv0pt", "c_sv0pt", 1100, 500);
        c_sv0pt->Divide(2, 1);
        
        c_sv0pt->cd(1);
        auto l_sv0pt_5060 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_sv0pt_5060->SetTextSize(0.034);
        l_sv0pt_5060->AddEntry(gd_sv0pt_5060_loose, "Loose", "lp");
        l_sv0pt_5060->AddEntry(gd_sv0pt_5060_tight, "Tight", "lp");
        l_sv0pt_5060->SetBorderSize(0);
        l_sv0pt_5060->SetFillStyle(0);

        gd_sv0pt_5060_loose->Draw("ALP");
        gd_sv0pt_5060_tight->Draw("LP SAME");
        l_sv0pt_5060->Draw();
        gPad->SetLogx();

        c_sv0pt->cd(2);
        auto l_sv0pt_6070 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_sv0pt_6070->SetTextSize(0.034);
        l_sv0pt_6070->AddEntry(gd_sv0pt_6070_loose, "Loose", "lp");
        l_sv0pt_6070->AddEntry(gd_sv0pt_6070_tight, "Tight", "lp");
        l_sv0pt_6070->SetBorderSize(0);
        l_sv0pt_6070->SetFillStyle(0);

        gd_sv0pt_6070_loose->Draw("ALP");
        gd_sv0pt_6070_tight->Draw("LP SAME");
        l_sv0pt_6070->Draw();
        gPad->SetLogx();

        c_sv0pt->Update();
        c_sv0pt->SaveAs("./Plots/Systematics/trackselection_sv0pt.pdf");
}

void DoPlotCentFluctSyst(){

    TFile *f = TFile::Open("./Data/Systematics/CentralityFluctuationUnc.root");

    // v0 plot
        TGraphErrors* gd_v0_positive = (TGraphErrors*)f->Get("gd_v0_positive");
        TGraphErrors* gd_v0_negative = (TGraphErrors*)f->Get("gd_v0_negative");

        customize_TGraphErrors(gd_v0_positive, "Centrality Fluctuation v_{0}; Centrality [%]; Diff", 50.0, 70.0, -0.3e-3, 0.45e-3, 20, 2, 1.0);
        customize_TGraphErrors(gd_v0_negative, "Centrality Fluctuation v_{0}; Centrality [%]; Diff", 50.0, 70.0, -0.3e-3, 0.45e-3, 21, 4, 1.0);

        auto c_v0 = new TCanvas("c_v0", "c_v0", 500, 500);

        auto l_v0 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_v0->SetTextSize(0.034);
        l_v0->AddEntry(gd_v0_positive, "Positive", "lp");
        l_v0->AddEntry(gd_v0_negative, "Negative", "lp");
        l_v0->SetBorderSize(0);
        l_v0->SetFillStyle(0);

        gd_v0_positive->Draw("ALP");
        gd_v0_negative->Draw("LP SAME");
        l_v0->Draw();
        //gPad->SetLogy();

        c_v0->Update();
        c_v0->SaveAs("./Plots/Systematics/centralityfluctuation_v0.pdf");

    // v0(pT) plot
        TGraphErrors* gd_v0pt_5060_positive = (TGraphErrors*)f->Get("gd_v0pt_5060_positive");
        TGraphErrors* gd_v0pt_6070_positive = (TGraphErrors*)f->Get("gd_v0pt_6070_positive");
        TGraphErrors* gd_v0pt_5060_negative = (TGraphErrors*)f->Get("gd_v0pt_5060_negative");
        TGraphErrors* gd_v0pt_6070_negative = (TGraphErrors*)f->Get("gd_v0pt_6070_negative");

        customize_TGraph(gd_v0pt_5060_positive, "Centrality Fluctuation v_{0}(p_{T}) 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.007, 0.01, 20, 2, 1.0);
        customize_TGraph(gd_v0pt_6070_positive, "Centrality Fluctuation v_{0}(p_{T}) 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.015, 0.0175, 20, 2, 1.0);
        customize_TGraph(gd_v0pt_5060_negative, "Centrality Fluctuation v_{0}(p_{T}) 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.007, 0.01, 21, 4, 1.0);
        customize_TGraph(gd_v0pt_6070_negative, "Centrality Fluctuation v_{0}(p_{T}) 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.015, 0.0175, 21, 4, 1.0);

        auto c_v0pt = new TCanvas("c_v0pt", "c_v0pt", 1100, 500);
        c_v0pt->Divide(2, 1);
        
        c_v0pt->cd(1);
        auto l_v0pt_5060 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_v0pt_5060->SetTextSize(0.034);
        l_v0pt_5060->AddEntry(gd_v0pt_5060_positive, "Positive", "lp");
        l_v0pt_5060->AddEntry(gd_v0pt_5060_negative, "Negative", "lp");
        l_v0pt_5060->SetBorderSize(0);
        l_v0pt_5060->SetFillStyle(0);

        gd_v0pt_5060_positive->Draw("ALP");
        gd_v0pt_5060_negative->Draw("LP SAME");
        l_v0pt_5060->Draw();
        gPad->SetLogx();

        c_v0pt->cd(2);
        auto l_v0pt_6070 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_v0pt_6070->SetTextSize(0.034);
        l_v0pt_6070->AddEntry(gd_v0pt_6070_positive, "Positive", "lp");
        l_v0pt_6070->AddEntry(gd_v0pt_6070_negative, "Negative", "lp");
        l_v0pt_6070->SetBorderSize(0);
        l_v0pt_6070->SetFillStyle(0);

        gd_v0pt_6070_positive->Draw("ALP");
        gd_v0pt_6070_negative->Draw("LP SAME");
        l_v0pt_6070->Draw();
        gPad->SetLogx();

        c_v0pt->Update();
        c_v0pt->SaveAs("./Plots/Systematics/centralityfluctuation_v0pt.pdf");

        // v0(pT)/v0 plot
        TGraphErrors* gd_sv0pt_5060_positive = (TGraphErrors*)f->Get("gd_sv0pt_5060_positive");
        TGraphErrors* gd_sv0pt_6070_positive = (TGraphErrors*)f->Get("gd_sv0pt_6070_positive");
        TGraphErrors* gd_sv0pt_5060_negative = (TGraphErrors*)f->Get("gd_sv0pt_5060_negative");
        TGraphErrors* gd_sv0pt_6070_negative = (TGraphErrors*)f->Get("gd_sv0pt_6070_negative");

        customize_TGraphErrors(gd_sv0pt_5060_positive, "Centrality Fluctuation v_{0}(p_{T})/v_{0} 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.425, 0.58, 20, 2, 1.0);
        customize_TGraphErrors(gd_sv0pt_6070_positive, "Centrality Fluctuation v_{0}(p_{T})/v_{0} 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.75, 1.0, 20, 2, 1.0);
        customize_TGraphErrors(gd_sv0pt_5060_negative, "Centrality Fluctuation v_{0}(p_{T})/v_{0} 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.425, 0.58, 21, 4, 1.0);
        customize_TGraphErrors(gd_sv0pt_6070_negative, "Centrality Fluctuation v_{0}(p_{T})/v_{0} 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.75, 1.0, 21, 4, 1.0);

        auto c_sv0pt = new TCanvas("c_sv0pt", "c_sv0pt", 1100, 500);
        c_sv0pt->Divide(2, 1);
        
        c_sv0pt->cd(1);
        auto l_sv0pt_5060 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_sv0pt_5060->SetTextSize(0.034);
        l_sv0pt_5060->AddEntry(gd_sv0pt_5060_positive, "Positive", "lp");
        l_sv0pt_5060->AddEntry(gd_sv0pt_5060_negative, "Negative", "lp");
        l_sv0pt_5060->SetBorderSize(0);
        l_sv0pt_5060->SetFillStyle(0);

        gd_sv0pt_5060_positive->Draw("ALP");
        gd_sv0pt_5060_negative->Draw("LP SAME");
        l_sv0pt_5060->Draw();
        gPad->SetLogx();

        c_sv0pt->cd(2);
        auto l_sv0pt_6070 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_sv0pt_6070->SetTextSize(0.034);
        l_sv0pt_6070->AddEntry(gd_sv0pt_6070_positive, "Positive", "lp");
        l_sv0pt_6070->AddEntry(gd_sv0pt_6070_negative, "Negative", "lp");
        l_sv0pt_6070->SetBorderSize(0);
        l_sv0pt_6070->SetFillStyle(0);

        gd_sv0pt_6070_positive->Draw("ALP");
        gd_sv0pt_6070_negative->Draw("LP SAME");
        l_sv0pt_6070->Draw();
        gPad->SetLogx();

        c_sv0pt->Update();
        c_sv0pt->SaveAs("./Plots/Systematics/centralityfluctuation_sv0pt.pdf");
}

void DoPlotCorrApplSyst(){

    TFile *f = TFile::Open("./Data/Systematics/CorrectionApplicationUnc.root");

    // v0 plot
        TGraphErrors* gd_v0_nocorrec = (TGraphErrors*)f->Get("gd_v0_nocorrec");

        customize_TGraph(gd_v0_nocorrec, "Correction Application v_{0}; Centrality [%]; Diff", 50.0, 70.0, 10e-6, 55e-6, 20, 2, 1.0);

        auto c_v0 = new TCanvas("c_v0", "c_v0", 500, 500);

        gd_v0_nocorrec->Draw("ALP");

        c_v0->Update();
        c_v0->SaveAs("./Plots/Systematics/correctionapplication_v0.pdf");

    // v0(pT) plot
        TGraphErrors* gd_v0pt_5060_nocorrec = (TGraphErrors*)f->Get("gd_v0pt_5060_nocorrec");
        TGraphErrors* gd_v0pt_6070_nocorrec = (TGraphErrors*)f->Get("gd_v0pt_6070_nocorrec");

        customize_TGraph(gd_v0pt_5060_nocorrec, "Correction Application v_{0}(p_{T}) 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, 0.0, 1.5e-3, 20, 2, 1.0);
        customize_TGraph(gd_v0pt_6070_nocorrec, "Correction Application v_{0}(p_{T}) 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, 0.0, 1.6e-3, 20, 2, 1.0);

        auto c_v0pt = new TCanvas("c_v0pt", "c_v0pt", 1100, 500);
        c_v0pt->Divide(2, 1);
        
        c_v0pt->cd(1);
        gd_v0pt_5060_nocorrec->Draw("ALP");
        gPad->SetLogx();

        c_v0pt->cd(2);
        gd_v0pt_6070_nocorrec->Draw("ALP");
        gPad->SetLogx();

        c_v0pt->Update();
        c_v0pt->SaveAs("./Plots/Systematics/correctionapplication_v0pt.pdf");

        // v0(pT)/v0 plot
        TGraphErrors* gd_sv0pt_5060_nocorrec = (TGraphErrors*)f->Get("gd_sv0pt_5060_nocorrec");
        TGraphErrors* gd_sv0pt_6070_nocorrec = (TGraphErrors*)f->Get("gd_sv0pt_6070_nocorrec");

        customize_TGraph(gd_sv0pt_5060_nocorrec, "Correction Application v_{0}(p_{T})/v_{0} 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, 0.0, 0.06, 20, 2, 1.0);
        customize_TGraph(gd_sv0pt_6070_nocorrec, "Correction Application v_{0}(p_{T})/v_{0} 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, 0.0, 0.1, 20, 2, 1.0);

        auto c_sv0pt = new TCanvas("c_sv0pt", "c_sv0pt", 1100, 500);
        c_sv0pt->Divide(2, 1);
        
        c_sv0pt->cd(1);
        gd_sv0pt_5060_nocorrec->Draw("ALP");
        gPad->SetLogx();

        c_sv0pt->cd(2);
        gd_sv0pt_6070_nocorrec->Draw("ALP");
        gPad->SetLogx();

        c_sv0pt->Update();
        c_sv0pt->SaveAs("./Plots/Systematics/correctionapplication_sv0pt.pdf");
}

void DoPlotZvertexSyst(){

    TFile *f = TFile::Open("./Data/Systematics/ZvertexPositionUnc.root");

    // v0 plot
        TGraphErrors* gd_v0_pvZ_3 = (TGraphErrors*)f->Get("gd_v0_pvZ_3");
        TGraphErrors* gd_v0_pvZ_3_15 = (TGraphErrors*)f->Get("gd_v0_pvZ_3_15");

        customize_TGraph(gd_v0_pvZ_3, "Z Vertex Position v_{0}; Centrality [%]; Diff", 50.0, 70.0, -0.0017, 0.001, 20, 2, 1.0);
        customize_TGraph(gd_v0_pvZ_3_15, "Z Vertex Position v_{0}; Centrality [%]; Diff", 50.0, 70.0, -0.0017, 0.001, 21, 4, 1.0);

        auto c_v0 = new TCanvas("c_v0", "c_v0", 500, 500);

        auto l_v0 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_v0->SetTextSize(0.034);
        l_v0->AddEntry(gd_v0_pvZ_3, "pvZ_3", "lp");
        l_v0->AddEntry(gd_v0_pvZ_3_15, "pvZ_3_15", "lp");
        l_v0->SetBorderSize(0);
        l_v0->SetFillStyle(0);

        gd_v0_pvZ_3->Draw("ALP");
        gd_v0_pvZ_3_15->Draw("LP SAME");
        //gPad->SetLogy();

        c_v0->Update();
        c_v0->SaveAs("./Plots/Systematics/zvertexposition_v0.pdf");

    // v0(pT) plot
        TGraphErrors* gd_v0pt_5060_pvZ_3 = (TGraphErrors*)f->Get("gd_v0pt_5060_pvZ_3");
        TGraphErrors* gd_v0pt_6070_pvZ_3 = (TGraphErrors*)f->Get("gd_v0pt_6070_pvZ_3");
        TGraphErrors* gd_v0pt_5060_pvZ_3_15 = (TGraphErrors*)f->Get("gd_v0pt_5060_pvZ_3_15");
        TGraphErrors* gd_v0pt_6070_pvZ_3_15 = (TGraphErrors*)f->Get("gd_v0pt_6070_pvZ_3_15");

        customize_TGraph(gd_v0pt_5060_pvZ_3, "Z Vertex Position v_{0}(p_{T}) 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.027, 0.027, 20, 2, 1.0);
        customize_TGraph(gd_v0pt_6070_pvZ_3, "Z Vertex Position v_{0}(p_{T}) 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.046, 0.046, 20, 2, 1.0);
        customize_TGraph(gd_v0pt_5060_pvZ_3_15, "Z Vertex Position v_{0}(p_{T}) 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.027, 0.027, 21, 4, 1.0);
        customize_TGraph(gd_v0pt_6070_pvZ_3_15, "Z Vertex Position v_{0}(p_{T}) 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, -0.046, 0.046, 21, 4, 1.0);

        auto c_v0pt = new TCanvas("c_v0pt", "c_v0pt", 1100, 500);
        c_v0pt->Divide(2, 1);
        
        c_v0pt->cd(1);
        auto l_v0pt_5060 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_v0pt_5060->SetTextSize(0.034);
        l_v0pt_5060->AddEntry(gd_v0pt_5060_pvZ_3, "pvZ_3", "lp");
        l_v0pt_5060->AddEntry(gd_v0pt_5060_pvZ_3_15, "pvZ_3_15", "lp");
        l_v0pt_5060->SetBorderSize(0);
        l_v0pt_5060->SetFillStyle(0);

        gd_v0pt_5060_pvZ_3->Draw("ALP");
        gd_v0pt_5060_pvZ_3_15->Draw("LP SAME");
        l_v0pt_5060->Draw();
        gPad->SetLogx();

        c_v0pt->cd(2);
        auto l_v0pt_6070 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_v0pt_6070->SetTextSize(0.034);
        l_v0pt_6070->AddEntry(gd_v0pt_6070_pvZ_3, "pvZ_3", "lp");
        l_v0pt_6070->AddEntry(gd_v0pt_6070_pvZ_3_15, "pvZ_3_15", "lp");
        l_v0pt_6070->SetBorderSize(0);
        l_v0pt_6070->SetFillStyle(0);

        gd_v0pt_6070_pvZ_3->Draw("ALP");
        gd_v0pt_6070_pvZ_3_15->Draw("LP SAME");
        l_v0pt_6070->Draw();
        gPad->SetLogx();

        c_v0pt->Update();
        c_v0pt->SaveAs("./Plots/Systematics/zvertexposition_v0pt.pdf");

        // v0(pT)/v0 plot
        TGraphErrors* gd_sv0pt_5060_pvZ_3 = (TGraphErrors*)f->Get("gd_sv0pt_5060_pvZ_3");
        TGraphErrors* gd_sv0pt_6070_pvZ_3 = (TGraphErrors*)f->Get("gd_sv0pt_6070_pvZ_3");
        TGraphErrors* gd_sv0pt_5060_pvZ_3_15 = (TGraphErrors*)f->Get("gd_sv0pt_5060_pvZ_3_15");
        TGraphErrors* gd_sv0pt_6070_pvZ_3_15 = (TGraphErrors*)f->Get("gd_sv0pt_6070_pvZ_3_15");


        customize_TGraph(gd_sv0pt_5060_pvZ_3, "Z Vertex Position v_{0}(p_{T})/v_{0} 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, -1.8, 1.8, 20, 2, 1.0);
        customize_TGraph(gd_sv0pt_6070_pvZ_3, "Z Vertex Position v_{0}(p_{T})/v_{0} 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, -2.5, 2.5, 20, 2, 1.0);
        customize_TGraph(gd_sv0pt_5060_pvZ_3_15, "Z Vertex Position v_{0}(p_{T})/v_{0} 50-60%; p_{T} [GeV]; Diff", 0.0, 10.0, -1.8, 1.8, 21, 4, 1.0);
        customize_TGraph(gd_sv0pt_6070_pvZ_3_15, "Z Vertex Position v_{0}(p_{T})/v_{0} 60-70%; p_{T} [GeV]; Diff", 0.0, 10.0, -2.5, 2.5, 21, 4, 1.0);

        auto c_sv0pt = new TCanvas("c_sv0pt", "c_sv0pt", 1100, 500);
        c_sv0pt->Divide(2, 1);
        
        c_sv0pt->cd(1);
        auto l_sv0pt_5060 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_sv0pt_5060->SetTextSize(0.034);
        l_sv0pt_5060->AddEntry(gd_sv0pt_5060_pvZ_3, "pvZ_3", "lp");
        l_sv0pt_5060->AddEntry(gd_sv0pt_5060_pvZ_3_15, "pvZ_3_15", "lp");
        l_sv0pt_5060->SetBorderSize(0);
        l_sv0pt_5060->SetFillStyle(0);

        gd_sv0pt_5060_pvZ_3->Draw("ALP");
        gd_sv0pt_5060_pvZ_3_15->Draw("LP SAME");
        l_sv0pt_5060->Draw();
        gPad->SetLogx();

        c_sv0pt->cd(2);
        auto l_sv0pt_6070 = new TLegend(0.103, 0.81, 0.5, 0.61);
        l_sv0pt_6070->SetTextSize(0.034);
        l_sv0pt_6070->AddEntry(gd_sv0pt_6070_pvZ_3, "pvZ_3", "lp");
        l_sv0pt_6070->AddEntry(gd_sv0pt_6070_pvZ_3_15, "pvZ_3_15", "lp");
        l_sv0pt_6070->SetBorderSize(0);
        l_sv0pt_6070->SetFillStyle(0);

        gd_sv0pt_6070_pvZ_3->Draw("ALP");
        gd_sv0pt_6070_pvZ_3_15->Draw("LP SAME");
        l_sv0pt_6070->Draw();
        gPad->SetLogx();

        c_sv0pt->Update();
        c_sv0pt->SaveAs("./Plots/Systematics/zvertexposition_sv0pt.pdf");
}

void DoPlot_Syst_v0(){

    int N_v0 = 2;
    vector<Double_t> vec_x_v0(N_v0, 0.0);
    vec_x_v0[0] = 55.0; vec_x_v0[1] = 65.0;

    TFile *f_main = TFile::Open("./Data/Figures/main.root");
    TFile *f = TFile::Open("./Data/Systematics/SystUncs.root");

    // Different types
    auto c_v0_vars = new TCanvas("c_v0_vars", "c_v0_vars", 725, 600);

    TGraphErrors *gr_v0_corr = (TGraphErrors*)f->Get("gr_v0_corr");
    TGraphErrors *gr_v0_trk = (TGraphErrors*)f->Get("gr_v0_trk");
    TGraphErrors *gr_v0_cent = (TGraphErrors*)f->Get("gr_v0_cent");
    TGraphErrors *gr_v0_pvZ = (TGraphErrors*)f->Get("gr_v0_pvZ");

    Double_t *ey_v0_corr = gr_v0_corr->GetEY();
    Double_t *ey_v0_trk = gr_v0_trk->GetEY();
    Double_t *ey_v0_cent = gr_v0_cent->GetEY();
    Double_t *ey_v0_pvZ = gr_v0_pvZ->GetEY();

    vector<Double_t> vec_ey_v0_corr(N_v0, 0.0);
    vector<Double_t> vec_ey_v0_trk(N_v0, 0.0);
    vector<Double_t> vec_ey_v0_cent(N_v0, 0.0);
    vector<Double_t> vec_ey_v0_pvZ(N_v0, 0.0);

    vec_ey_v0_corr.assign(ey_v0_corr, ey_v0_corr + N_v0);
    vec_ey_v0_trk.assign(ey_v0_trk, ey_v0_trk + N_v0);
    vec_ey_v0_cent.assign(ey_v0_cent, ey_v0_cent + N_v0);
    vec_ey_v0_pvZ.assign(ey_v0_pvZ, ey_v0_pvZ + N_v0);

    TGraph *gr_ey_v0_corr = new TGraph(N_v0, vec_x_v0.data(), vec_ey_v0_corr.data());
    TGraph *gr_ey_v0_trk = new TGraph(N_v0, vec_x_v0.data(), vec_ey_v0_trk.data());
    TGraph *gr_ey_v0_cent = new TGraph(N_v0, vec_x_v0.data(), vec_ey_v0_cent.data());
    TGraph *gr_ey_v0_pvZ = new TGraph(N_v0, vec_x_v0.data(), vec_ey_v0_pvZ.data());

    customize_TGraph(gr_ey_v0_corr, "v_{0}; Centrality [%]; Uncertainty", 50, 70, 0, 1.8e-4, 20, 51, 1.0);
    customize_TGraph(gr_ey_v0_trk, "v_{0}; Centrality [%]; Uncertainty", 50, 70, 0, 1.8e-4, 20, 62, 1.0);
    customize_TGraph(gr_ey_v0_cent, "v_{0}; Centrality [%]; Uncertainty", 50, 70, 0, 1.8e-4, 20, 76, 1.0);
    customize_TGraph(gr_ey_v0_pvZ, "v_{0}; Centrality [%]; Uncertainty", 50, 70, 0, 1.8e-4, 20, 95, 1.0);

    auto l_v0_vars = new TLegend(0.103, 0.81, 0.5, 0.61);
    l_v0_vars->SetTextSize(0.034);
    l_v0_vars->AddEntry(gr_ey_v0_corr, "Corrections", "lp");
    l_v0_vars->AddEntry(gr_ey_v0_trk, "Track Select.", "lp");
    l_v0_vars->AddEntry(gr_ey_v0_cent, "Centrality Fluct.", "lp");
    l_v0_vars->AddEntry(gr_ey_v0_pvZ, "Z Vertex Pos.", "lp");
    l_v0_vars->SetBorderSize(0);
    l_v0_vars->SetFillStyle(0);

    gr_ey_v0_corr->Draw("ALP");
    gr_ey_v0_trk->Draw("LP");
    gr_ey_v0_cent->Draw("LP");
    gr_ey_v0_pvZ->Draw("LP");
    l_v0_vars->Draw();

    c_v0_vars->SaveAs("./Plots/SystematicsErrors/v0_vars.pdf");
}

void DoPlot_Syst_v0pt(){

    TFile *f_main = TFile::Open("./Data/Figures/main.root");
    TFile *f = TFile::Open("./Data/Systematics/SystUncs.root");

    // Different types
    auto c_v0pt_vars = new TCanvas("c_v0pt_vars", "c_v0pt_vars", 1100, 500);
    c_v0pt_vars->Divide(2,1);

    c_v0pt_vars->cd(1);
    TGraphAsymmErrors *gr_v0pt_5060_corr = (TGraphAsymmErrors*)f->Get("gr_v0pt_5060_corr");
    TGraphAsymmErrors *gr_v0pt_5060_trk = (TGraphAsymmErrors*)f->Get("gr_v0pt_5060_trk");
    TGraphAsymmErrors *gr_v0pt_5060_cent = (TGraphAsymmErrors*)f->Get("gr_v0pt_5060_cent");
    TGraphAsymmErrors *gr_v0pt_5060_pvZ = (TGraphAsymmErrors*)f->Get("gr_v0pt_5060_pvZ");

    Int_t N_v0pt_5060 = gr_v0pt_5060_corr->GetN();
    Double_t *x_v0pt_5060 = gr_v0pt_5060_corr->GetX();
    Double_t *ey_v0pt_5060_corr = gr_v0pt_5060_corr->GetEYlow();
    Double_t *ey_v0pt_5060_trk = gr_v0pt_5060_trk->GetEYlow();
    Double_t *ey_v0pt_5060_cent = gr_v0pt_5060_cent->GetEYlow();
    Double_t *ey_v0pt_5060_pvZ = gr_v0pt_5060_pvZ->GetEYlow();

    vector<Double_t> vec_x_v0pt_5060(N_v0pt_5060, 0.0);
    vector<Double_t> vec_ey_v0pt_5060_corr(N_v0pt_5060, 0.0);
    vector<Double_t> vec_ey_v0pt_5060_trk(N_v0pt_5060, 0.0);
    vector<Double_t> vec_ey_v0pt_5060_cent(N_v0pt_5060, 0.0);
    vector<Double_t> vec_ey_v0pt_5060_pvZ(N_v0pt_5060, 0.0);

    vec_x_v0pt_5060.assign(x_v0pt_5060, x_v0pt_5060 + N_v0pt_5060);
    vec_ey_v0pt_5060_corr.assign(ey_v0pt_5060_corr, ey_v0pt_5060_corr + N_v0pt_5060);
    vec_ey_v0pt_5060_trk.assign(ey_v0pt_5060_trk, ey_v0pt_5060_trk + N_v0pt_5060);
    vec_ey_v0pt_5060_cent.assign(ey_v0pt_5060_cent, ey_v0pt_5060_cent + N_v0pt_5060);
    vec_ey_v0pt_5060_pvZ.assign(ey_v0pt_5060_pvZ, ey_v0pt_5060_pvZ + N_v0pt_5060);

    TGraph *gr_ey_v0pt_5060_corr = new TGraph(N_v0pt_5060, vec_x_v0pt_5060.data(), vec_ey_v0pt_5060_corr.data());
    TGraph *gr_ey_v0pt_5060_trk = new TGraph(N_v0pt_5060, vec_x_v0pt_5060.data(), vec_ey_v0pt_5060_trk.data());
    TGraph *gr_ey_v0pt_5060_cent = new TGraph(N_v0pt_5060, vec_x_v0pt_5060.data(), vec_ey_v0pt_5060_cent.data());
    TGraph *gr_ey_v0pt_5060_pvZ = new TGraph(N_v0pt_5060, vec_x_v0pt_5060.data(), vec_ey_v0pt_5060_pvZ.data());

    customize_TGraph(gr_ey_v0pt_5060_corr, "v_{0}(p_{T}) - 50-60%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.012, 20, 51, 1.0);
    customize_TGraph(gr_ey_v0pt_5060_trk, "v_{0}(p_{T}) - 50-60%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.012, 20, 62, 1.0);
    customize_TGraph(gr_ey_v0pt_5060_cent, "v_{0}(p_{T}) - 50-60%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.012, 20, 76, 1.0);
    customize_TGraph(gr_ey_v0pt_5060_pvZ, "v_{0}(p_{T}) - 50-60%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.012, 20, 95, 1.0);

    auto l_v0pt_5060_vars = new TLegend(0.103, 0.81, 0.5, 0.61);
    l_v0pt_5060_vars->SetTextSize(0.034);
    l_v0pt_5060_vars->AddEntry(gr_ey_v0pt_5060_corr, "Corrections", "lp");
    l_v0pt_5060_vars->AddEntry(gr_ey_v0pt_5060_trk, "Track Select.", "lp");
    l_v0pt_5060_vars->AddEntry(gr_ey_v0pt_5060_cent, "Centrality Fluct.", "lp");
    l_v0pt_5060_vars->AddEntry(gr_ey_v0pt_5060_pvZ, "Z Vertex Pos.", "lp");
    l_v0pt_5060_vars->SetBorderSize(0);
    l_v0pt_5060_vars->SetFillStyle(0);

    gr_ey_v0pt_5060_corr->Draw("ALP");
    gr_ey_v0pt_5060_trk->Draw("LP");
    gr_ey_v0pt_5060_cent->Draw("LP");
    gr_ey_v0pt_5060_pvZ->Draw("LP");
    l_v0pt_5060_vars->Draw();
    gPad->SetLogx();

    c_v0pt_vars->cd(2);
    TGraphAsymmErrors *gr_v0pt_6070_corr = (TGraphAsymmErrors*)f->Get("gr_v0pt_6070_corr");
    TGraphAsymmErrors *gr_v0pt_6070_trk = (TGraphAsymmErrors*)f->Get("gr_v0pt_6070_trk");
    TGraphAsymmErrors *gr_v0pt_6070_cent = (TGraphAsymmErrors*)f->Get("gr_v0pt_6070_cent");
    TGraphAsymmErrors *gr_v0pt_6070_pvZ = (TGraphAsymmErrors*)f->Get("gr_v0pt_6070_pvZ");

    Int_t N_v0pt_6070 = gr_v0pt_6070_corr->GetN();
    Double_t *x_v0pt_6070 = gr_v0pt_6070_corr->GetX();
    Double_t *ey_v0pt_6070_corr = gr_v0pt_6070_corr->GetEYlow();
    Double_t *ey_v0pt_6070_trk = gr_v0pt_6070_trk->GetEYlow();
    Double_t *ey_v0pt_6070_cent = gr_v0pt_6070_cent->GetEYlow();
    Double_t *ey_v0pt_6070_pvZ = gr_v0pt_6070_pvZ->GetEYlow();

    vector<Double_t> vec_x_v0pt_6070(N_v0pt_6070, 0.0);
    vector<Double_t> vec_ey_v0pt_6070_corr(N_v0pt_6070, 0.0);
    vector<Double_t> vec_ey_v0pt_6070_trk(N_v0pt_6070, 0.0);
    vector<Double_t> vec_ey_v0pt_6070_cent(N_v0pt_6070, 0.0);
    vector<Double_t> vec_ey_v0pt_6070_pvZ(N_v0pt_6070, 0.0);

    vec_x_v0pt_6070.assign(x_v0pt_6070, x_v0pt_6070 + N_v0pt_6070);
    vec_ey_v0pt_6070_corr.assign(ey_v0pt_6070_corr, ey_v0pt_6070_corr + N_v0pt_6070);
    vec_ey_v0pt_6070_trk.assign(ey_v0pt_6070_trk, ey_v0pt_6070_trk + N_v0pt_6070);
    vec_ey_v0pt_6070_cent.assign(ey_v0pt_6070_cent, ey_v0pt_6070_cent + N_v0pt_6070);
    vec_ey_v0pt_6070_pvZ.assign(ey_v0pt_6070_pvZ, ey_v0pt_6070_pvZ + N_v0pt_6070);

    TGraph *gr_ey_v0pt_6070_corr = new TGraph(N_v0pt_6070, vec_x_v0pt_6070.data(), vec_ey_v0pt_6070_corr.data());
    TGraph *gr_ey_v0pt_6070_trk = new TGraph(N_v0pt_6070, vec_x_v0pt_6070.data(), vec_ey_v0pt_6070_trk.data());
    TGraph *gr_ey_v0pt_6070_cent = new TGraph(N_v0pt_6070, vec_x_v0pt_6070.data(), vec_ey_v0pt_6070_cent.data());
    TGraph *gr_ey_v0pt_6070_pvZ = new TGraph(N_v0pt_6070, vec_x_v0pt_6070.data(), vec_ey_v0pt_6070_pvZ.data());

    customize_TGraph(gr_ey_v0pt_6070_corr, "v_{0}(p_{T}) - 60-70%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.012, 20, 51, 1.0);
    customize_TGraph(gr_ey_v0pt_6070_trk, "v_{0}(p_{T}) - 60-70%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.012, 20, 62, 1.0);
    customize_TGraph(gr_ey_v0pt_6070_cent, "v_{0}(p_{T}) - 60-70%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.012, 20, 76, 1.0);
    customize_TGraph(gr_ey_v0pt_6070_pvZ, "v_{0}(p_{T}) - 60-70%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.012, 20, 95, 1.0);

    auto l_v0pt_6070_vars = new TLegend(0.103, 0.81, 0.5, 0.61);
    l_v0pt_6070_vars->SetTextSize(0.034);
    l_v0pt_6070_vars->AddEntry(gr_ey_v0pt_6070_corr, "Corrections", "lp");
    l_v0pt_6070_vars->AddEntry(gr_ey_v0pt_6070_trk, "Track Select.", "lp");
    l_v0pt_6070_vars->AddEntry(gr_ey_v0pt_6070_cent, "Centrality Fluct.", "lp");
    l_v0pt_6070_vars->AddEntry(gr_ey_v0pt_6070_pvZ, "Z Vertex Pos.", "lp");
    l_v0pt_6070_vars->SetBorderSize(0);
    l_v0pt_6070_vars->SetFillStyle(0);

    gr_ey_v0pt_6070_corr->Draw("ALP");
    gr_ey_v0pt_6070_trk->Draw("LP");
    gr_ey_v0pt_6070_cent->Draw("LP");
    gr_ey_v0pt_6070_pvZ->Draw("LP");
    l_v0pt_6070_vars->Draw();
    gPad->SetLogx();

    c_v0pt_vars->SaveAs("./Plots/SystematicsErrors/v0pt_vars.pdf");
}

void DoPlot_Syst_sv0pt(){

    TFile *f_main = TFile::Open("./Data/Figures/main.root");
    TFile *f = TFile::Open("./Data/Systematics/SystUncs.root");

    // Different types
    auto c_sv0pt_vars = new TCanvas("c_sv0pt_vars", "c_sv0pt_vars", 1100, 500);
    c_sv0pt_vars->Divide(2,1);

    c_sv0pt_vars->cd(1);
    TGraphAsymmErrors *gr_sv0pt_5060_corr = (TGraphAsymmErrors*)f->Get("gr_sv0pt_5060_corr");
    TGraphAsymmErrors *gr_sv0pt_5060_trk = (TGraphAsymmErrors*)f->Get("gr_sv0pt_5060_trk");
    TGraphAsymmErrors *gr_sv0pt_5060_cent = (TGraphAsymmErrors*)f->Get("gr_sv0pt_5060_cent");
    TGraphAsymmErrors *gr_sv0pt_5060_pvZ = (TGraphAsymmErrors*)f->Get("gr_sv0pt_5060_pvZ");

    Int_t N_sv0pt_5060 = gr_sv0pt_5060_corr->GetN();
    Double_t *x_sv0pt_5060 = gr_sv0pt_5060_corr->GetX();
    Double_t *ey_sv0pt_5060_corr = gr_sv0pt_5060_corr->GetEYlow();
    Double_t *ey_sv0pt_5060_trk = gr_sv0pt_5060_trk->GetEYlow();
    Double_t *ey_sv0pt_5060_cent = gr_sv0pt_5060_cent->GetEYlow();
    Double_t *ey_sv0pt_5060_pvZ = gr_sv0pt_5060_pvZ->GetEYlow();

    vector<Double_t> vec_x_sv0pt_5060(N_sv0pt_5060, 0.0);
    vector<Double_t> vec_ey_sv0pt_5060_corr(N_sv0pt_5060, 0.0);
    vector<Double_t> vec_ey_sv0pt_5060_trk(N_sv0pt_5060, 0.0);
    vector<Double_t> vec_ey_sv0pt_5060_cent(N_sv0pt_5060, 0.0);
    vector<Double_t> vec_ey_sv0pt_5060_pvZ(N_sv0pt_5060, 0.0);

    vec_x_sv0pt_5060.assign(x_sv0pt_5060, x_sv0pt_5060 + N_sv0pt_5060);
    vec_ey_sv0pt_5060_corr.assign(ey_sv0pt_5060_corr, ey_sv0pt_5060_corr + N_sv0pt_5060);
    vec_ey_sv0pt_5060_trk.assign(ey_sv0pt_5060_trk, ey_sv0pt_5060_trk + N_sv0pt_5060);
    vec_ey_sv0pt_5060_cent.assign(ey_sv0pt_5060_cent, ey_sv0pt_5060_cent + N_sv0pt_5060);
    vec_ey_sv0pt_5060_pvZ.assign(ey_sv0pt_5060_pvZ, ey_sv0pt_5060_pvZ + N_sv0pt_5060);

    TGraph *gr_ey_sv0pt_5060_corr = new TGraph(N_sv0pt_5060, vec_x_sv0pt_5060.data(), vec_ey_sv0pt_5060_corr.data());
    TGraph *gr_ey_sv0pt_5060_trk = new TGraph(N_sv0pt_5060, vec_x_sv0pt_5060.data(), vec_ey_sv0pt_5060_trk.data());
    TGraph *gr_ey_sv0pt_5060_cent = new TGraph(N_sv0pt_5060, vec_x_sv0pt_5060.data(), vec_ey_sv0pt_5060_cent.data());
    TGraph *gr_ey_sv0pt_5060_pvZ = new TGraph(N_sv0pt_5060, vec_x_sv0pt_5060.data(), vec_ey_sv0pt_5060_pvZ.data());

    customize_TGraph(gr_ey_sv0pt_5060_corr, "v_{0}(p_{T})/v_{0} - 50-60%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.6, 20, 51, 1.0);
    customize_TGraph(gr_ey_sv0pt_5060_trk, "v_{0}(p_{T})/v_{0} - 50-60%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.6, 20, 62, 1.0);
    customize_TGraph(gr_ey_sv0pt_5060_cent, "v_{0}(p_{T})/v_{0} - 50-60%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.6, 20, 76, 1.0);
    customize_TGraph(gr_ey_sv0pt_5060_pvZ, "v_{0}(p_{T})/v_{0} - 50-60%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.6, 20, 95, 1.0);

    auto l_sv0pt_5060_vars = new TLegend(0.103, 0.81, 0.5, 0.61);
    l_sv0pt_5060_vars->SetTextSize(0.034);
    l_sv0pt_5060_vars->AddEntry(gr_ey_sv0pt_5060_corr, "Corrections", "lp");
    l_sv0pt_5060_vars->AddEntry(gr_ey_sv0pt_5060_trk, "Track Select.", "lp");
    l_sv0pt_5060_vars->AddEntry(gr_ey_sv0pt_5060_cent, "Centrality Fluct.", "lp");
    l_sv0pt_5060_vars->AddEntry(gr_ey_sv0pt_5060_pvZ, "Z Vertex Pos.", "lp");
    l_sv0pt_5060_vars->SetBorderSize(0);
    l_sv0pt_5060_vars->SetFillStyle(0);

    gr_ey_sv0pt_5060_corr->Draw("ALP");
    gr_ey_sv0pt_5060_trk->Draw("LP");
    gr_ey_sv0pt_5060_cent->Draw("LP");
    gr_ey_sv0pt_5060_pvZ->Draw("LP");
    l_sv0pt_5060_vars->Draw();
    gPad->SetLogx();

    c_sv0pt_vars->cd(2);
    TGraphAsymmErrors *gr_sv0pt_6070_corr = (TGraphAsymmErrors*)f->Get("gr_sv0pt_6070_corr");
    TGraphAsymmErrors *gr_sv0pt_6070_trk = (TGraphAsymmErrors*)f->Get("gr_sv0pt_6070_trk");
    TGraphAsymmErrors *gr_sv0pt_6070_cent = (TGraphAsymmErrors*)f->Get("gr_sv0pt_6070_cent");
    TGraphAsymmErrors *gr_sv0pt_6070_pvZ = (TGraphAsymmErrors*)f->Get("gr_sv0pt_6070_pvZ");

    Int_t N_sv0pt_6070 = gr_sv0pt_6070_corr->GetN();
    Double_t *x_sv0pt_6070 = gr_sv0pt_6070_corr->GetX();
    Double_t *ey_sv0pt_6070_corr = gr_sv0pt_6070_corr->GetEYlow();
    Double_t *ey_sv0pt_6070_trk = gr_sv0pt_6070_trk->GetEYlow();
    Double_t *ey_sv0pt_6070_cent = gr_sv0pt_6070_cent->GetEYlow();
    Double_t *ey_sv0pt_6070_pvZ = gr_sv0pt_6070_pvZ->GetEYlow();

    vector<Double_t> vec_x_sv0pt_6070(N_sv0pt_6070, 0.0);
    vector<Double_t> vec_ey_sv0pt_6070_corr(N_sv0pt_6070, 0.0);
    vector<Double_t> vec_ey_sv0pt_6070_trk(N_sv0pt_6070, 0.0);
    vector<Double_t> vec_ey_sv0pt_6070_cent(N_sv0pt_6070, 0.0);
    vector<Double_t> vec_ey_sv0pt_6070_pvZ(N_sv0pt_6070, 0.0);

    vec_x_sv0pt_6070.assign(x_sv0pt_6070, x_sv0pt_6070 + N_sv0pt_6070);
    vec_ey_sv0pt_6070_corr.assign(ey_sv0pt_6070_corr, ey_sv0pt_6070_corr + N_sv0pt_6070);
    vec_ey_sv0pt_6070_trk.assign(ey_sv0pt_6070_trk, ey_sv0pt_6070_trk + N_sv0pt_6070);
    vec_ey_sv0pt_6070_cent.assign(ey_sv0pt_6070_cent, ey_sv0pt_6070_cent + N_sv0pt_6070);
    vec_ey_sv0pt_6070_pvZ.assign(ey_sv0pt_6070_pvZ, ey_sv0pt_6070_pvZ + N_sv0pt_6070);

    TGraph *gr_ey_sv0pt_6070_corr = new TGraph(N_sv0pt_6070, vec_x_sv0pt_6070.data(), vec_ey_sv0pt_6070_corr.data());
    TGraph *gr_ey_sv0pt_6070_trk = new TGraph(N_sv0pt_6070, vec_x_sv0pt_6070.data(), vec_ey_sv0pt_6070_trk.data());
    TGraph *gr_ey_sv0pt_6070_cent = new TGraph(N_sv0pt_6070, vec_x_sv0pt_6070.data(), vec_ey_sv0pt_6070_cent.data());
    TGraph *gr_ey_sv0pt_6070_pvZ = new TGraph(N_sv0pt_6070, vec_x_sv0pt_6070.data(), vec_ey_sv0pt_6070_pvZ.data());

    customize_TGraph(gr_ey_sv0pt_6070_corr, "v_{0}(p_{T})/v_{0} - 60-70%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.6, 20, 51, 1.0);
    customize_TGraph(gr_ey_sv0pt_6070_trk, "v_{0}(p_{T})/v_{0} - 60-70%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.6, 20, 62, 1.0);
    customize_TGraph(gr_ey_sv0pt_6070_cent, "v_{0}(p_{T})/v_{0} - 60-70%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.6, 20, 76, 1.0);
    customize_TGraph(gr_ey_sv0pt_6070_pvZ, "v_{0}(p_{T})/v_{0} - 60-70%; p_{T} [GeV]; Uncertainty", 0.0, 10.0, 0, 0.6, 20, 95, 1.0);

    auto l_sv0pt_6070_vars = new TLegend(0.103, 0.81, 0.5, 0.61);
    l_sv0pt_6070_vars->SetTextSize(0.034);
    l_sv0pt_6070_vars->AddEntry(gr_ey_sv0pt_6070_corr, "Corrections", "lp");
    l_sv0pt_6070_vars->AddEntry(gr_ey_sv0pt_6070_trk, "Track Select.", "lp");
    l_sv0pt_6070_vars->AddEntry(gr_ey_sv0pt_6070_cent, "Centrality Fluct.", "lp");
    l_sv0pt_6070_vars->AddEntry(gr_ey_sv0pt_6070_pvZ, "Z Vertex Pos.", "lp");
    l_sv0pt_6070_vars->SetBorderSize(0);
    l_sv0pt_6070_vars->SetFillStyle(0);

    gr_ey_sv0pt_6070_corr->Draw("ALP");
    gr_ey_sv0pt_6070_trk->Draw("LP");
    gr_ey_sv0pt_6070_cent->Draw("LP");
    gr_ey_sv0pt_6070_pvZ->Draw("LP");
    l_sv0pt_6070_vars->Draw();
    gPad->SetLogx();

    c_sv0pt_vars->SaveAs("./Plots/SystematicsErrors/sv0pt_vars.pdf");
}

void DoPlot_v0_only(){

    TFile *f_v0 = TFile::Open("./Data/Figures/main.root", "READ");
    TGraphErrors *gr_v0_55 = (TGraphErrors*)f_v0->Get("v0_55_1");
    TGraphErrors *gr_v0_65 = (TGraphErrors*)f_v0->Get("v0_65_1");

    TFile *f_unc = TFile::Open("./Data/Systematics/SystUncs.root", "READ");
    TGraphErrors *gr_v0_unc = (TGraphErrors*)f_unc->Get("gr_v0_total");

    customize_TGraphErrors(gr_v0_unc, "; Centrality [%]; v_{0}", 50.0, 70.0, 0.013, 0.022, 20, 1, 1.2);
    gr_v0_unc->SetFillColorAlpha(1, 0.3); gr_v0_unc->SetFillStyle(1001);
    customize_TGraphErrors(gr_v0_55, "; Centrality [%]; v_{0}", 50.0, 70.0, 0.013, 0.022, 20, 1, 1.2);
    customize_TGraphErrors(gr_v0_65, "; Centrality [%]; v_{0}", 50.0, 70.0, 0.013, 0.022, 20, 1, 1.2);

    auto c_v0 = new TCanvas("c_v0", "c_v0", 725, 600);

    gr_v0_unc->Draw("A2");
    gr_v0_55->Draw("P SAME");
    gr_v0_65->Draw("P SAME");
    gPad->SetLeftMargin(0.12);
    gPad->SetTopMargin(0.01);

    c_v0->SaveAs("./Plots/SystematicsErrors/v0.pdf");
}