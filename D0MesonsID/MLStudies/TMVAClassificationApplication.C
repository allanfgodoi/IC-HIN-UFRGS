#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TMath.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"

using namespace TMVA;

// RUN: root -l -b -q TMVAClassificationApplication.C\(\"BDT\"\)

void cd_config(TH1F* histBdt_all_x, TH1F* histBdt_signal_x, TH1F* histBdt_background_x, TLegend* leg_x, TPaveStats* st_all_x, TPaveStats* st_signal_x, TPaveStats* st_background_x){
    histBdt_all_x->SetLineColor(1);
    histBdt_all_x->SetMarkerColor(1);
    histBdt_all_x->SetLineWidth(2);
    histBdt_signal_x->SetLineColor(4);
    histBdt_signal_x->SetMarkerColor(4);
    histBdt_signal_x->SetLineWidth(2);
    histBdt_background_x->SetLineColor(2);
    histBdt_background_x->SetMarkerColor(2);
    histBdt_background_x->SetLineStyle(2);
    histBdt_background_x->SetLineWidth(2);
    gPad->SetLogy();
    histBdt_all_x->Draw();
    gPad->Update();
    st_all_x = (TPaveStats*)histBdt_all_x->FindObject("stats");
        st_all_x->SetY1NDC(0.75);
        st_all_x->SetY2NDC(0.90);
        st_all_x->SetX1NDC(0.80);
        st_all_x->SetX2NDC(1.00);
    histBdt_signal_x->Draw("sameS");
    gPad->Update();
    st_signal_x = (TPaveStats*)histBdt_signal_x->FindObject("stats");
        st_signal_x->SetY1NDC(0.45);
        st_signal_x->SetY2NDC(0.60);
        st_signal_x->SetX1NDC(0.80);
        st_signal_x->SetX2NDC(1.00);
    histBdt_background_x->Draw("sameS");
    gPad->Update();
    st_background_x = (TPaveStats*)histBdt_background_x->FindObject("stats");
        st_background_x->SetY1NDC(0.60);
        st_background_x->SetY2NDC(0.75);
        st_background_x->SetX1NDC(0.80);
        st_background_x->SetX2NDC(1.00);
    gPad->Update();
    leg_x = new TLegend(0.6,0.68,0.85,0.9);
    leg_x->AddEntry(histBdt_all_x,"All","l");
    leg_x->AddEntry(histBdt_signal_x,"Signal","l");
    leg_x->AddEntry(histBdt_background_x,"Background","l");
    leg_x->SetFillStyle(0);
    leg_x->SetBorderSize(0);
    leg_x->SetTextSize(0.045);
    leg_x->SetTextFont(42);
    leg_x->Draw();
}

void TMVAClassificationApplication( TString myMethodList = "" )
{
        
    //---------------------------------------------------------------
    // This loads the library
    TMVA::Tools::Instance();
    // Default MVA methods to be trained + tested
    std::map<std::string,int> Use;
    // Cut optimisation
    Use["Cuts"]            = 1;
    Use["CutsD"]           = 1;
    Use["CutsPCA"]         = 0;
    Use["CutsGA"]          = 0;
    Use["CutsSA"]          = 0;
    //
    // 1-dimensional likelihood ("naive Bayes estimator")
    Use["Likelihood"]      = 1;
    Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
    Use["LikelihoodPCA"]   = 1; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
    Use["LikelihoodKDE"]   = 0;
    Use["LikelihoodMIX"]   = 0;
    //
    // Mutidimensional likelihood and Nearest-Neighbour methods
    Use["PDERS"]           = 1;
    Use["PDERSD"]          = 0;
    Use["PDERSPCA"]        = 0;
    Use["PDEFoam"]         = 1;
    Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
    Use["KNN"]             = 1; // k-nearest neighbour method
    //
    // Linear Discriminant Analysis
    Use["LD"]              = 1; // Linear Discriminant identical to Fisher
    Use["Fisher"]          = 0;
    Use["FisherG"]         = 0;
    Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
    Use["HMatrix"]         = 0;
    //
    // Function Discriminant analysis
    Use["FDA_GA"]          = 1; // minimisation of user-defined function using Genetics Algorithm
    Use["FDA_SA"]          = 0;
    Use["FDA_MC"]          = 0;
    Use["FDA_MT"]          = 0;
    Use["FDA_GAMT"]        = 0;
    Use["FDA_MCMT"]        = 0;
    //
    // Neural Networks (all are feed-forward Multilayer Perceptrons)
    Use["MLP"]             = 0; // Recommended ANN
    Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
    Use["MLPBNN"]          = 1; // Recommended ANN with BFGS training method and bayesian regulator
    Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
    Use["TMlpANN"]         = 0; // ROOT's own ANN
    //
    // Support Vector Machine
    Use["SVM"]             = 1;
    //
    // Boosted Decision Trees
    Use["BDT"]             = 1; // uses Adaptive Boost
    Use["BDTG"]            = 0; // uses Gradient Boost
    Use["BDTB"]            = 0; // uses Bagging
    Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
    //
    // Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
    Use["RuleFit"]         = 1;
    // ---------------------------------------------------------------
    Use["Plugin"]          = 0;
    Use["Category"]        = 0;
    Use["SVM_Gauss"]       = 0;
    Use["SVM_Poly"]        = 0;
    Use["SVM_Lin"]         = 0;
    std::cout << std::endl;
    std::cout << "==> Start TMVAClassificationApplication" << std::endl;

    // Select methods (don't look at this code - not of interest)
    if (myMethodList != "") {
        for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;
        std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
        for (UInt_t i=0; i<mlist.size(); i++) {
            std::string regMethod(mlist[i]);
            if (Use.find(regMethod) == Use.end()) {
                std::cout << "Method \"" << regMethod
                        << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
                for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
                std::cout << it->first << " ";
                }
                std::cout << std::endl;
                return;
            }
            Use[regMethod] = 1;
        }
    }

    // --------------------------------------------------------------------------------------------------
    // Create the Reader object
    TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );

    // Create a set of variables and declare them to the reader
    // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
    Float_t D3DDecayLength, D3DDecayLengthSignificance, DTrk1Chi2n, DTrk1PtErr, DTrk2Chi2n, DVtxProb, D3DPointingAngle, DDca, DTtrk1Pt, DTrk2Pt, DTrk2PtErr, DTrk1Eta, DTrk2Eta, DxyDCASignificanceDaugther1, DxyDCASignificanceDaugther2, DzDCASignificanceDaugther1, DzDCASignificanceDaugther2, DMass, DPhi, DPt, DRapidity;
    reader->AddVariable("D3DDecayLength", &D3DDecayLength);
    reader->AddVariable("D3DDecayLengthSignificance", &D3DDecayLengthSignificance);
    reader->AddVariable("DTrk1Chi2n", &DTrk1Chi2n);
    reader->AddVariable("DTrk1PtErr", &DTrk1PtErr);
    reader->AddVariable("DTrk2Chi2n", &DTrk2Chi2n);
    reader->AddVariable("DVtxProb", &DVtxProb);
    reader->AddVariable("D3DPointingAngle", &D3DPointingAngle);
    reader->AddVariable("DDca", &DDca);
    reader->AddVariable("DTtrk1Pt", &DTtrk1Pt);
    reader->AddVariable("DTrk2Pt", &DTrk2Pt);
    reader->AddVariable("DTrk2PtErr", &DTrk2PtErr);
    reader->AddVariable("DTrk1Eta", &DTrk1Eta);
    reader->AddVariable("DTrk2Eta", &DTrk2Eta);
    reader->AddVariable("DxyDCASignificanceDaugther1", &DxyDCASignificanceDaugther1);
    reader->AddVariable("DxyDCASignificanceDaugther2", &DxyDCASignificanceDaugther2);
    reader->AddVariable("DzDCASignificanceDaugther1", &DzDCASignificanceDaugther1);
    reader->AddVariable("DzDCASignificanceDaugther2", &DzDCASignificanceDaugther2);
    reader->AddVariable("DMass", &DMass);
    reader->AddVariable("DPhi", &DPhi);
    reader->AddVariable("DPt", &DPt);
    reader->AddVariable("DRapidity", &DRapidity);

    // Spectator variables declared in the training have to be added to the reader, too
    Float_t DGenpt, DGenphi, DGeny, DGenIndex;
    reader->AddSpectator("DGenpt", &DGenpt);
    reader->AddSpectator("DGenphi", &DGenphi);
    reader->AddSpectator("DGeny", &DGeny);
    reader->AddSpectator("DGenIndex", &DGenIndex);

    // Book the MVA methods
    TString dir    = "/home/allanfgodoi/Desktop/";
    //TString dir    = "/Users/cesarbernardes/Dropbox/Ubuntu_1204/AltasEnergias/ProfessorUFRGS/OrientacaoDeAlunos/IC_TCC/ReposGit/IC-HIN-UFRGS/D0MesonsID/MLStudies/";
    TString prefix = "TMVAClassification";

    // Book method(s)
    for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
        if (it->second) {
            TString methodName = TString(it->first) + TString(" method");
            TString weightfile = dir + prefix + TString("_") + TString(it->first) + TString(".weights.xml");
            reader->BookMVA( methodName, weightfile );
        }
    }

    // Book output histograms
    UInt_t nbin = 10;
    TH1F *histBdt_all(0);
    TH1F *histBdt_signal(0);
    TH1F *histBdt_background(0);

    TH1F *histBdt_all_DMass(0);
    TH1F *histBdt_signal_DMass(0);
    TH1F *histBdt_background_DMass(0);

    TH1F *histBdt_all_DPhi(0);
    TH1F *histBdt_signal_DPhi(0);
    TH1F *histBdt_background_DPhi(0);

    TH1F *histBdt_all_DPt(0);
    TH1F *histBdt_signal_DPt(0);
    TH1F *histBdt_background_DPt(0);

    TH1F *histBdt_all_DRapidity(0);
    TH1F *histBdt_signal_DRapidity(0);
    TH1F *histBdt_background_DRapidity(0);

    if (Use["BDT"]) {
        histBdt_all = new TH1F("MVA_BDT_all", "MVA_BDT_all", nbin, -0.45, 0.25 );
        histBdt_signal = new TH1F("MVA_BDT_signal", "MVA_BDT_signal", nbin, -0.45, 0.25 );
        histBdt_background = new TH1F("MVA_BDT_background", "MVA_BDT_background", nbin, -0.45, 0.25 );

        histBdt_all_DMass = new TH1F("MVA_BDT_all_DMass", "MVA_BDT_all_DMass", nbin, -1, 1 );
        histBdt_signal_DMass = new TH1F("MVA_BDT_signal_DMass", "MVA_BDT_signal_DMass", nbin, -1, 1 );
        histBdt_background_DMass = new TH1F("MVA_BDT_background_DMass", "MVA_BDT_background_DMass", nbin, -1, 1 );

        histBdt_all_DPhi = new TH1F("MVA_BDT_all_DPhi", "MVA_BDT_all_DPhi", nbin, -1, 1 );
        histBdt_signal_DPhi = new TH1F("MVA_BDT_signal_DPhi", "MVA_BDT_signal_DPhi", nbin, -1, 1 );
        histBdt_background_DPhi = new TH1F("MVA_BDT_background_DPhi", "MVA_BDT_background_DPhi", nbin, -1, 1 );

        histBdt_all_DPt = new TH1F("MVA_BDT_all_DPt", "MVA_BDT_all_DPt", nbin, -1, 1 );
        histBdt_signal_DPt = new TH1F("MVA_BDT_signal_DPt", "MVA_BDT_signal_DPt", nbin, -1, 1 );
        histBdt_background_DPt = new TH1F("MVA_BDT_background_DPt", "MVA_BDT_background_DPt", nbin, -1, 1 );

        histBdt_all_DRapidity= new TH1F("MVA_BDT_all_DRapidity", "MVA_BDT_all_DRapidity", nbin, -1, 1 );
        histBdt_signal_DRapidity= new TH1F("MVA_BDT_signal_DRapidity", "MVA_BDT_signal_DRapidity", nbin, -1, 1 );
        histBdt_background_DRapidity= new TH1F("MVA_BDT_background_DRapidity", "MVA_BDT_background_DRapidity", nbin, -1, 1 );
    }

    // Prepare input tree (this must be replaced by your data source)
    // in this example, there is a toy tree with signal and one with background events
    // we'll later on use only the "signal" events for the test in this example.
    //
    TFile *input(0);
    TString fname = "/home/allanfgodoi/Desktop/tree_skim_MC_promptTest.root";
    //TString fname = "/Users/cesarbernardes/Dropbox/Ubuntu_1204/AltasEnergias/ProfessorUFRGS/OrientacaoDeAlunos/IC_TCC/TopicosDeEstudo/D0_Selection/D0_MC_SkimmedTrees/tree_skim_MC_prompt.root";
    if (!gSystem->AccessPathName( fname ))
        input = TFile::Open( fname ); // check if file in local directory exists
    else
        input = TFile::Open( "http://root.cern.ch/files/tmva_class_example.root" ); // if not: download from ROOT server
    if (!input) {
        std::cout << "ERROR: could not open data file" << std::endl;
        exit(1);
    }
    std::cout << "--- TMVAClassificationApp    : Using input file: " << input->GetName() << std::endl;

    // Event loop
    // Prepare the event tree
    // - Here the variable names have to corresponds to your tree
    // - You can use the same variables as above which is slightly faster,
    //   but of course you can use different ones and copy the values inside the event loop
    //
    std::cout << "--- Select signal+background sample" << std::endl;
    TTree* theTree = (TTree*)input->Get("Dfinder/ntDkpi");
    std::vector<float> * vec_D3DDecayLength = 0; 
    std::vector<float> * vec_D3DDecayLengthSignificance = 0;
    std::vector<float> * vec_DTrk1Chi2n = 0;
    std::vector<float> * vec_DTrk1PtErr = 0;
    std::vector<float> * vec_DTrk2Chi2n = 0;
    std::vector<float> * vec_DVtxProb = 0;
    std::vector<float> * vec_D3DPointingAngle = 0;
    std::vector<float> * vec_DDca = 0;
    std::vector<float> * vec_DTtrk1Pt = 0;
    std::vector<float> * vec_DTrk2Pt = 0;
    std::vector<float> * vec_DTrk2PtErr = 0;
    std::vector<float> * vec_DTrk1Eta = 0;
    std::vector<float> * vec_DTrk2Eta = 0;
    std::vector<float> * vec_DxyDCASignificanceDaugther1 = 0;
    std::vector<float> * vec_DxyDCASignificanceDaugther2 = 0;
    std::vector<float> * vec_DzDCASignificanceDaugther1 = 0;
    std::vector<float> * vec_DzDCASignificanceDaugther2 = 0;
    std::vector<float> * vec_DMass = 0;
    std::vector<float> * vec_DPhi = 0;
    std::vector<float> * vec_DPt = 0;
    std::vector<float> * vec_DRapidity = 0;
    std::vector<float> * vec_DGen = 0;
    
    theTree->SetBranchAddress("D3DDecayLength", &vec_D3DDecayLength);
    theTree->SetBranchAddress("D3DDecayLengthSignificance", &vec_D3DDecayLengthSignificance);
    theTree->SetBranchAddress("DTrk1Chi2n", &vec_DTrk1Chi2n);
    theTree->SetBranchAddress("DTrk1PtErr", &vec_DTrk1PtErr);
    theTree->SetBranchAddress("DTrk2Chi2n", &vec_DTrk2Chi2n);
    theTree->SetBranchAddress("DVtxProb", &vec_DVtxProb);
    theTree->SetBranchAddress("D3DPointingAngle", &vec_D3DPointingAngle);
    theTree->SetBranchAddress("DDca", &vec_DDca);
    theTree->SetBranchAddress("DTtrk1Pt", &vec_DTtrk1Pt);
    theTree->SetBranchAddress("DTrk2Pt", &vec_DTrk2Pt);
    theTree->SetBranchAddress("DTrk2PtErr", &vec_DTrk2PtErr);
    theTree->SetBranchAddress("DTrk1Eta", &vec_DTrk1Eta);
    theTree->SetBranchAddress("DTrk2Eta", &vec_DTrk2Eta);
    theTree->SetBranchAddress("DxyDCASignificanceDaugther1", &vec_DxyDCASignificanceDaugther1);
    theTree->SetBranchAddress("DxyDCASignificanceDaugther2", &vec_DxyDCASignificanceDaugther2);
    theTree->SetBranchAddress("DzDCASignificanceDaugther1", &vec_DzDCASignificanceDaugther1);
    theTree->SetBranchAddress("DzDCASignificanceDaugther2", &vec_DzDCASignificanceDaugther2);
    theTree->SetBranchAddress("DMass", &vec_DMass);
    theTree->SetBranchAddress("DPhi", &vec_DPhi);
    theTree->SetBranchAddress("DPt", &vec_DPt);
    theTree->SetBranchAddress("DRapidity", &vec_DRapidity);
    theTree->SetBranchAddress("DGen", &vec_DGen);

    std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
    TStopwatch sw;
    sw.Start();

    //this is a loop in the events (each collision)
    std::vector<float> aux_vec_all_trainingVariables;
    std::vector<float> aux_vec_DMass;
    std::vector<float> aux_vec_DPhi;
    std::vector<float> aux_vec_DPt;
    std::vector<float> aux_vec_DRapidity;
    for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {

        if (ievt%100000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;

        theTree->GetEntry(ievt);

	//this is a loop in the D0 meson candidates in a given event
	//NB.: here it can include signal and background 
        for(int iD0=0; iD0<vec_D3DDecayLength->size(); iD0++){
            //use same sequence of variables as in the weight file        
            aux_vec_all_trainingVariables.push_back((*vec_D3DDecayLength)[iD0]); 
	        if (!TMath::IsNaN((*vec_D3DDecayLengthSignificance)[iD0])) aux_vec_all_trainingVariables.push_back((*vec_D3DDecayLengthSignificance)[iD0]);
	        aux_vec_all_trainingVariables.push_back((*vec_DTrk1Chi2n)[iD0]);
	        aux_vec_all_trainingVariables.push_back((*vec_DTrk1PtErr)[iD0]);
	        aux_vec_all_trainingVariables.push_back((*vec_DTrk2Chi2n)[iD0]);
	        aux_vec_all_trainingVariables.push_back((*vec_DVtxProb)[iD0]);
	        aux_vec_all_trainingVariables.push_back((*vec_D3DPointingAngle)[iD0]);
	        aux_vec_all_trainingVariables.push_back((*vec_DDca)[iD0]);
            aux_vec_all_trainingVariables.push_back((*vec_DTtrk1Pt)[iD0]);
            aux_vec_all_trainingVariables.push_back((*vec_DTrk2Pt)[iD0]);
            aux_vec_all_trainingVariables.push_back((*vec_DTrk2PtErr)[iD0]);
            aux_vec_all_trainingVariables.push_back((*vec_DTrk1Eta)[iD0]);
            aux_vec_all_trainingVariables.push_back((*vec_DTrk2Eta)[iD0]);
	        if (!TMath::IsNaN((*vec_DxyDCASignificanceDaugther1)[iD0])) aux_vec_all_trainingVariables.push_back((*vec_DxyDCASignificanceDaugther1)[iD0]);
            if (!TMath::IsNaN((*vec_DxyDCASignificanceDaugther2)[iD0])) aux_vec_all_trainingVariables.push_back((*vec_DxyDCASignificanceDaugther2)[iD0]);
            if (!TMath::IsNaN((*vec_DzDCASignificanceDaugther1)[iD0])) aux_vec_all_trainingVariables.push_back((*vec_DzDCASignificanceDaugther1)[iD0]);
            if (!TMath::IsNaN((*vec_DzDCASignificanceDaugther2)[iD0])) aux_vec_all_trainingVariables.push_back((*vec_DzDCASignificanceDaugther2)[iD0]);
            aux_vec_DMass.push_back((*vec_DMass)[iD0]);
            aux_vec_DPhi.push_back((*vec_DPhi)[iD0]);
            aux_vec_DPt.push_back((*vec_DPt)[iD0]);
            aux_vec_DRapidity.push_back((*vec_DRapidity)[iD0]);

            // Return the MVA outputs and fill into histograms
	        // See method here: https://root.cern.ch/root/html608/Reader_8cxx_source.html#l00486

            if (Use["BDT"]) {
                histBdt_all->Fill(reader->EvaluateMVA(aux_vec_all_trainingVariables,"BDT method"));
                histBdt_all_DMass->Fill(reader->EvaluateMVA(aux_vec_DMass,"BDT method"));
                histBdt_all_DPhi->Fill(reader->EvaluateMVA(aux_vec_DPhi,"BDT method"));
                histBdt_all_DPt->Fill(reader->EvaluateMVA(aux_vec_DPt,"BDT method"));
                histBdt_all_DRapidity->Fill(reader->EvaluateMVA(aux_vec_DRapidity,"BDT method"));

                if ((*vec_DGen)[iD0]==23333 || (*vec_DGen)[iD0]==23344){
                    histBdt_signal->Fill(reader->EvaluateMVA(aux_vec_all_trainingVariables,"BDT method"));
                    histBdt_signal_DMass->Fill(reader->EvaluateMVA(aux_vec_DMass,"BDT method"));
                    histBdt_signal_DPhi->Fill(reader->EvaluateMVA(aux_vec_DPhi,"BDT method"));
                    histBdt_signal_DPt->Fill(reader->EvaluateMVA(aux_vec_DPt,"BDT method"));
                    histBdt_signal_DRapidity->Fill(reader->EvaluateMVA(aux_vec_DRapidity,"BDT method"));
                }

                if ((*vec_DGen)[iD0]!=23333 && (*vec_DGen)[iD0]!=23344){
                    histBdt_background->Fill(reader->EvaluateMVA(aux_vec_all_trainingVariables,"BDT method"));
                    histBdt_background_DMass->Fill(reader->EvaluateMVA(aux_vec_DMass,"BDT method"));
                    histBdt_background_DPhi->Fill(reader->EvaluateMVA(aux_vec_DPhi,"BDT method"));
                    histBdt_background_DPt->Fill(reader->EvaluateMVA(aux_vec_DPt,"BDT method"));
                    histBdt_background_DRapidity->Fill(reader->EvaluateMVA(aux_vec_DRapidity,"BDT method"));
                }
            }
            //clean up the vectors to fill next D0 meson information
            aux_vec_all_trainingVariables.clear();	
            aux_vec_DMass.clear();
            aux_vec_DPhi.clear();
            aux_vec_DPt.clear();
            aux_vec_DRapidity.clear();
	} 
    }

    // Get elapsed time
    sw.Stop();
    std::cout << "--- End of event loop: "; sw.Print();

    // Create TCanvas and write histograms
    
    TCanvas c1("c1","c1", 1000, 1000);
    c1.Divide(2, 2, 0.01, 0.01);
    c1.cd(1);
    histBdt_all->Draw();
    c1.cd(2);
    histBdt_signal->Draw();
    c1.cd(3);
    histBdt_background->Draw();

    TCanvas ck("ck", "ck", 1000, 1000);
        ck.Divide(2,2,0.01,0.01);
        ck.cd(1);
            TLegend* leg_DMass; TPaveStats* st_all_DMass; TPaveStats* st_signal_DMass; TPaveStats* st_background_DMass;
            cd_config(histBdt_all_DMass, histBdt_signal_DMass, histBdt_background_DMass, leg_DMass, st_all_DMass, st_signal_DMass, st_background_DMass);
        
        ck.cd(2);
            TLegend* leg_DPhi; TPaveStats* st_all_DPhi; TPaveStats* st_signal_DPhi; TPaveStats* st_background_DPhi;
            cd_config(histBdt_all_DPhi, histBdt_signal_DPhi, histBdt_background_DPhi, leg_DPhi, st_all_DPhi, st_signal_DPhi, st_background_DPhi);
        ck.cd(3);
            TLegend* leg_DPt; TPaveStats* st_all_DPt; TPaveStats* st_signal_DPt; TPaveStats* st_background_DPt;
            cd_config(histBdt_all_DPt, histBdt_signal_DPt, histBdt_background_DPt, leg_DPt, st_all_DPt, st_signal_DPt, st_background_DPt);
        ck.cd(4);
            TLegend *leg_DRapidity; TPaveStats* st_all_DRapidity; TPaveStats* st_signal_DRapidity; TPaveStats* st_background_DRapidity;
            cd_config(histBdt_all_DRapidity, histBdt_signal_DRapidity, histBdt_background_DRapidity, leg_DRapidity, st_all_DRapidity, st_signal_DRapidity, st_background_DRapidity);


    // Write histograms in .root file
    TFile *target  = new TFile( "TMVApp.root","RECREATE" );
    if (Use["BDT"]) {
        histBdt_all->Write(); histBdt_signal->Write(); histBdt_background->Write();
        histBdt_all_DMass->Write(); histBdt_signal_DMass->Write(); histBdt_background_DMass->Write();
        histBdt_all_DPhi->Write(); histBdt_signal_DPhi->Write(); histBdt_background_DPhi->Write();
        histBdt_all_DPt->Write(); histBdt_signal_DPt->Write(); histBdt_background_DPt->Write();
        histBdt_all_DRapidity->Write(); histBdt_signal_DRapidity->Write(); histBdt_background_DRapidity->Write();
        c1.Write(); ck.Write();
    }

    target->Close();
    std::cout << "--- Created root file: \"TMVApp.root\" containing the MVA output histograms" << std::endl;
    delete reader;
    std::cout << "==> TMVAClassificationApplication is done!" << std::endl << std::endl;
}

int main( int argc, char** argv ) {
    TString methodList;
    for (int i=1; i<argc; i++) {
        TString regMethod(argv[i]);
        if(regMethod=="-b" || regMethod=="--batch") continue;
        if (!methodList.IsNull()) methodList += TString(",");
        methodList += regMethod;
    }
    TMVAClassificationApplication(methodList);
    return 0;
}
