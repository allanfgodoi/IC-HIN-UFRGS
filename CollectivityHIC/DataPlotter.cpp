// TGraph creator function
TGraph* create_TGraph(int nPoints, const float* x, const float* y, const char* title, float xmin, int xmax, float ymin, float ymax, int style, int color){
    TGraph* g = new TGraph(nPoints, x, y);
    g->SetTitle(title);
    g->GetXaxis()->SetLimits(xmin, xmax); // X axis range
    g->GetXaxis()->CenterTitle(); // Center axis label
    g->GetYaxis()->CenterTitle(); // Center axis label
    g->SetMinimum(ymin); // Y axis range
    g->SetMaximum(ymax); // Y axis range
    g->SetMarkerStyle(style);
    g->SetMarkerColor(color);
    g->SetMarkerSize(1.2);
    return g;
}

TGraph* create_TGraphFromTxt(string filePath, int nPoints, const char* title, float xmin, float xmax, float ymin, float ymax, int style, int color){
    float* x = new float[nPoints];
    float* y = new float[nPoints];
    ifstream f(filePath);
    for (int i=0; i<nPoints; i++){
        f >> x[i] >> y[i];
    }
    TGraph* g = create_TGraph(nPoints, x, y, title, xmin, xmax, ymin, ymax, style, color);
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
    g->SetMarkerSize(size);
}

    void DoPlot3(TString filename){
    TFile *f = TFile::Open(filename, "READ");

    TGraph *gr0 = (TGraph*)f->Get("eta0");
    TGraph *gr1 = (TGraph*)f->Get("eta1");
    TGraph *gr2 = (TGraph*)f->Get("eta2");
    TGraph *gr3 = (TGraph*)f->Get("eta3");

    auto c = new TCanvas("c", "c_v0pt_eta", 500, 500);
    auto leg_title = new TLegend();
    leg_title->AddEntry((TObject*)0, "#font[62]{Pb+Pb}", "");
    leg_title->AddEntry((TObject*)0, "#font[62]{p_{T}^{ref}: 0.5-2 GeV}", "");
    leg_title->AddEntry((TObject*)0, "#font[62]{50-60% Centrality}", "");
    auto leg_etas = new TLegend();
    leg_etas->AddEntry(gr0, "#eta_{gap} = 0", "p");
    leg_etas->AddEntry(gr1, "#eta_{gap} = 1", "p");
    leg_etas->AddEntry(gr2, "#eta_{gap} = 2", "p");
    leg_etas->AddEntry(gr3, "#eta_{gap} = 3", "p");

    gr0->Draw("AP");
    gr1->Draw("P SAME");
    gr2->Draw("P SAME");
    gr3->Draw("P SAME");
    leg_title->Draw();
    leg_etas->Draw();
    gPad->SetLogx();

    c->Update();
    c->SaveAs("Plot3.pdf");
    delete c;
}

void DoPlot4(TString filename){
    TFile *f;
    f = TFile::Open(filename, "READ");

    TGraph *gr_v0pt_red = (TGraph*)f->Get("v0pt_red");
    TGraph *gr_v0pt_blue = (TGraph*)f->Get("v0pt_blue");
    TGraph *gr_v0ptv0_red = (TGraph*)f->Get("v0ptv0_red");
    TGraph *gr_v0ptv0_blue = (TGraph*)f->Get("v0ptv0_blue");

    TGraph* gr_v0pt_pink = create_TGraphFromTxt("./Data/Fig4/ATLAS_v0pt_pink.txt", 29, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 47, 6);
    TGraph* gr_v0pt_purple = create_TGraphFromTxt("./Data/Fig4/ATLAS_v0pt_purple.txt", 29, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 34, 52);
    TGraph* gr_v0ptv0_pink = create_TGraphFromTxt("./Data/Fig4/ATLAS_v0ptv0_pink.txt", 29, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 47, 6);
    TGraph* gr_v0ptv0_purple = create_TGraphFromTxt("./Data/Fig4/ATLAS_v0ptv0_purple.txt", 29, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 34, 52);

    customize_TGraph(gr_v0pt_red, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 48, 2, 1.2);
    customize_TGraph(gr_v0pt_blue, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 49, 4, 1.2);
    customize_TGraph(gr_v0ptv0_red, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 48, 2, 1.2);
    customize_TGraph(gr_v0ptv0_blue, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 49, 4, 1.2);

    auto c = new TCanvas("c", "c", 850, 500);
    c->Divide(2, 1);

    // v0(pT) legend
    auto legend_v0pt = new TLegend(0.125, 0.55, 0.585, 0.885);
    legend_v0pt->SetTextSize(0.0325);
    legend_v0pt->SetMargin(0.05);
    legend_v0pt->AddEntry((TObject*)0, "#font[62]{Pb+Pb,   #eta_{gap} = 1}", "");
    legend_v0pt->AddEntry((TObject*)0, "#font[62]{p_{T}^{ref} 0.5-2 GeV}", "");
    legend_v0pt->AddEntry(gr_v0pt_red, "CMS OpenData 2.76 TeV (50-60%)", "p");
    legend_v0pt->AddEntry(gr_v0pt_blue, "CMS OpenData 2.76 TeV (60-70%)", "p");
    legend_v0pt->AddEntry(gr_v0pt_purple, "ATLAS 5.02 TeV (50-60%)", "p");
    legend_v0pt->AddEntry(gr_v0pt_pink, "ATLAS 5.02 TeV (60-70%)", "p");
    legend_v0pt->SetBorderSize(0);
    legend_v0pt->SetFillStyle(0);
    
    // v0(pT)/v0 legend
    auto legend_v0ptv0 = new TLegend(0.125, 0.55, 0.585, 0.885);
    legend_v0ptv0->SetTextSize(0.0325);
    legend_v0ptv0->SetMargin(0.05);
    legend_v0ptv0->AddEntry((TObject*)0, "#font[62]{Pb+Pb,   #eta_{gap} = 1}", "");
    legend_v0ptv0->AddEntry((TObject*)0, "#font[62]{p_{T}^{ref} 0.5-2 GeV}", "");
    legend_v0ptv0->AddEntry(gr_v0ptv0_red, "CMS OpenData 2.76 TeV (50-60%)", "p");
    legend_v0ptv0->AddEntry(gr_v0ptv0_blue, "CMS OpenData 2.76 TeV (60-70%)", "p");
    legend_v0ptv0->AddEntry(gr_v0pt_purple, "ATLAS 5.02 TeV (50-60%)", "p");
    legend_v0ptv0->AddEntry(gr_v0pt_pink, "ATLAS 5.02 TeV (60-70%)", "p");
    legend_v0ptv0->SetBorderSize(0);
    legend_v0ptv0->SetFillStyle(0);

     // Drawing v0(pT) plot
    c->cd(1);
    gr_v0pt_pink->Draw("AP");
    gr_v0pt_red->Draw("P SAME");
    gr_v0pt_purple->Draw("P SAME");
    gr_v0pt_blue->Draw("P SAME");
    legend_v0pt->Draw();
    gPad->SetLogx();

    // Drawing v0(pT)/v0 plot
    c->cd(2);
    gr_v0ptv0_pink->Draw("AP");
    gr_v0ptv0_blue->Draw("P SAME");
    gr_v0ptv0_purple->Draw("P SAME");
    gr_v0ptv0_red->Draw("P SAME");
    legend_v0ptv0->Draw();
    gPad->SetLogx();

    // Saving canvas as pdf
    c->Update();
    c->SaveAs("Plot4.pdf");
    delete c;
}