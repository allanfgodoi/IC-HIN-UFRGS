///To run it, please, do: root -l -b -q 'macro_doControlPlots.C("RootFileFullPath/RootFileName.root")'
///Example: root -l -b -q 'macro_doControlPlots.C("/Users/cesarbernardes/RootTreesD0mesons/tree_skim_MC_prompt.root")'


///Let's use RDataFrame :-) 
///Very good reference here: https://root.cern.ch/doc/v614/classROOT_1_1RDataFrame.html


void macro_doControlPlots(std::string filename = "/Users/cesarbernardes/D0_MC_SkimmedTrees/tree_skim_MC_prompt.root"){


auto fileName = filename;
auto treeName = "Dfinder/ntDkpi";

// We read the tree from the file and create a RDataFrame object called "d"
ROOT::RDataFrame d(treeName, fileName);

// Create histograms

auto h_centrality = d.Histo1D("centrality"); //for centrality variable

TCanvas tc_centrality("tc_centrality");
tc_centrality.cd(1);
h_centrality->Draw();
tc_centrality.Print("hist_centrality.pdf");


//Just to check the branches in the input trees
///Open the root file and list the columns of the tree
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

