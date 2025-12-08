# RUN WITH:
# pyroot PredictionPlots.py funcName

import numpy as np
import ROOT as root
import sys

def customize_TGraphErrors(g, title, xmin, xmax, ymin, ymax, style, color, size):
    g.SetTitle(title)
    g.GetXaxis().SetLimits(xmin, xmax) 
    g.GetXaxis().CenterTitle()          
    g.GetYaxis().CenterTitle()         
    g.SetMinimum(ymin)
    g.SetMaximum(ymax)
    g.SetMarkerStyle(style)
    g.SetMarkerColor(color)
    g.SetLineColor(color)
    g.SetMarkerSize(size)

def RescaleX(g, mean_pt):
    n = g.GetN()
    x_buffer = g.GetX()

    for i in range(n):
        x_buffer[i] = x_buffer[i]/mean_pt


def doPlot_v0pt():
    predic = np.load("./Data/CE_MAP_predictions_v0_data.npz", allow_pickle=True)
    ptlist = predic["ptlist"]
    v0pT = predic["v0pT"].item()
    v0pT_err = predic["v0pT_err"].item()

    N = len(ptlist)

    f = root.TFile.Open("./Data/Figures/main.root", "READ")

    c = root.TCanvas("c", "Plots", 1900, 900)
    c.Divide(2, 1)
    c.cd(1)

    g_5060_data = f.Get("v0pt_ptref_1_5060")
    customize_TGraphErrors(g_5060_data, "50-60% Centrality; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.45, 21, 2, 1.0)
    g_5060_data.Draw("AP")

    g_5060_predic = root.TGraphErrors(N, ptlist, v0pT["50-60%"], np.zeros(N), v0pT_err["50-60%"])
    g_5060_predic.SetFillStyle(1001)
    g_5060_predic.SetFillColorAlpha(2, 0.3)
    g_5060_predic.SetLineColor(2)
    g_5060_predic.Draw("3L SAME")

    l_5060 = root.TLegend(0.16, 0.8, 0.5, 0.9) 
    l_5060.SetBorderSize(0)
    l_5060.SetFillStyle(0)
    l_5060.SetTextSize(0.033)
    l_5060.AddEntry(g_5060_predic, "JETSCAPE MAP CE PbPb 2.76 TeV", "lf") 
    l_5060.AddEntry(g_5060_data, "CMS Open Data PbPb 2.76 TeV", "pe")       
    l_5060.Draw()

    root.gPad.SetTopMargin(0.075)
    root.gPad.SetLeftMargin(0.12)
    root.gPad.SetRightMargin(0.02)
    root.gPad.SetLogx()

    c.cd(2)

    g_6070_data = f.Get("v0pt_ptref_1_6070")
    customize_TGraphErrors(g_6070_data, "60-70% Centrality; p_{T} [GeV]; v_{0}(p_{T})", 0.0, 10.0, -0.1, 0.63, 20, 4, 1.0)
    g_6070_data.Draw("AP SAME")

    g_6070_predic = root.TGraphErrors(N, ptlist, v0pT["60-70%"], np.zeros(N), v0pT_err["60-70%"])
    g_6070_predic.SetFillStyle(1001)
    g_6070_predic.SetFillColorAlpha(4, 0.3)
    g_6070_predic.SetLineColor(4)
    g_6070_predic.Draw("3L SAME")

    l_6070 = root.TLegend(0.16, 0.8, 0.5, 0.9) 
    l_6070.SetBorderSize(0)
    l_6070.SetFillStyle(0)
    l_6070.SetTextSize(0.033)
    l_6070.AddEntry(g_6070_predic, "JETSCAPE MAP CE PbPb 2.76 TeV", "lf") 
    l_6070.AddEntry(g_6070_data, "CMS Open Data PbPb 2.76 TeV", "pe")       
    l_6070.Draw()

    root.gPad.SetTopMargin(0.075)
    root.gPad.SetLeftMargin(0.12)
    root.gPad.SetRightMargin(0.02)
    root.gPad.SetLogx()

    c.SaveAs("test.pdf")

def doPlot_ratio():
    predic = np.load("./Data/CE_MAP_predictions_v0_data.npz", allow_pickle=True)
    ptlist = predic["ptlist"]
    ratio = predic["ratio"].item()
    ratio_err = predic["ratio_err"].item()

    N = len(ptlist)

    f = root.TFile.Open("./Data/Figures/main.root", "READ")
    p = f.Get("mean_pt_5060")
    mean_pt = p.GetVal()

    c = root.TCanvas("c", "Plots", 1900, 900)
    c.Divide(2, 1)
    c.cd(1)

    g_5060_data = f.Get("sv0pt_ptref_1_5060")
    customize_TGraphErrors(g_5060_data, " ; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 22.0, 20, 1, 1.0)
    g_5060_data.Draw("AP")

    '''
    ATLAS v0(pT)/v0 values

    f2 = root.TFile.Open("./Data/Figures_ATLAS/sv0pt_ATLAS_5060.root", "READ")
    g2 = f2.Get("Figure 4b_cent7/Graph1D_y1")
    g2.Draw("P SAME")
    '''

    g_5060_predic = root.TGraphErrors(N, ptlist, ratio["50-60%"], np.zeros(N), ratio_err["50-60%"])
    g_5060_predic.SetFillStyle(1001)
    g_5060_predic.SetFillColorAlpha(6, 0.3)
    g_5060_predic.SetLineColor(6)
    g_5060_predic.Draw("3L SAME")

    l_5060 = root.TLegend(0.16, 0.8, 0.5, 0.9) 
    l_5060.SetBorderSize(0)
    l_5060.SetFillStyle(0)
    l_5060.SetTextSize(0.033)
    l_5060.AddEntry(g_5060_predic, "JETSCAPE MAP CE PbPb 2.76 TeV", "lf") 
    l_5060.AddEntry(g_5060_data, "CMS Open Data PbPb 2.76 TeV", "pe")       
    l_5060.Draw()

    root.gPad.SetTopMargin(0.075)
    root.gPad.SetLeftMargin(0.12)
    root.gPad.SetRightMargin(0.02)
    root.gPad.SetLogx()

    c.cd(2)

    g_5060_data_rescaled = f.Get("sv0pt_ptref_1_5060")
    RescaleX(g_5060_data_rescaled, mean_pt)
    customize_TGraphErrors(g_5060_data_rescaled, " ; p_{T} [GeV]; v_{0}(p_{T})/v_{0}", 0.0, 10.0, -4.0, 22.0, 20, 1, 1.0)
    g_5060_data_rescaled.Draw("AP")

    print(g_5060_data.GetX()[0])
    print(g_5060_data_rescaled.GetX()[0])

    g_5060_predic_rescaled = root.TGraphErrors(N, ptlist/mean_pt, ratio["50-60%"], np.zeros(N), ratio_err["50-60%"])
    g_5060_predic_rescaled.SetFillStyle(1001)
    g_5060_predic_rescaled.SetFillColorAlpha(6, 0.3)
    g_5060_predic_rescaled.SetLineColor(6)
    g_5060_predic_rescaled.Draw("3L SAME")

    l_5060_rescaled = root.TLegend(0.16, 0.8, 0.5, 0.9) 
    l_5060_rescaled.SetBorderSize(0)
    l_5060_rescaled.SetFillStyle(0)
    l_5060_rescaled.SetTextSize(0.033)
    l_5060_rescaled.AddEntry(g_5060_predic_rescaled, "JETSCAPE MAP CE PbPb 2.76 TeV", "lf") 
    l_5060_rescaled.AddEntry(g_5060_data_rescaled, "CMS Open Data PbPb 2.76 TeV", "pe")       
    l_5060_rescaled.Draw()

    root.gPad.SetTopMargin(0.075)
    root.gPad.SetLeftMargin(0.12)
    root.gPad.SetRightMargin(0.02)
    root.gPad.SetLogx()

    c.SaveAs("Fig5.pdf")


if __name__ == "__main__":
    plot = sys.argv[1]
    if plot == "doPlot_v0pt":
        doPlot_v0pt()
    if plot == "doPlot_ratio":
        doPlot_ratio()