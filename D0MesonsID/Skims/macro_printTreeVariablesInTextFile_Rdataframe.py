###To run it do: << python macro_printTreeVariablesInTextFile_Rdataframe.py >> 

import ROOT

# We prepare an input tree to run on
fileName = "/Users/cesarbernardes/Dropbox/Ubuntu_1204/AltasEnergias/ProfessorUFRGS/OrientacaoDeAlunos/IC_TCC/TopicosDeEstudo/D0_Selection/D0_MC_SkimmedTrees/tree_skim_MC_prompt.root"
treeName = "Dfinder/ntDkpi"
outFileNameTest = "tree_print_text.root"

# We read the tree from the file and create a RDataFrame
d = ROOT.RDataFrame(treeName, fileName) 

# Auxiliar functions to define variables as vector<float> instead of ROOT::VecOps::RVec<Float_t> default format from RDataframe
getVector_code ='''
std::vector<float> getVectorFloat_Aux(ROOT::VecOps::RVec<Float_t> vec_, ROOT::VecOps::RVec<Float_t> vec_dgen_){
   std::vector<float> v;
   for(int i=0; i<vec_.size(); i++){
      if(vec_dgen_[i]==23333 || vec_dgen_[i]==23344) v.push_back(vec_[i]); //select signal only
   }
   return v;
}
'''
ROOT.gInterpreter.Declare(getVector_code)


d_ = d.Define("Dgen","getVectorFloat_Aux(DGen,DGen)") \
      .Define("Dtrk1Pt","getVectorFloat_Aux(DTtrk1Pt,DGen)") \
      .Define("Dtrk2Pt","getVectorFloat_Aux(DTrk2Pt,DGen)")


d_.Snapshot(treeName, outFileNameTest, {"Dtrk1Pt","Dtrk2Pt","Dgen"})
