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
    g->SetMarkerSize(size);
}

void DoPlot1(TString filename){
    TFile *f = TFile::Open(filename, "READ");
    TGraph *gr1 = (TGraph*)f->Get("55_1");
    TGraph *gr2 = (TGraph*)f->Get("55_2");
    TGraph *gr3 = (TGraph*)f->Get("55_3");
    TGraph *gr4 = (TGraph*)f->Get("65_1");
    TGraph *gr5 = (TGraph*)f->Get("65_2");
    TGraph *gr6 = (TGraph*)f->Get("65_3");

    TGraph *grd1 = create_TGraphFromTxt("./Data/Fig1/v0_1.txt", 29, "v_{0} vs centrality; centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 20, 2, 1.0);
    TGraph *grd2 = create_TGraphFromTxt("./Data/Fig1/v0_2.txt", 29, "v_{0} vs centrality; centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 21, 4, 1.0);
    TGraph *grd3 = create_TGraphFromTxt("./Data/Fig1/v0_3.txt", 29, "v_{0} vs centrality; centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 25, 6, 1.0);

    customize_TGraph(gr1, "v_{0} vs centrality; centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 47, 52, 1.2);
    customize_TGraph(gr2, "v_{0} vs centrality; centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 33, 7, 1.2);
    customize_TGraph(gr3, "v_{0} vs centrality; centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 34, 95, 1.2);
    customize_TGraph(gr4, "v_{0} vs centrality; centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 47, 52, 1.2);
    customize_TGraph(gr5, "v_{0} vs centrality; centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 33, 7, 1.2);
    customize_TGraph(gr6, "v_{0} vs centrality; centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 34, 95, 1.2);

    auto c = new TCanvas("c", "v0_centrality", 500, 500);

    auto leg_title = new TLegend(0.015, 0.97, 0.5, 0.71);
    leg_title->SetTextSize(0.0457);
    leg_title->AddEntry((TObject*)0, "Pb+Pb      #eta_{gap} = 1", "");
    leg_title->SetBorderSize(0);
    leg_title->SetFillStyle(0);

    auto leg_ptref = new TLegend(0.45, 0.43, 0.7, 0.13);
    leg_ptref->SetTextSize(0.023);
    leg_ptref->AddEntry((TObject*)0, "#bf{p_{T}^{ref} range}", "C");
    leg_ptref->AddEntry(gr1, "CMS Open Data 2.76 TeV   0.5-2 GeV", "p");
    leg_ptref->AddEntry(gr2, "CMS Open Data 2.76 TeV   0.5-5 GeV", "p");
    leg_ptref->AddEntry(gr3, "CMS Open Data 2.76 TeV   1-5 GeV", "p");
    leg_ptref->AddEntry(grd1, "ATLAS 5.02 TeV   0.5-2 GeV", "p");
    leg_ptref->AddEntry(grd2, "ATLAS 5.02 TeV   0.5-5 GeV", "p");
    leg_ptref->AddEntry(grd3, "ATLAS 5.02 TeV   1-5 GeV", "p");
    leg_ptref->SetBorderSize(0);
    leg_ptref->SetFillStyle(0);

    grd1->Draw("AP");
    grd2->Draw("P SAME");
    grd3->Draw("P SAME");
    gr1->Draw("P SAME");
    gr2->Draw("P SAME");
    gr3->Draw("P SAME");
    gr4->Draw("P SAME");
    gr5->Draw("P SAME");
    gr6->Draw("P SAME");
    leg_title->Draw();
    leg_ptref->Draw();
    c->SetLogy();   
    c->Update();
    c->SaveAs("Plot1.pdf");
    delete c;
}

void DoPlot2(TString filename){
    TFile *f = TFile::Open(filename, "READ");
    TGraph *gr_rel1 = (TGraph*)f->Get("rel_1");
    TGraph *gr_rel2 = (TGraph*)f->Get("rel_2");
    TGraph *gr_rel3 = (TGraph*)f->Get("rel_3");
    TGraph *gr_v0pt1 = (TGraph*)f->Get("v0pt_1");
    TGraph *gr_v0pt2 = (TGraph*)f->Get("v0pt_2");
    TGraph *gr_v0pt3 = (TGraph*)f->Get("v0pt_3");

    customize_TGraph(gr_rel1, "Relation; v_{0}(p_{T}); #langle N(p_{T}) #delta p_{T} #rangle / #langle N_{0}(p_{T}) #rangle #langle p_{T} #rangle", 0.0, 10.0, -2.0, 8.0, 47, 52, 1.2);
    customize_TGraph(gr_rel2, "Relation; v_{0}(p_{T}); #langle N(p_{T}) #delta p_{T} #rangle / #langle N_{0}(p_{T}) #rangle #langle p_{T} #rangle", 0.0, 10.0, -2.0, 8.0, 33, 7, 1.2);
    customize_TGraph(gr_rel3, "Relation; v_{0}(p_{T}); #langle N(p_{T}) #delta p_{T} #rangle / #langle N_{0}(p_{T}) #rangle #langle p_{T} #rangle", 0.0, 10.0, -2.0, 8.0, 34, 95, 1.2);
    customize_TGraph(gr_v0pt1, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 47, 52, 1.2);
    customize_TGraph(gr_v0pt2, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 33, 7, 1.2);
    customize_TGraph(gr_v0pt3, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 34, 95, 1.2);

    TGraph *grd_v0pt1 = create_TGraphFromTxt("./Data/Fig2/ATLAS_v0ptref1.txt", 29, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 20, 2, 1.0);
    TGraph *grd_v0pt2 = create_TGraphFromTxt("./Data/Fig2/ATLAS_v0ptref2.txt", 29, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 21, 4, 1.0);
    TGraph *grd_v0pt3 = create_TGraphFromTxt("./Data/Fig2/ATLAS_v0ptref3.txt", 29, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.5, 25, 6, 1.0);

    auto c = new TCanvas("c", "c", 850, 500);
    c->Divide(2, 1);

    auto leg_v0pt_title = new TLegend(0.025, 0.89, 0.5, 0.76);
    leg_v0pt_title->SetTextSize(0.0457);
    leg_v0pt_title->AddEntry((TObject*)0, "Pb+Pb,   #eta_{gap} = 1", "");
    leg_v0pt_title->AddEntry((TObject*)0, "50-60% Centrality", "");
    leg_v0pt_title->SetBorderSize(0);
    leg_v0pt_title->SetFillStyle(0);

    auto leg_v0pt_ptref = new TLegend(0.103, 0.748, 0.5, 0.57);
    leg_v0pt_ptref->SetTextSize(0.025);
    leg_v0pt_ptref->AddEntry((TObject*)0, "#bf{p_{T}^{ref} range}", "C");
    leg_v0pt_ptref->AddEntry(gr_v0pt1, "CMS OpenData 2.76 TeV: 0.5-2 GeV", "p");
    leg_v0pt_ptref->AddEntry(gr_v0pt2, "CMS OpenData 2.76 TeV: 0.5-5 GeV", "p");
    leg_v0pt_ptref->AddEntry(gr_v0pt3, "CMS OpenData 2.76 TeV: 1-5 GeV", "p");
    leg_v0pt_ptref->AddEntry(grd_v0pt1, "ATLAS 5.02 TeV: 0.5-2 GeV", "p");
    leg_v0pt_ptref->AddEntry(grd_v0pt2, "ATLAS 5.02 TeV: 0.5-5 GeV", "p");
    leg_v0pt_ptref->AddEntry(grd_v0pt3, "ATLAS 5.02 TeV: 1-5 GeV", "p");
    leg_v0pt_ptref->SetBorderSize(0);
    leg_v0pt_ptref->SetFillStyle(0);

    c->cd(1);
    gr_rel1->Draw("AP");
    gr_rel2->Draw("P SAME");
    gr_rel3->Draw("P SAME");
    gPad->SetLogx();

    c->cd(2);
    gr_v0pt1->Draw("AP");
    gr_v0pt2->Draw("P SAME");
    gr_v0pt3->Draw("P SAME");
    grd_v0pt1->Draw("P SAME");
    grd_v0pt2->Draw("P SAME");
    grd_v0pt3->Draw("P SAME");
    leg_v0pt_title->Draw();
    leg_v0pt_ptref->Draw();
    gPad->SetLogx();

    c->Update();
    c->SaveAs("Plot2.pdf");
    delete c;
}

void DoPlot3(TString filename){
    TFile *f = TFile::Open(filename, "READ");
    TGraph *gr0 = (TGraph*)f->Get("eta0");
    TGraph *gr1 = (TGraph*)f->Get("eta1");
    TGraph *gr2 = (TGraph*)f->Get("eta2");
    TGraph *gr3 = (TGraph*)f->Get("eta3");

    customize_TGraph(gr0, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.4, 0.5, 47, 52, 1.0);
    customize_TGraph(gr1, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.4, 0.5, 34, 7, 1.0);
    customize_TGraph(gr2, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.4, 0.5, 22, 95, 1.0);
    customize_TGraph(gr3, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.4, 0.5, 33, 8, 1.0);

    TGraph *grd0 = create_TGraphFromTxt("./Data/Fig3/ATLAS_eta0.txt", 29, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.4, 0.5, 20, 2, 1.0);
    TGraph *grd1 = create_TGraphFromTxt("./Data/Fig3/ATLAS_eta1.txt", 29, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.4, 0.5, 21, 1, 1.0);
    TGraph *grd2 = create_TGraphFromTxt("./Data/Fig3/ATLAS_eta0.txt", 29, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.4, 0.5, 23, 4, 1.0);
    TGraph *grd3 = create_TGraphFromTxt("./Data/Fig3/ATLAS_eta0.txt", 29, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.4, 0.5, 45, 6, 1.0);

    auto c = new TCanvas("c", "c_v0pt_eta", 500, 500);

    auto leg_title = new TLegend(0.025, 0.89, 0.5, 0.71);
    leg_title->SetTextSize(0.0457);
    leg_title->AddEntry((TObject*)0, "Pb+Pb", "");
    leg_title->AddEntry((TObject*)0, "p_{T}^{ref}: 0.5-2 GeV", "");
    leg_title->AddEntry((TObject*)0, "50-60% Centrality", "");
    leg_title->SetBorderSize(0);
    leg_title->SetFillStyle(0);

    auto leg_etas = new TLegend(0.23, 0.125, 0.63, 0.43);
    leg_etas->SetTextSize(0.023);
    leg_etas->AddEntry(gr0, "CMS Open Data 2.76 TeV    #eta_{gap} = 0", "p");
    leg_etas->AddEntry(gr1, "CMS Open Data 2.76 TeV    #eta_{gap} = 1", "p");
    leg_etas->AddEntry(gr2, "CMS Open Data 2.76 TeV    #eta_{gap} = 2", "p");
    leg_etas->AddEntry(gr3, "CMS Open Data 2.76 TeV    #eta_{gap} = 3", "p");
    leg_etas->AddEntry(grd0, "ATLAS 5.02 TeV    #eta_{gap} = 0", "p");
    leg_etas->AddEntry(grd1, "ATLAS 5.02 TeV    #eta_{gap} = 1", "p");
    leg_etas->AddEntry(grd2, "ATLAS 5.02 TeV    #eta_{gap} = 2", "p");
    leg_etas->AddEntry(grd3, "ATLAS 5.02 TeV    #eta_{gap} = 3", "p");
    leg_etas->SetBorderSize(0);
    leg_etas->SetFillStyle(0);

    gr0->Draw("AP");
    gr1->Draw("P SAME");
    gr2->Draw("P SAME");
    gr3->Draw("P SAME");
    grd0->Draw("P SAME");
    grd1->Draw("P SAME");
    grd2->Draw("P SAME");
    grd3->Draw("P SAME");
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

    TGraph* gr_v0pt_pink = create_TGraphFromTxt("./Data/Fig4/ATLAS_v0pt_pink.txt", 29, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 21, 6, 1.0);
    TGraph* gr_v0pt_purple = create_TGraphFromTxt("./Data/Fig4/ATLAS_v0pt_purple.txt", 29, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 20, 52, 1.0);
    TGraph* gr_v0ptv0_pink = create_TGraphFromTxt("./Data/Fig4/ATLAS_v0ptv0_pink.txt", 29, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 21, 6, 1.0);
    TGraph* gr_v0ptv0_purple = create_TGraphFromTxt("./Data/Fig4/ATLAS_v0ptv0_purple.txt", 29, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 20, 52, 1.0);

    customize_TGraph(gr_v0pt_red, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 47, 2, 1.0);
    customize_TGraph(gr_v0pt_blue, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 34, 4, 1.0);
    customize_TGraph(gr_v0ptv0_red, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 47, 2, 1.0);
    customize_TGraph(gr_v0ptv0_blue, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 34, 4, 1.0);

    auto c = new TCanvas("c", "c", 850, 500);
    c->Divide(2, 1);

    // v0(pT) legend
    auto legend_v0pt_text = new TLegend(0.025, 0.89, 0.5, 0.76);
    legend_v0pt_text->SetTextSize(0.0457);
    legend_v0pt_text->AddEntry((TObject*)0, "Pb+Pb,   #eta_{gap} = 1", "");
    legend_v0pt_text->AddEntry((TObject*)0, "p_{T}^{ref} 0.5-2 GeV", "");
    legend_v0pt_text->SetBorderSize(0);
    legend_v0pt_text->SetFillStyle(0);

    auto legend_v0pt_cents = new TLegend(0.103, 0.748, 0.5, 0.57);
    legend_v0pt_cents->SetTextSize(0.0325);
    legend_v0pt_cents->AddEntry(gr_v0pt_red, "CMS OpenData 2.76 TeV (50-60%)", "p");
    legend_v0pt_cents->AddEntry(gr_v0pt_blue, "CMS OpenData 2.76 TeV (60-70%)", "p");
    legend_v0pt_cents->AddEntry(gr_v0pt_purple, "ATLAS 5.02 TeV (50-60%)", "p");
    legend_v0pt_cents->AddEntry(gr_v0pt_pink, "ATLAS 5.02 TeV (60-70%)", "p");
    legend_v0pt_cents->SetBorderSize(0);
    legend_v0pt_cents->SetFillStyle(0);
    
    // v0(pT)/v0 legend
    auto legend_v0ptv0_text = new TLegend(0.025, 0.89, 0.5, 0.76);
    legend_v0ptv0_text->SetTextSize(0.0457);
    legend_v0ptv0_text->AddEntry((TObject*)0, "Pb+Pb,   #eta_{gap} = 1", "");
    legend_v0ptv0_text->AddEntry((TObject*)0, "p_{T}^{ref} 0.5-2 GeV", "");
    legend_v0ptv0_text->SetBorderSize(0);
    legend_v0ptv0_text->SetFillStyle(0);

    auto legend_v0ptv0_cents = new TLegend(0.103, 0.748, 0.5, 0.57);
    legend_v0ptv0_cents->SetTextSize(0.0325);
    legend_v0ptv0_cents->AddEntry(gr_v0pt_red, "CMS OpenData 2.76 TeV (50-60%)", "p");
    legend_v0ptv0_cents->AddEntry(gr_v0pt_blue, "CMS OpenData 2.76 TeV (60-70%)", "p");
    legend_v0ptv0_cents->AddEntry(gr_v0pt_purple, "ATLAS 5.02 TeV (50-60%)", "p");
    legend_v0ptv0_cents->AddEntry(gr_v0pt_pink, "ATLAS 5.02 TeV (60-70%)", "p");
    legend_v0ptv0_cents->SetBorderSize(0);
    legend_v0ptv0_cents->SetFillStyle(0);

     // Drawing v0(pT) plot
    c->cd(1);
    gr_v0pt_pink->Draw("AP");
    gr_v0pt_purple->Draw("P SAME");
    gr_v0pt_red->Draw("P SAME");
    gr_v0pt_blue->Draw("P SAME");
    legend_v0pt_text->Draw();
    legend_v0pt_cents->Draw();
    gPad->SetLogx();

    // Drawing v0(pT)/v0 plot
    c->cd(2);
    gr_v0ptv0_pink->Draw("AP");
    gr_v0ptv0_blue->Draw("P SAME");
    gr_v0ptv0_purple->Draw("P SAME");
    gr_v0ptv0_red->Draw("P SAME");
    legend_v0ptv0_text->Draw();
    legend_v0ptv0_cents->Draw();
    gPad->SetLogx();

    // Saving canvas as pdf
    c->Update();
    c->SaveAs("Plot4.pdf");
    delete c;
}