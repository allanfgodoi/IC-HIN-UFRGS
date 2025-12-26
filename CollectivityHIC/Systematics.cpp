#include "ObsConstructor.h"

void TrackSelection(){
    
    cout << "Evaluating nominal 50-60%" << endl; // Calculating nominal 50-60%
    ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "noCorrec", 0.0, "nominal", "./Data/Systematics/TrackSelectionData.root", "obs_ptref", "Nominal", 15.0);
    cout << "Evaluating nominal 60-70%" << endl; // Calculating nominal 60-70%
    ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "noCorrec", 0.0, "nominal", "./Data/Systematics/TrackSelectionData.root", "obs_ptref", "Nominal", 15.0);

    cout << "Evaluating loose 50-60%" << endl; // Calculating loose 50-60%
    ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "noCorrec", 0.0, "loose", "./Data/Systematics/TrackSelectionData.root", "obs_ptref", "Loose", 15.0);
    cout << "Evaluating loose 60-70%" << endl;// Calculating loose 60-70%
    ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "noCorrec", 0.0, "loose", "./Data/Systematics/TrackSelectionData.root", "obs_ptref", "Loose", 15.0);

    cout << "Evaluating tight 50-60%" << endl; // Calculating tight 50-60%
    ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "noCorrec", 0.0, "tight", "./Data/Systematics/TrackSelectionData.root", "obs_ptref", "Tight", 15.0);
    cout << "Evaluating tight 60-70%" << endl; // Calculating tight 60-70%
    ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "noCorrec", 0.0, "tight", "./Data/Systematics/TrackSelectionData.root", "obs_ptref", "Tight", 15.0);

    TFile *f = TFile::Open("./Data/Systematics/TrackSelectionData.root", "READ");

    // Calculating v0 difference
        TGraphErrors *g_v0_55_nominal = (TGraphErrors*)f->Get("v0_55_nominal");
        TGraphErrors *g_v0_55_loose = (TGraphErrors*)f->Get("v0_55_loose");
        TGraphErrors *g_v0_55_tight = (TGraphErrors*)f->Get("v0_55_tight");
        TGraphErrors *g_v0_65_nominal = (TGraphErrors*)f->Get("v0_65_nominal");
        TGraphErrors *g_v0_65_loose = (TGraphErrors*)f->Get("v0_65_loose");
        TGraphErrors *g_v0_65_tight = (TGraphErrors*)f->Get("v0_65_tight");

        Int_t N_v0 = 2;
        Double_t *y_v0_55_nominal = g_v0_55_nominal->GetY();
        Double_t *y_v0_65_nominal = g_v0_65_nominal->GetY();
        Double_t *y_v0_55_loose = g_v0_55_loose->GetY();
        Double_t *y_v0_65_loose = g_v0_65_loose->GetY();
        Double_t *y_v0_55_tight = g_v0_55_tight->GetY();
        Double_t *y_v0_65_tight = g_v0_65_tight->GetY();

        vector<Double_t> vec_x_v0(N_v0, 0.0);
        vector<Double_t> vec_y_v0_loose(N_v0, 0.0);
        vector<Double_t> vec_y_v0_tight(N_v0, 0.0);

        vec_x_v0[0] = 55.0; vec_x_v0[1] = 65.0;
        vec_y_v0_loose[0] = abs(y_v0_55_loose[0] - y_v0_55_nominal[0]); vec_y_v0_loose[1] = abs(y_v0_65_loose[0] - y_v0_65_nominal[0]);
        vec_y_v0_tight[0] = abs(y_v0_55_tight[0] - y_v0_55_nominal[0]); vec_y_v0_tight[1] = abs(y_v0_65_tight[0] - y_v0_65_nominal[0]);

        TGraph* gd_v0_loose = new TGraph(N_v0, vec_x_v0.data(), vec_y_v0_loose.data());
        TGraph* gd_v0_tight = new TGraph(N_v0, vec_x_v0.data(), vec_y_v0_tight.data());

    // Calculating v0(pT) difference
        TGraphErrors *g_v0pt_5060_nominal = (TGraphErrors*)f->Get("v0pt_ptref_nominal_5060");
        TGraphErrors *g_v0pt_6070_nominal = (TGraphErrors*)f->Get("v0pt_ptref_nominal_6070");
        TGraphErrors *g_v0pt_5060_loose = (TGraphErrors*)f->Get("v0pt_ptref_loose_5060");
        TGraphErrors *g_v0pt_6070_loose = (TGraphErrors*)f->Get("v0pt_ptref_loose_6070");
        TGraphErrors *g_v0pt_5060_tight = (TGraphErrors*)f->Get("v0pt_ptref_tight_5060");
        TGraphErrors *g_v0pt_6070_tight = (TGraphErrors*)f->Get("v0pt_ptref_tight_6070");

        Int_t N_v0pt = g_v0pt_5060_nominal->GetN();
        Double_t *x_v0pt = g_v0pt_5060_nominal->GetX();
        Double_t *y_v0pt_5060_nominal = g_v0pt_5060_nominal->GetY();
        Double_t *y_v0pt_6070_nominal = g_v0pt_6070_nominal->GetY();
        Double_t *y_v0pt_5060_loose = g_v0pt_5060_loose->GetY();
        Double_t *y_v0pt_6070_loose = g_v0pt_6070_loose->GetY();
        Double_t *y_v0pt_5060_tight = g_v0pt_5060_tight->GetY();
        Double_t *y_v0pt_6070_tight = g_v0pt_6070_tight->GetY();

        vector<Double_t> vec_x_v0pt(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_loose(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_loose(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_tight(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_tight(N_v0pt, 0.0);

        for (int i=0; i<N_v0pt; i++){
            vec_x_v0pt[i] = x_v0pt[i];
            vec_y_v0pt_5060_loose[i] = abs(y_v0pt_5060_loose[i] - y_v0pt_5060_nominal[i]);
            vec_y_v0pt_6070_loose[i] = abs(y_v0pt_6070_loose[i] - y_v0pt_6070_nominal[i]);
            vec_y_v0pt_5060_tight[i] = abs(y_v0pt_5060_tight[i] - y_v0pt_5060_nominal[i]);
            vec_y_v0pt_6070_tight[i] = abs(y_v0pt_6070_tight[i] - y_v0pt_6070_nominal[i]);
        }

        TGraph* gd_v0pt_5060_loose = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_loose.data());
        TGraph* gd_v0pt_6070_loose = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_loose.data());
        TGraph* gd_v0pt_5060_tight = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_tight.data());
        TGraph* gd_v0pt_6070_tight = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_tight.data());

    // Calculating v0(pT)/v0 difference
        TGraphErrors *g_sv0pt_5060_nominal = (TGraphErrors*)f->Get("sv0pt_ptref_nominal_5060");
        TGraphErrors *g_sv0pt_6070_nominal = (TGraphErrors*)f->Get("sv0pt_ptref_nominal_6070");
        TGraphErrors *g_sv0pt_5060_loose = (TGraphErrors*)f->Get("sv0pt_ptref_loose_5060");
        TGraphErrors *g_sv0pt_6070_loose = (TGraphErrors*)f->Get("sv0pt_ptref_loose_6070");
        TGraphErrors *g_sv0pt_5060_tight = (TGraphErrors*)f->Get("sv0pt_ptref_tight_5060");
        TGraphErrors *g_sv0pt_6070_tight = (TGraphErrors*)f->Get("sv0pt_ptref_tight_6070");

        Int_t N_sv0pt = g_sv0pt_5060_nominal->GetN();
        Double_t *x_sv0pt = g_sv0pt_5060_nominal->GetX();
        Double_t *y_sv0pt_5060_nominal = g_sv0pt_5060_nominal->GetY();
        Double_t *y_sv0pt_6070_nominal = g_sv0pt_6070_nominal->GetY();
        Double_t *y_sv0pt_5060_loose = g_sv0pt_5060_loose->GetY();
        Double_t *y_sv0pt_6070_loose = g_sv0pt_6070_loose->GetY();
        Double_t *y_sv0pt_5060_tight = g_sv0pt_5060_tight->GetY();
        Double_t *y_sv0pt_6070_tight = g_sv0pt_6070_tight->GetY();

        vector<Double_t> vec_x_sv0pt(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_loose(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_loose(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_tight(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_tight(N_sv0pt, 0.0);

        for (int i=0; i<N_sv0pt; i++){
            vec_x_sv0pt[i] = x_sv0pt[i];
            vec_y_sv0pt_5060_loose[i] = abs(y_sv0pt_5060_loose[i] - y_sv0pt_5060_nominal[i]);
            vec_y_sv0pt_6070_loose[i] = abs(y_sv0pt_6070_loose[i] - y_sv0pt_6070_nominal[i]);
            vec_y_sv0pt_5060_tight[i] = abs(y_sv0pt_5060_tight[i] - y_sv0pt_5060_nominal[i]);
            vec_y_sv0pt_6070_tight[i] = abs(y_sv0pt_6070_tight[i] - y_sv0pt_6070_nominal[i]);
        }

        TGraph* gd_sv0pt_5060_loose = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_loose.data());
        TGraph* gd_sv0pt_6070_loose = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_loose.data());
        TGraph* gd_sv0pt_5060_tight = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_tight.data());
        TGraph* gd_sv0pt_6070_tight = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_tight.data());

    f->Close();

    // Saving TGraphs
    TFile *sf = new TFile("./Data/Systematics/TrackSelectionUnc.root", "UPDATE");

    // v0
    gd_v0_loose->SetName("gd_v0_loose"); gd_v0_loose->Write();
    gd_v0_tight->SetName("gd_v0_tight"); gd_v0_tight->Write();
    // v0(pT)
    gd_v0pt_5060_loose->SetName("gd_v0pt_5060_loose"); gd_v0pt_5060_loose->Write();
    gd_v0pt_6070_loose->SetName("gd_v0pt_6070_loose"); gd_v0pt_6070_loose->Write();
    gd_v0pt_5060_tight->SetName("gd_v0pt_5060_tight"); gd_v0pt_5060_tight->Write();
    gd_v0pt_6070_tight->SetName("gd_v0pt_6070_tight"); gd_v0pt_6070_tight->Write();
    // v0(pT)/v0
    gd_sv0pt_5060_loose->SetName("gd_sv0pt_5060_loose"); gd_sv0pt_5060_loose->Write();
    gd_sv0pt_6070_loose->SetName("gd_sv0pt_6070_loose"); gd_sv0pt_6070_loose->Write();
    gd_sv0pt_5060_tight->SetName("gd_sv0pt_5060_tight"); gd_sv0pt_5060_tight->Write();
    gd_sv0pt_6070_tight->SetName("gd_sv0pt_6070_tight"); gd_sv0pt_6070_tight->Write();
    
    sf->Close();
}

void CentralityFluctuations(){

    cout << "Evaluating zero fluctuation 50-60%" << endl; 
    ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "Correc", 0.0, "zero", "./Data/Systematics/CentralityFluctuationData.root", "obs_ptref", "Nominal", 15.0);
    cout << "Evaluating zero fluctuation 60-70%" << endl; 
    ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "Correc", 0.0, "zero", "./Data/Systematics/CentralityFluctuationData.root", "obs_ptref", "Nominal", 15.0);

    cout << "Evaluating positive fluctuation 50-60%" << endl; 
    ObsConstructor(1.0, 210.0, 375.0, 0.01, 0.5, 2.0, "Correc", 0.0, "positive", "./Data/Systematics/CentralityFluctuationData.root", "obs_ptref", "Nominal", 15.0);
    cout << "Evaluating positive fluctuation 60-70%" << endl;
    ObsConstructor(1.0, 100.0, 210.0, 0.01, 0.5, 2.0, "Correc", 0.0, "positive", "./Data/Systematics/CentralityFluctuationData.root", "obs_ptref", "Nominal", 15.0);

    cout << "Evaluating negative fluctuation 50-60%" << endl; 
    ObsConstructor(1.0, 210.0, 375.0, -0.01, 0.5, 2.0, "Correc", 0.0, "negative", "./Data/Systematics/CentralityFluctuationData.root", "obs_ptref", "Nominal", 15.0);
    cout << "Evaluating negative fluctuation 60-70%" << endl;
    ObsConstructor(1.0, 100.0, 210.0, -0.01, 0.5, 2.0, "Correc", 0.0, "negative", "./Data/Systematics/CentralityFluctuationData.root", "obs_ptref", "Nominal", 15.0);

    TFile *f = TFile::Open("./Data/Systematics/CentralityFluctuationData.root", "READ");

    // Calculating v0 difference
        TGraphErrors *g_v0_55_zero = (TGraphErrors*)f->Get("v0_55_zero");
        TGraphErrors *g_v0_55_positive = (TGraphErrors*)f->Get("v0_55_positive");
        TGraphErrors *g_v0_55_negative = (TGraphErrors*)f->Get("v0_55_negative");
        TGraphErrors *g_v0_65_zero = (TGraphErrors*)f->Get("v0_65_zero");
        TGraphErrors *g_v0_65_positive = (TGraphErrors*)f->Get("v0_65_positive");
        TGraphErrors *g_v0_65_negative = (TGraphErrors*)f->Get("v0_65_negative");

        Int_t N_v0 = 2;
        Double_t *y_v0_55_zero = g_v0_55_zero->GetY();
        Double_t *y_v0_65_zero = g_v0_65_zero->GetY();
        Double_t *y_v0_55_positive = g_v0_55_positive->GetY();
        Double_t *y_v0_65_positive = g_v0_65_positive->GetY();
        Double_t *y_v0_55_negative = g_v0_55_negative->GetY();
        Double_t *y_v0_65_negative = g_v0_65_negative->GetY();

        vector<Double_t> vec_x_v0(N_v0, 0.0);
        vector<Double_t> vec_y_v0_positive(N_v0, 0.0);
        vector<Double_t> vec_y_v0_negative(N_v0, 0.0);

        vec_x_v0[0] = 55.0; vec_x_v0[1] = 65.0;
        vec_y_v0_positive[0] = abs(y_v0_55_positive[0] - y_v0_55_zero[0]); vec_y_v0_positive[1] = abs(y_v0_65_positive[0] - y_v0_65_zero[0]);
        vec_y_v0_negative[0] = abs(y_v0_55_negative[0] - y_v0_55_zero[0]); vec_y_v0_negative[1] = abs(y_v0_65_negative[0] - y_v0_65_zero[0]);

        TGraph* gd_v0_positive = new TGraph(N_v0, vec_x_v0.data(), vec_y_v0_positive.data());
        TGraph* gd_v0_negative = new TGraph(N_v0, vec_x_v0.data(), vec_y_v0_negative.data());

    // Calculating v0(pT) difference
        TGraphErrors *g_v0pt_5060_zero = (TGraphErrors*)f->Get("v0pt_ptref_zero_5060");
        TGraphErrors *g_v0pt_6070_zero = (TGraphErrors*)f->Get("v0pt_ptref_zero_6070");
        TGraphErrors *g_v0pt_5060_positive = (TGraphErrors*)f->Get("v0pt_ptref_positive_5060");
        TGraphErrors *g_v0pt_6070_positive = (TGraphErrors*)f->Get("v0pt_ptref_positive_6070");
        TGraphErrors *g_v0pt_5060_negative = (TGraphErrors*)f->Get("v0pt_ptref_negative_5060");
        TGraphErrors *g_v0pt_6070_negative = (TGraphErrors*)f->Get("v0pt_ptref_negative_6070");

        Int_t N_v0pt = g_v0pt_5060_zero->GetN();
        Double_t *x_v0pt = g_v0pt_5060_zero->GetX();
        Double_t *y_v0pt_5060_zero = g_v0pt_5060_zero->GetY();
        Double_t *y_v0pt_6070_zero = g_v0pt_6070_zero->GetY();
        Double_t *y_v0pt_5060_positive = g_v0pt_5060_positive->GetY();
        Double_t *y_v0pt_6070_positive = g_v0pt_6070_positive->GetY();
        Double_t *y_v0pt_5060_negative = g_v0pt_5060_negative->GetY();
        Double_t *y_v0pt_6070_negative = g_v0pt_6070_negative->GetY();

        vector<Double_t> vec_x_v0pt(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_positive(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_positive(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_negative(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_negative(N_v0pt, 0.0);

        for (int i=0; i<N_v0pt; i++){
            vec_x_v0pt[i] = x_v0pt[i];
            vec_y_v0pt_5060_positive[i] = abs(y_v0pt_5060_positive[i] - y_v0pt_5060_zero[i]);
            vec_y_v0pt_6070_positive[i] = abs(y_v0pt_6070_positive[i] - y_v0pt_6070_zero[i]);
            vec_y_v0pt_5060_negative[i] = abs(y_v0pt_5060_negative[i] - y_v0pt_5060_zero[i]);
            vec_y_v0pt_6070_negative[i] = abs(y_v0pt_6070_negative[i] - y_v0pt_6070_zero[i]);
        }

        TGraph* gd_v0pt_5060_positive = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_positive.data());
        TGraph* gd_v0pt_6070_positive = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_positive.data());
        TGraph* gd_v0pt_5060_negative = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_negative.data());
        TGraph* gd_v0pt_6070_negative = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_negative.data());

    // Calculating v0(pT)/v0 difference
        TGraphErrors *g_sv0pt_5060_zero = (TGraphErrors*)f->Get("sv0pt_ptref_zero_5060");
        TGraphErrors *g_sv0pt_6070_zero = (TGraphErrors*)f->Get("sv0pt_ptref_zero_6070");
        TGraphErrors *g_sv0pt_5060_positive = (TGraphErrors*)f->Get("sv0pt_ptref_positive_5060");
        TGraphErrors *g_sv0pt_6070_positive = (TGraphErrors*)f->Get("sv0pt_ptref_positive_6070");
        TGraphErrors *g_sv0pt_5060_negative = (TGraphErrors*)f->Get("sv0pt_ptref_negative_5060");
        TGraphErrors *g_sv0pt_6070_negative = (TGraphErrors*)f->Get("sv0pt_ptref_negative_6070");

        Int_t N_sv0pt = g_sv0pt_5060_zero->GetN();
        Double_t *x_sv0pt = g_sv0pt_5060_zero->GetX();
        Double_t *y_sv0pt_5060_zero = g_sv0pt_5060_zero->GetY();
        Double_t *y_sv0pt_6070_zero = g_sv0pt_6070_zero->GetY();
        Double_t *y_sv0pt_5060_positive = g_sv0pt_5060_positive->GetY();
        Double_t *y_sv0pt_6070_positive = g_sv0pt_6070_positive->GetY();
        Double_t *y_sv0pt_5060_negative = g_sv0pt_5060_negative->GetY();
        Double_t *y_sv0pt_6070_negative = g_sv0pt_6070_negative->GetY();

        vector<Double_t> vec_x_sv0pt(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_positive(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_positive(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_negative(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_negative(N_sv0pt, 0.0);

        for (int i=0; i<N_sv0pt; i++){
            vec_x_sv0pt[i] = x_sv0pt[i];
            vec_y_sv0pt_5060_positive[i] = abs(y_sv0pt_5060_positive[i] - y_sv0pt_5060_zero[i]);
            vec_y_sv0pt_6070_positive[i] = abs(y_sv0pt_6070_positive[i] - y_sv0pt_6070_zero[i]);
            vec_y_sv0pt_5060_negative[i] = abs(y_sv0pt_5060_negative[i] - y_sv0pt_5060_zero[i]);
            vec_y_sv0pt_6070_negative[i] = abs(y_sv0pt_6070_negative[i] - y_sv0pt_6070_zero[i]);
        }

        TGraph* gd_sv0pt_5060_positive = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_positive.data());
        TGraph* gd_sv0pt_6070_positive = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_positive.data());
        TGraph* gd_sv0pt_5060_negative = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_negative.data());
        TGraph* gd_sv0pt_6070_negative = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_negative.data());

    f->Close();

    // Saving TGraphs
    TFile *sf = new TFile("./Data/Systematics/CentralityFluctuationUnc.root", "UPDATE");

    // v0
    gd_v0_positive->SetName("gd_v0_positive"); gd_v0_positive->Write();
    gd_v0_negative->SetName("gd_v0_negative"); gd_v0_negative->Write();
    // v0(pT)
    gd_v0pt_5060_positive->SetName("gd_v0pt_5060_positive"); gd_v0pt_5060_positive->Write();
    gd_v0pt_6070_positive->SetName("gd_v0pt_6070_positive"); gd_v0pt_6070_positive->Write();
    gd_v0pt_5060_negative->SetName("gd_v0pt_5060_negative"); gd_v0pt_5060_negative->Write();
    gd_v0pt_6070_negative->SetName("gd_v0pt_6070_negative"); gd_v0pt_6070_negative->Write();
    // v0(pT)/v0
    gd_sv0pt_5060_positive->SetName("gd_sv0pt_5060_positive"); gd_sv0pt_5060_positive->Write();
    gd_sv0pt_6070_positive->SetName("gd_sv0pt_6070_positive"); gd_sv0pt_6070_positive->Write();
    gd_sv0pt_5060_negative->SetName("gd_sv0pt_5060_negative"); gd_sv0pt_5060_negative->Write();
    gd_sv0pt_6070_negative->SetName("gd_sv0pt_6070_negative"); gd_sv0pt_6070_negative->Write();
    
    sf->Close();
}

void CorrectionFluctuation(){

    /*
    cout << "Evaluating zero fluctuation 50-60%" << endl; 
    ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "Correc", 0.0, "zero", "./Data/Systematics/CorrectionFluctuationData.root", "obs_ptref", "Nominal", 15.0);
    cout << "Evaluating zero fluctuation 60-70%" << endl; // Calculating nominal 60-70%
    ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "Correc", 0.0, "zero", "./Data/Systematics/CorrectionFluctuationData.root", "obs_ptref", "Nominal", 15.0);

    cout << "Evaluating positive fluctuation 50-60%" << endl; 
    ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "Correc", 0.039, "positive", "./Data/Systematics/CorrectionFluctuationData.root", "obs_ptref", "Nominal", 15.0);
    cout << "Evaluating positive fluctuation 60-70%" << endl;
    ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "Correc", 0.039, "positive", "./Data/Systematics/CorrectionFluctuationData.root", "obs_ptref", "Nominal", 15.0);

    cout << "Evaluating negative fluctuation 50-60%" << endl; 
    ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "Correc", -0.039, "negative", "./Data/Systematics/CorrectionFluctuationData.root", "obs_ptref", "Nominal", 15.0);
    cout << "Evaluating negative fluctuation 60-70%" << endl; 
    ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "Correc", -0.039, "negative", "./Data/Systematics/CorrectionFluctuationData.root", "obs_ptref", "Nominal", 15.0);
    */

    TFile *f = TFile::Open("./Data/Systematics/CorrectionFluctuationData.root", "READ");

    // Calculating v0 difference
        TGraphErrors *g_v0_55_zero = (TGraphErrors*)f->Get("v0_55_zero");
        TGraphErrors *g_v0_55_positive = (TGraphErrors*)f->Get("v0_55_positive");
        TGraphErrors *g_v0_55_negative = (TGraphErrors*)f->Get("v0_55_negative");
        TGraphErrors *g_v0_65_zero = (TGraphErrors*)f->Get("v0_65_zero");
        TGraphErrors *g_v0_65_positive = (TGraphErrors*)f->Get("v0_65_positive");
        TGraphErrors *g_v0_65_negative = (TGraphErrors*)f->Get("v0_65_negative");

        Int_t N_v0 = 2;
        Double_t *y_v0_55_zero = g_v0_55_zero->GetY();
        Double_t *y_v0_65_zero = g_v0_65_zero->GetY();
        Double_t *y_v0_55_positive = g_v0_55_positive->GetY();
        Double_t *y_v0_65_positive = g_v0_65_positive->GetY();
        Double_t *y_v0_55_negative = g_v0_55_negative->GetY();
        Double_t *y_v0_65_negative = g_v0_65_negative->GetY();

        vector<Double_t> vec_x_v0(N_v0, 0.0);
        vector<Double_t> vec_y_v0_positive(N_v0, 0.0);
        vector<Double_t> vec_y_v0_negative(N_v0, 0.0);

        vec_x_v0[0] = 55.0; vec_x_v0[1] = 65.0;
        vec_y_v0_positive[0] = abs(y_v0_55_positive[0] - y_v0_55_zero[0]); vec_y_v0_positive[1] = abs(y_v0_65_positive[0] - y_v0_65_zero[0]);
        vec_y_v0_negative[0] = abs(y_v0_55_negative[0] - y_v0_55_zero[0]); vec_y_v0_negative[1] = abs(y_v0_65_negative[0] - y_v0_65_zero[0]);

        TGraph* gd_v0_positive = new TGraph(N_v0, vec_x_v0.data(), vec_y_v0_positive.data());
        TGraph* gd_v0_negative = new TGraph(N_v0, vec_x_v0.data(), vec_y_v0_negative.data());

    // Calculating v0(pT) difference
        TGraphErrors *g_v0pt_5060_zero = (TGraphErrors*)f->Get("v0pt_ptref_zero_5060");
        TGraphErrors *g_v0pt_6070_zero = (TGraphErrors*)f->Get("v0pt_ptref_zero_6070");
        TGraphErrors *g_v0pt_5060_positive = (TGraphErrors*)f->Get("v0pt_ptref_positive_5060");
        TGraphErrors *g_v0pt_6070_positive = (TGraphErrors*)f->Get("v0pt_ptref_positive_6070");
        TGraphErrors *g_v0pt_5060_negative = (TGraphErrors*)f->Get("v0pt_ptref_negative_5060");
        TGraphErrors *g_v0pt_6070_negative = (TGraphErrors*)f->Get("v0pt_ptref_negative_6070");

        Int_t N_v0pt = g_v0pt_5060_zero->GetN();
        Double_t *x_v0pt = g_v0pt_5060_zero->GetX();
        Double_t *y_v0pt_5060_zero = g_v0pt_5060_zero->GetY();
        Double_t *y_v0pt_6070_zero = g_v0pt_6070_zero->GetY();
        Double_t *y_v0pt_5060_positive = g_v0pt_5060_positive->GetY();
        Double_t *y_v0pt_6070_positive = g_v0pt_6070_positive->GetY();
        Double_t *y_v0pt_5060_negative = g_v0pt_5060_negative->GetY();
        Double_t *y_v0pt_6070_negative = g_v0pt_6070_negative->GetY();

        vector<Double_t> vec_x_v0pt(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_positive(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_positive(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_negative(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_negative(N_v0pt, 0.0);

        for (int i=0; i<N_v0pt; i++){
            vec_x_v0pt[i] = x_v0pt[i];
            vec_y_v0pt_5060_positive[i] = abs(y_v0pt_5060_positive[i] - y_v0pt_5060_zero[i]);
            vec_y_v0pt_6070_positive[i] = abs(y_v0pt_6070_positive[i] - y_v0pt_6070_zero[i]);
            vec_y_v0pt_5060_negative[i] = abs(y_v0pt_5060_negative[i] - y_v0pt_5060_zero[i]);
            vec_y_v0pt_6070_negative[i] = abs(y_v0pt_6070_negative[i] - y_v0pt_6070_zero[i]);
        }

        TGraph* gd_v0pt_5060_positive = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_positive.data());
        TGraph* gd_v0pt_6070_positive = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_positive.data());
        TGraph* gd_v0pt_5060_negative = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_negative.data());
        TGraph* gd_v0pt_6070_negative = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_negative.data());

    // Calculating v0(pT)/v0 difference
        TGraphErrors *g_sv0pt_5060_zero = (TGraphErrors*)f->Get("sv0pt_ptref_zero_5060");
        TGraphErrors *g_sv0pt_6070_zero = (TGraphErrors*)f->Get("sv0pt_ptref_zero_6070");
        TGraphErrors *g_sv0pt_5060_positive = (TGraphErrors*)f->Get("sv0pt_ptref_positive_5060");
        TGraphErrors *g_sv0pt_6070_positive = (TGraphErrors*)f->Get("sv0pt_ptref_positive_6070");
        TGraphErrors *g_sv0pt_5060_negative = (TGraphErrors*)f->Get("sv0pt_ptref_negative_5060");
        TGraphErrors *g_sv0pt_6070_negative = (TGraphErrors*)f->Get("sv0pt_ptref_negative_6070");

        Int_t N_sv0pt = g_sv0pt_5060_zero->GetN();
        Double_t *x_sv0pt = g_sv0pt_5060_zero->GetX();
        Double_t *y_sv0pt_5060_zero = g_sv0pt_5060_zero->GetY();
        Double_t *y_sv0pt_6070_zero = g_sv0pt_6070_zero->GetY();
        Double_t *y_sv0pt_5060_positive = g_sv0pt_5060_positive->GetY();
        Double_t *y_sv0pt_6070_positive = g_sv0pt_6070_positive->GetY();
        Double_t *y_sv0pt_5060_negative = g_sv0pt_5060_negative->GetY();
        Double_t *y_sv0pt_6070_negative = g_sv0pt_6070_negative->GetY();

        vector<Double_t> vec_x_sv0pt(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_positive(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_positive(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_negative(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_negative(N_sv0pt, 0.0);

        for (int i=0; i<N_sv0pt; i++){
            vec_x_sv0pt[i] = x_sv0pt[i];
            vec_y_sv0pt_5060_positive[i] = abs(y_sv0pt_5060_positive[i] - y_sv0pt_5060_zero[i]);
            vec_y_sv0pt_6070_positive[i] = abs(y_sv0pt_6070_positive[i] - y_sv0pt_6070_zero[i]);
            vec_y_sv0pt_5060_negative[i] = abs(y_sv0pt_5060_negative[i] - y_sv0pt_5060_zero[i]);
            vec_y_sv0pt_6070_negative[i] = abs(y_sv0pt_6070_negative[i] - y_sv0pt_6070_zero[i]);
        }

        TGraph* gd_sv0pt_5060_positive = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_positive.data());
        TGraph* gd_sv0pt_6070_positive = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_positive.data());
        TGraph* gd_sv0pt_5060_negative = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_negative.data());
        TGraph* gd_sv0pt_6070_negative = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_negative.data());

    f->Close();

    // Saving TGraphs
    TFile *sf = new TFile("./Data/Systematics/CorrectionFluctuationUnc.root", "UPDATE");

    // v0
    gd_v0_positive->SetName("gd_v0_positive"); gd_v0_positive->Write();
    gd_v0_negative->SetName("gd_v0_negative"); gd_v0_negative->Write();
    // v0(pT)
    gd_v0pt_5060_positive->SetName("gd_v0pt_5060_positive"); gd_v0pt_5060_positive->Write();
    gd_v0pt_6070_positive->SetName("gd_v0pt_6070_positive"); gd_v0pt_6070_positive->Write();
    gd_v0pt_5060_negative->SetName("gd_v0pt_5060_negative"); gd_v0pt_5060_negative->Write();
    gd_v0pt_6070_negative->SetName("gd_v0pt_6070_negative"); gd_v0pt_6070_negative->Write();
    // v0(pT)/v0
    gd_sv0pt_5060_positive->SetName("gd_sv0pt_5060_positive"); gd_sv0pt_5060_positive->Write();
    gd_sv0pt_6070_positive->SetName("gd_sv0pt_6070_positive"); gd_sv0pt_6070_positive->Write();
    gd_sv0pt_5060_negative->SetName("gd_sv0pt_5060_negative"); gd_sv0pt_5060_negative->Write();
    gd_sv0pt_6070_negative->SetName("gd_sv0pt_6070_negative"); gd_sv0pt_6070_negative->Write();
    
    sf->Close();
}

void ZvertexPosition(){

    cout << "Evaluating |pvZ| < 15 cm 50-60%" << endl; // default
    ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "noCorrec", 0.0, "pvZ_15", "./Data/Systematics/ZvertexPositionData.root", "obs_ptref", "Nominal", 15.0);
    cout << "Evaluating |pvZ| < 15 cm 60-70%" << endl; // default
    ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "noCorrec", 0.0, "pvZ_15", "./Data/Systematics/ZvertexPositionData.root", "obs_ptref", "Nominal", 15.0);

    cout << "Evaluating |pvZ| < 3 cm 50-60%" << endl; // default
    ObsConstructor(1.0, 210.0, 375.0, 0.0, 0.5, 2.0, "noCorrec", 0.0, "pvZ_3", "./Data/Systematics/ZvertexPositionData.root", "obs_ptref", "Nominal", 3.0);
    cout << "Evaluating |pvZ| < 3 cm 60-70%" << endl; // default
    ObsConstructor(1.0, 100.0, 210.0, 0.0, 0.5, 2.0, "noCorrec", 0.0, "pvZ_3", "./Data/Systematics/ZvertexPositionData.root", "obs_ptref", "Nominal", 3.0);

    TFile *f = TFile::Open("./Data/Systematics/ZvertexPositionData.root", "READ");

    // Calculating v0 difference
        TGraphErrors *g_v0_55_pvZ_15 = (TGraphErrors*)f->Get("v0_55_pvZ_15");
        TGraphErrors *g_v0_55_pvZ_3 = (TGraphErrors*)f->Get("v0_55_pvZ_3");
        TGraphErrors *g_v0_65_pvZ_15 = (TGraphErrors*)f->Get("v0_65_pvZ_15");
        TGraphErrors *g_v0_65_pvZ_3 = (TGraphErrors*)f->Get("v0_65_pvZ_3");

        Int_t N_v0 = 2;
        Double_t *y_v0_55_pvZ_15 = g_v0_55_pvZ_15->GetY();
        Double_t *y_v0_65_pvZ_15 = g_v0_65_pvZ_15->GetY();
        Double_t *y_v0_55_pvZ_3 = g_v0_55_pvZ_3->GetY();
        Double_t *y_v0_65_pvZ_3 = g_v0_65_pvZ_3->GetY();

        vector<Double_t> vec_x_v0(N_v0, 0.0);
        vector<Double_t> vec_y_v0_pvZ_3(N_v0, 0.0);

        vec_x_v0[0] = 55.0; vec_x_v0[1] = 65.0;
        vec_y_v0_pvZ_3[0] = abs(y_v0_55_pvZ_3[0] - y_v0_55_pvZ_15[0]); vec_y_v0_pvZ_3[1] = abs(y_v0_65_pvZ_3[0] - y_v0_65_pvZ_15[0]);

        TGraph* gd_v0_pvZ_3 = new TGraph(N_v0, vec_x_v0.data(), vec_y_v0_pvZ_3.data());

    // Calculating v0(pT) difference
        TGraphErrors *g_v0pt_5060_pvZ_15 = (TGraphErrors*)f->Get("v0pt_ptref_pvZ_15_5060");
        TGraphErrors *g_v0pt_6070_pvZ_15 = (TGraphErrors*)f->Get("v0pt_ptref_pvZ_15_6070");
        TGraphErrors *g_v0pt_5060_pvZ_3 = (TGraphErrors*)f->Get("v0pt_ptref_pvZ_3_5060");
        TGraphErrors *g_v0pt_6070_pvZ_3 = (TGraphErrors*)f->Get("v0pt_ptref_pvZ_3_6070");

        Int_t N_v0pt = g_v0pt_5060_pvZ_15->GetN();
        Double_t *x_v0pt = g_v0pt_5060_pvZ_15->GetX();
        Double_t *y_v0pt_5060_pvZ_15 = g_v0pt_5060_pvZ_15->GetY();
        Double_t *y_v0pt_6070_pvZ_15 = g_v0pt_6070_pvZ_15->GetY();
        Double_t *y_v0pt_5060_pvZ_3 = g_v0pt_5060_pvZ_3->GetY();
        Double_t *y_v0pt_6070_pvZ_3 = g_v0pt_6070_pvZ_3->GetY();

        vector<Double_t> vec_x_v0pt(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_5060_pvZ_3(N_v0pt, 0.0);
        vector<Double_t> vec_y_v0pt_6070_pvZ_3(N_v0pt, 0.0);

        for (int i=0; i<N_v0pt; i++){
            vec_x_v0pt[i] = x_v0pt[i];
            vec_y_v0pt_5060_pvZ_3[i] = abs(y_v0pt_5060_pvZ_3[i] - y_v0pt_5060_pvZ_15[i]);
            vec_y_v0pt_6070_pvZ_3[i] = abs(y_v0pt_6070_pvZ_3[i] - y_v0pt_6070_pvZ_15[i]);
        }

        TGraph* gd_v0pt_5060_pvZ_3 = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_5060_pvZ_3.data());
        TGraph* gd_v0pt_6070_pvZ_3 = new TGraph(N_v0pt, vec_x_v0pt.data(), vec_y_v0pt_6070_pvZ_3.data());

    // Calculating v0(pT)/v0 difference
        TGraphErrors *g_sv0pt_5060_pvZ_15 = (TGraphErrors*)f->Get("sv0pt_ptref_pvZ_15_5060");
        TGraphErrors *g_sv0pt_6070_pvZ_15 = (TGraphErrors*)f->Get("sv0pt_ptref_pvZ_15_6070");
        TGraphErrors *g_sv0pt_5060_pvZ_3 = (TGraphErrors*)f->Get("sv0pt_ptref_pvZ_3_5060");
        TGraphErrors *g_sv0pt_6070_pvZ_3 = (TGraphErrors*)f->Get("sv0pt_ptref_pvZ_3_6070");

        Int_t N_sv0pt = g_sv0pt_5060_pvZ_15->GetN();
        Double_t *x_sv0pt = g_sv0pt_5060_pvZ_15->GetX();
        Double_t *y_sv0pt_5060_pvZ_15 = g_sv0pt_5060_pvZ_15->GetY();
        Double_t *y_sv0pt_6070_pvZ_15 = g_sv0pt_6070_pvZ_15->GetY();
        Double_t *y_sv0pt_5060_pvZ_3 = g_sv0pt_5060_pvZ_3->GetY();
        Double_t *y_sv0pt_6070_pvZ_3 = g_sv0pt_6070_pvZ_3->GetY();

        vector<Double_t> vec_x_sv0pt(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_5060_pvZ_3(N_sv0pt, 0.0);
        vector<Double_t> vec_y_sv0pt_6070_pvZ_3(N_sv0pt, 0.0);

        for (int i=0; i<N_sv0pt; i++){
            vec_x_sv0pt[i] = x_sv0pt[i];
            vec_y_sv0pt_5060_pvZ_3[i] = abs(y_sv0pt_5060_pvZ_3[i] - y_sv0pt_5060_pvZ_15[i]);
            vec_y_sv0pt_6070_pvZ_3[i] = abs(y_sv0pt_6070_pvZ_3[i] - y_sv0pt_6070_pvZ_15[i]);
        }

        TGraph* gd_sv0pt_5060_pvZ_3 = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_5060_pvZ_3.data());
        TGraph* gd_sv0pt_6070_pvZ_3 = new TGraph(N_sv0pt, vec_x_v0pt.data(), vec_y_sv0pt_6070_pvZ_3.data());

    f->Close();

    // Saving TGraphs
    TFile *sf = new TFile("./Data/Systematics/ZvertexPositionUnc.root", "UPDATE");

    // v0
    gd_v0_pvZ_3->SetName("gd_v0_pvZ_3"); gd_v0_pvZ_3->Write();
    // v0(pT)
    gd_v0pt_5060_pvZ_3->SetName("gd_v0pt_5060_pvZ_3"); gd_v0pt_5060_pvZ_3->Write();
    gd_v0pt_6070_pvZ_3->SetName("gd_v0pt_6070_pvZ_3"); gd_v0pt_6070_pvZ_3->Write();
    // v0(pT)/v0
    gd_sv0pt_5060_pvZ_3->SetName("gd_sv0pt_5060_pvZ_3"); gd_sv0pt_5060_pvZ_3->Write();
    gd_sv0pt_6070_pvZ_3->SetName("gd_sv0pt_6070_pvZ_3"); gd_sv0pt_6070_pvZ_3->Write();
    
    sf->Close();
}