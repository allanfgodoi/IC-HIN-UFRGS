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
    g->SetMarkerSize(size);
}

void DoPlot1(TString filename1, TString filename2){
    TFile *f = TFile::Open(filename1, "READ");
    TGraph *gr1 = (TGraph*)f->Get("55_1");
    TGraph *gr2 = (TGraph*)f->Get("55_2");
    TGraph *gr3 = (TGraph*)f->Get("55_3");
    TGraph *gr4 = (TGraph*)f->Get("65_1");
    TGraph *gr5 = (TGraph*)f->Get("65_2");
    TGraph *gr6 = (TGraph*)f->Get("65_3");

    TFile *cf = TFile::Open(filename2, "READ");
    TGraph *grc1 = (TGraph*)cf->Get("55_1");
    TGraph *grc2 = (TGraph*)cf->Get("55_2");
    TGraph *grc3 = (TGraph*)cf->Get("55_3");
    TGraph *grc4 = (TGraph*)cf->Get("65_1");
    TGraph *grc5 = (TGraph*)cf->Get("65_2");
    TGraph *grc6 = (TGraph*)cf->Get("65_3");

    customize_TGraph(gr1, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 47, 52, 1.2);
    customize_TGraph(gr2, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 33, 7, 1.2);
    customize_TGraph(gr3, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 34, 95, 1.2);
    customize_TGraph(gr4, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 47, 52, 1.2);
    customize_TGraph(gr5, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 33, 7, 1.2);
    customize_TGraph(gr6, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 34, 95, 1.2);

    customize_TGraph(grc1, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 20, 2, 1.0);
    customize_TGraph(grc2, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 21, 4, 1.0);
    customize_TGraph(grc3, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 25, 6, 1.0);
    customize_TGraph(grc4, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 20, 2, 1.0);
    customize_TGraph(grc5, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 21, 4, 1.0);
    customize_TGraph(grc6, "v_{0} vs Centrality; Centrality [%]; v_{0}", 50.0, 70.0, 8e-3, 4e-2, 25, 6, 1.0);


    auto c = new TCanvas("c", "v0_centrality", 500, 500);

    auto leg_title = new TLegend(0.015, 0.97, 0.5, 0.71);
    leg_title->SetTextSize(0.0457);
    leg_title->AddEntry((TObject*)0, "Pb+Pb   #eta_{gap} = 1", "");
    leg_title->SetBorderSize(0);
    leg_title->SetFillStyle(0);

    auto leg_ptref = new TLegend(0.45, 0.43, 0.7, 0.13);
    leg_ptref->SetTextSize(0.023);
    leg_ptref->AddEntry((TObject*)0, "#bf{p_{T}^{ref} range}", "C");
    leg_ptref->AddEntry(gr1, "Without correction   0.5-2 GeV", "p");
    leg_ptref->AddEntry(gr2, "Without correction   0.5-5 GeV", "p");
    leg_ptref->AddEntry(gr3, "Without correction   1-5 GeV", "p");
    leg_ptref->AddEntry(grc1, "With correction   0.5-2 GeV", "p");
    leg_ptref->AddEntry(grc2, "With correction   0.5-5 GeV", "p");
    leg_ptref->AddEntry(grc3, "With correction   1-5 GeV", "p");
    leg_ptref->SetBorderSize(0);
    leg_ptref->SetFillStyle(0);

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
    c->SetLogy();   
    c->Update();
    c->SaveAs("CorrectionPlot1.pdf");
    delete c;
}

void DoPlot4(TString filename, TString cfilename){
    TFile *f;
    TFile *cf;
    f = TFile::Open(filename, "READ");
    cf = TFile::Open(cfilename, "READ");

    TGraph *gr_v0pt_red_nc = (TGraph*)f->Get("v0pt_red");
    TGraph *gr_v0pt_blue_nc = (TGraph*)f->Get("v0pt_blue");
    TGraph *gr_v0ptv0_red_nc = (TGraph*)f->Get("v0ptv0_red");
    TGraph *gr_v0ptv0_blue_nc = (TGraph*)f->Get("v0ptv0_blue");

    TGraph *gr_v0pt_red_c = (TGraph*)cf->Get("v0pt_red");
    TGraph *gr_v0pt_blue_c = (TGraph*)cf->Get("v0pt_blue");
    TGraph *gr_v0ptv0_red_c = (TGraph*)cf->Get("v0ptv0_red");
    TGraph *gr_v0ptv0_blue_c = (TGraph*)cf->Get("v0ptv0_blue");

    customize_TGraph(gr_v0pt_red_nc, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 47, 2, 1.0);
    customize_TGraph(gr_v0pt_blue_nc, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 34, 4, 1.0);
    customize_TGraph(gr_v0ptv0_red_nc, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 47, 2, 1.0);
    customize_TGraph(gr_v0ptv0_blue_nc, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 34, 4, 1.0);

    customize_TGraph(gr_v0pt_red_c, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 21, 3, 1.0);
    customize_TGraph(gr_v0pt_blue_c, "v_{0}(p_{T}) vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.42, 20, 5, 1.0);
    customize_TGraph(gr_v0ptv0_red_c, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 21, 3, 1.0);
    customize_TGraph(gr_v0ptv0_blue_c, "v_{0}(p_{T})/v_{0} vs p_{T}; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 28.0, 20, 5, 1.0);

    auto c = new TCanvas("c", "c", 850, 500);
    c->Divide(2, 1);

    // v0(pT) legend
    auto legend_v0pt_text = new TLegend(0.025, 0.89, 0.5, 0.76);
    legend_v0pt_text->SetTextSize(0.0457);
    legend_v0pt_text->AddEntry((TObject*)0, "CMS Open Data 2.76 TeV", "");
    legend_v0pt_text->AddEntry((TObject*)0, "Pb+Pb,   #eta_{gap} = 1   p_{T}^{ref} 0.5-2 GeV", "");
    legend_v0pt_text->SetBorderSize(0);
    legend_v0pt_text->SetFillStyle(0);

    auto legend_v0pt_cents = new TLegend(0.103, 0.748, 0.5, 0.57);
    legend_v0pt_cents->SetTextSize(0.0325);
    legend_v0pt_cents->AddEntry(gr_v0pt_red_nc, "Without correction (50-60%)", "p");
    legend_v0pt_cents->AddEntry(gr_v0pt_blue_nc, "Without correction (60-70%)", "p");
    legend_v0pt_cents->AddEntry(gr_v0pt_red_c, "With correction (50-60%)", "p");
    legend_v0pt_cents->AddEntry(gr_v0pt_blue_c, "With correction (50-60%)", "p");
    legend_v0pt_cents->SetBorderSize(0);
    legend_v0pt_cents->SetFillStyle(0);
    
    // v0(pT)/v0 legend
    auto legend_v0ptv0_text = new TLegend(0.025, 0.89, 0.5, 0.76);
    legend_v0ptv0_text->SetTextSize(0.0457);
    legend_v0ptv0_text->AddEntry((TObject*)0, "CMS Open Data 2.76 TeV", "");
    legend_v0ptv0_text->AddEntry((TObject*)0, "Pb+Pb,   #eta_{gap} = 1   p_{T}^{ref} 0.5-2 GeV", "");
    legend_v0ptv0_text->SetBorderSize(0);
    legend_v0ptv0_text->SetFillStyle(0);

    auto legend_v0ptv0_cents = new TLegend(0.103, 0.748, 0.5, 0.57);
    legend_v0ptv0_cents->SetTextSize(0.0325);
    legend_v0ptv0_cents->AddEntry(gr_v0pt_red_nc, "Without correction (50-60%)", "p");
    legend_v0ptv0_cents->AddEntry(gr_v0pt_blue_nc, "Without correction (60-70%)", "p");
    legend_v0ptv0_cents->AddEntry(gr_v0ptv0_red_c, "With correction (50-60%)", "p");
    legend_v0ptv0_cents->AddEntry(gr_v0ptv0_blue_c, "With correction (50-60%)", "p");
    legend_v0ptv0_cents->SetBorderSize(0);
    legend_v0ptv0_cents->SetFillStyle(0);

     // Drawing v0(pT) plot
    c->cd(1);
    gr_v0pt_red_c->Draw("AP");
    gr_v0pt_blue_c->Draw("P SAME");
    gr_v0pt_red_nc->Draw("P SAME");
    gr_v0pt_blue_nc->Draw("P SAME");
    legend_v0pt_text->Draw();
    legend_v0pt_cents->Draw();
    gPad->SetLogx();

    // Drawing v0(pT)/v0 plot
    c->cd(2);
    gr_v0ptv0_red_c->Draw("AP");
    gr_v0ptv0_blue_nc->Draw("P SAME");
    gr_v0ptv0_blue_c->Draw("P SAME");
    gr_v0ptv0_red_nc->Draw("P SAME");
    legend_v0ptv0_text->Draw();
    legend_v0ptv0_cents->Draw();
    gPad->SetLogx();

    // Saving canvas as pdf
    c->Update();
    c->SaveAs("CorrectionPlot4.pdf");
    delete c;
}