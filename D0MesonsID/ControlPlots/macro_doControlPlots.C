///To run it, please, do: root -l -b -q 'macro_doControlPlots.C("RootFileFullPath/RootFileName.root")'
///Example: root -l -b -q 'macro_doControlPlots.C("/Users/cesarbernardes/RootTreesD0mesons/tree_skim_MC_prompt.root")'


///Let's use RDataFrame :-) 
///Reference here: https://root.cern.ch/doc/v614/classROOT_1_1RDataFrame.html


void macro_doControlPlots(std::string filename = "/Users/cesarbernardes/D0_MC_SkimmedTrees/tree_skim_MC_prompt.root"){


auto fileName = filename;
auto treeName = "Dfinder/ntDkpi";

// We read the tree from the file and create a RDataFrame object called "d"
ROOT::RDataFrame d(treeName, fileName);

// Book histograms

///common y-axis label
auto yaxis_label1 = "Number of Events / Bin";
auto yaxis_label2 = "Total Number of Particles / Bin";

auto h_centrality = d.Define("cent","centrality/2.0")
	             .Histo1D(TH1D("Centrality","Centrality",100,0,100),"cent"); //for centrality variable
h_centrality->GetYaxis()->SetTitle(yaxis_label1);
h_centrality->GetXaxis()->SetTitle("Centrality (%)");
h_centrality->SetLineColor(2);
h_centrality->SetLineWidth(2);

auto h_PVz = d.Histo1D(TH1D("PVz","Z Position of Primary Vertex",100,-25,25),"PVz"); //for particle primary vertex Z position
h_PVz->GetYaxis()->SetTitle(yaxis_label1);
h_PVz->GetXaxis()->SetTitle("PV Z Position (cm)");
h_PVz->SetLineColor(2);
h_PVz->SetLineWidth(2);

auto h_PVx = d.Histo1D(TH1D("PVx","X Position of Primary Vertex",100,0,0.07),"PVx"); //for particle primary vertex X position
h_PVx->GetYaxis()->SetTitle(yaxis_label1);
h_PVx->GetXaxis()->SetTitle("PV X Position (cm)");
h_PVx->SetLineColor(2);
h_PVx->SetLineWidth(2);

auto h_PVy = d.Histo1D(TH1D("PVy","Y Position of Primary Vertex",100,0,0.07),"PVy"); //for particle primary vertex Y position
h_PVy->GetYaxis()->SetTitle(yaxis_label1);
h_PVy->GetXaxis()->SetTitle("PV Y Position (cm)");
h_PVy->SetLineColor(2);
h_PVy->SetLineWidth(2);

auto h_PVpos = d.Histo3D(TH3D("PV3DPos","3D Position of Primary Vertex",100,-0.07,0.07,100,-0.07,0.07,100,-25,25),"PVx","PVy","PVz"); //for particle primary vertex 3D position
h_PVpos->GetZaxis()->SetTitle("PV Z Position (cm)");
h_PVpos->GetYaxis()->SetTitle("PV Y Position (cm)");
h_PVpos->GetXaxis()->SetTitle("PV X Position (cm)");


// Draw and save histograms

TCanvas tc_globalObservables("tc_globalObservables","GlobalObservables",1000,1000);
tc_globalObservables.Divide(2,2,0,0);
tc_globalObservables.cd(1);
gPad->SetTickx(2);
gPad->SetTicky(2);
h_centrality->Draw();
tc_globalObservables.cd(2);
gPad->SetTickx(2);
gPad->SetTicky(2);
h_PVz->GetYaxis()->SetLabelOffset(0.01);
h_PVz->Draw();
tc_globalObservables.cd(3);
gPad->SetTickx(2);
gPad->SetTicky(2);
h_PVx->Draw();
tc_globalObservables.cd(4);
gPad->SetTickx(2);
gPad->SetTicky(2);
h_PVy->GetYaxis()->SetLabelOffset(0.01);
h_PVy->Draw();
tc_globalObservables.Print("hist_globalObservables.pdf");


TCanvas tc_PVpos("tc_PVpos","PVPosition3D",500,500);
tc_PVpos.cd(1);
h_PVpos->Draw("BOX2Z");
tc_PVpos.Print("hist_3D_PVposition.pdf");


//Just to check the branches in the input trees
//Open the root file and list the columns of the tree
/*
TString fName = (TString) filename;
TFile f1(fName);
auto t = f1.Get<TTree>(treeName); //get root tree
std::cout << "These are the columns of the tree:" << std::endl;
for (auto branch : *t->GetListOfBranches()) {
   std::cout << "Branch: " << branch->GetName() << std::endl;	
}
f1.Close();
*/


}

