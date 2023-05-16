###To run it do: << python macro_divideTree_Rdataframe.py >> 

import ROOT

# We prepare an input tree to run on
fileName = "/Users/cesarbernardes/Dropbox/Ubuntu_1204/AltasEnergias/ProfessorUFRGS/OrientacaoDeAlunos/IC_TCC/TopicosDeEstudo/D0_Selection/D0_MC_SkimmedTrees/tree_skim_MC_prompt.root"
treeName = "Dfinder/ntDkpi"
outFileNameTrain = "tree_skim_MC_promptTrain.root"
outFileNameTest = "tree_skim_MC_promptTest.root"

# We read the tree from the file and create a RDataFrame
d = ROOT.RDataFrame(treeName, fileName) 

# Auxiliar functions to define variables as vector<float> instead of ROOT::VecOps::RVec<Float_t> default format from RDataframe
getVector_code ='''
std::vector<float> getVectorFloat_Aux(ROOT::VecOps::RVec<Float_t> vec_){
   std::vector<float> v;
   for(int i=0; i<vec_.size(); i++){
      v.push_back(vec_[i]);
   }
   return v;
}
'''
ROOT.gInterpreter.Declare(getVector_code)

##select first 500k entries 
d_train = d.Range(500000) \
           .Define("Dpt","getVectorFloat_Aux(DPt)") \
           .Define("Drapidity","getVectorFloat_Aux(DRapidity)") \
           .Define("Dmass","getVectorFloat_Aux(DMass)") \
           .Define("Dphi","getVectorFloat_Aux(DPhi)") \
           .Define("D2ddecayLength","getVectorFloat_Aux(D2DDecayLength)") \
           .Define("Dd0Err","getVectorFloat_Aux(DD0Err)") \
           .Define("D3ddecayLength","getVectorFloat_Aux(D3DDecayLength)") \
           .Define("DdxyzErr","getVectorFloat_Aux(DDxyzErr)") \
           .Define("D3ddecayLengthSignificance","getVectorFloat_Aux(D3DDecayLengthSignificance)") \
           .Define("D2ddecayLengthSignificance","getVectorFloat_Aux(D2DDecayLengthSignificance)") \
           .Define("DvtxProb","getVectorFloat_Aux(DVtxProb)") \
           .Define("D3dpointingAngle","getVectorFloat_Aux(D3DPointingAngle)") \
           .Define("D2dpointingAngle","getVectorFloat_Aux(D2DPointingAngle)") \
           .Define("Ddca","getVectorFloat_Aux(DDca)") \
           .Define("DimpactParameter3D","getVectorFloat_Aux(DImpactParameter3D)") \
           .Define("DimpactParameter3Derror","getVectorFloat_Aux(DImpactParameter3Derror)") \
           .Define("Dtrk1Pt","getVectorFloat_Aux(DTtrk1Pt)") \
           .Define("Dtrk2Pt","getVectorFloat_Aux(DTrk2Pt)") \
           .Define("Dtrk1PtErr","getVectorFloat_Aux(DTrk1PtErr)") \
           .Define("Dtrk2PtErr","getVectorFloat_Aux(DTrk2PtErr)") \
           .Define("Dtrk1Eta","getVectorFloat_Aux(DTrk1Eta)") \
           .Define("Dtrk2Eta","getVectorFloat_Aux(DTrk2Eta)") \
           .Define("DxydcaSignificanceDaugther1","getVectorFloat_Aux(DxyDCASignificanceDaugther1)") \
           .Define("DxydcaSignificanceDaugther2","getVectorFloat_Aux(DxyDCASignificanceDaugther2)") \
           .Define("DzdcaSignificanceDaugther1","getVectorFloat_Aux(DzDCASignificanceDaugther1)") \
           .Define("DzdcaSignificanceDaugther2","getVectorFloat_Aux(DzDCASignificanceDaugther2)") \
           .Define("Dtrk1Chi2n","getVectorFloat_Aux(DTrk1Chi2n)") \
           .Define("Dtrk2Chi2n","getVectorFloat_Aux(DTrk2Chi2n)") \
           .Define("Dgen","getVectorFloat_Aux(DGen)") \
           .Define("DgenIndex","getVectorFloat_Aux(DGenIndex)") \
           .Define("DgennDa","getVectorFloat_Aux(DGennDa)") \
           .Define("Dgenpt","getVectorFloat_Aux(DGenpt)") \
           .Define("Dgenphi","getVectorFloat_Aux(DGenphi)") \
           .Define("Dgeny","getVectorFloat_Aux(DGeny)") \

##from 500001 till the end of dataset, which has around 1M events
d_test = d.Range(500001,0) \
          .Define("Dpt","getVectorFloat_Aux(DPt)") \
          .Define("Drapidity","getVectorFloat_Aux(DRapidity)") \
          .Define("Dmass","getVectorFloat_Aux(DMass)") \
          .Define("Dphi","getVectorFloat_Aux(DPhi)") \
          .Define("D2ddecayLength","getVectorFloat_Aux(D2DDecayLength)") \
          .Define("Dd0Err","getVectorFloat_Aux(DD0Err)") \
          .Define("D3ddecayLength","getVectorFloat_Aux(D3DDecayLength)") \
          .Define("DdxyzErr","getVectorFloat_Aux(DDxyzErr)") \
          .Define("D3ddecayLengthSignificance","getVectorFloat_Aux(D3DDecayLengthSignificance)") \
          .Define("D2ddecayLengthSignificance","getVectorFloat_Aux(D2DDecayLengthSignificance)") \
          .Define("DvtxProb","getVectorFloat_Aux(DVtxProb)") \
          .Define("D3dpointingAngle","getVectorFloat_Aux(D3DPointingAngle)") \
          .Define("D2dpointingAngle","getVectorFloat_Aux(D2DPointingAngle)") \
          .Define("Ddca","getVectorFloat_Aux(DDca)") \
          .Define("DimpactParameter3D","getVectorFloat_Aux(DImpactParameter3D)") \
          .Define("DimpactParameter3Derror","getVectorFloat_Aux(DImpactParameter3Derror)") \
          .Define("Dtrk1Pt","getVectorFloat_Aux(DTtrk1Pt)") \
          .Define("Dtrk2Pt","getVectorFloat_Aux(DTrk2Pt)") \
          .Define("Dtrk1PtErr","getVectorFloat_Aux(DTrk1PtErr)") \
          .Define("Dtrk2PtErr","getVectorFloat_Aux(DTrk2PtErr)") \
          .Define("Dtrk1Eta","getVectorFloat_Aux(DTrk1Eta)") \
          .Define("Dtrk2Eta","getVectorFloat_Aux(DTrk2Eta)") \
          .Define("DxydcaSignificanceDaugther1","getVectorFloat_Aux(DxyDCASignificanceDaugther1)") \
          .Define("DxydcaSignificanceDaugther2","getVectorFloat_Aux(DxyDCASignificanceDaugther2)") \
          .Define("DzdcaSignificanceDaugther1","getVectorFloat_Aux(DzDCASignificanceDaugther1)") \
          .Define("DzdcaSignificanceDaugther2","getVectorFloat_Aux(DzDCASignificanceDaugther2)") \
          .Define("Dtrk1Chi2n","getVectorFloat_Aux(DTrk1Chi2n)") \
          .Define("Dtrk2Chi2n","getVectorFloat_Aux(DTrk2Chi2n)") \
          .Define("Dgen","getVectorFloat_Aux(DGen)") \
          .Define("DgenIndex","getVectorFloat_Aux(DGenIndex)") \
          .Define("DgennDa","getVectorFloat_Aux(DGennDa)") \
          .Define("Dgenpt","getVectorFloat_Aux(DGenpt)") \
          .Define("Dgenphi","getVectorFloat_Aux(DGenphi)") \
          .Define("Dgeny","getVectorFloat_Aux(DGeny)") \

d_train.Snapshot(treeName, outFileNameTrain, {"centrality","PVx","PVy","PVz","Dpt","Drapidity","Dmass","Dphi","D2ddecayLength","Dd0Err","D3ddecayLength","DdxyzErr","D3ddecayLengthSignificance","D2ddecayLengthSignificance","DvtxProb","D3dpointingAngle","D2dpointingAngle","Ddca","DimpactParameter3D","DimpactParameter3Derror","Dtrk1Pt","Dtrk2Pt","Dtrk1PtErr","Dtrk2PtErr","Dtrk1Eta","Dtrk2Eta","DxydcaSignificanceDaugther1","DxydcaSignificanceDaugther2","DzdcaSignificanceDaugther1","DzdcaSignificanceDaugther2","Dtrk1Chi2n","Dtrk2Chi2n","Dgen","DgenIndex","DgennDa","Dgenpt","Dgenphi","Dgeny"})

d_test.Snapshot(treeName, outFileNameTest, {"centrality","PVx","PVy","PVz","Dpt","Drapidity","Dmass","Dphi","D2ddecayLength","Dd0Err","D3ddecayLength","DdxyzErr","D3ddecayLengthSignificance","D2ddecayLengthSignificance","DvtxProb","D3dpointingAngle","D2dpointingAngle","Ddca","DimpactParameter3D","DimpactParameter3Derror","Dtrk1Pt","Dtrk2Pt","Dtrk1PtErr","Dtrk2PtErr","Dtrk1Eta","Dtrk2Eta","DxydcaSignificanceDaugther1","DxydcaSignificanceDaugther2","DzdcaSignificanceDaugther1","DzdcaSignificanceDaugther2","Dtrk1Chi2n","Dtrk2Chi2n","Dgen","DgenIndex","DgennDa","Dgenpt","Dgenphi","Dgeny"})