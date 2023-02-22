import ROOT

# We prepare an input tree to run on
fileName = "/Users/cesarbernardes/Dropbox/Ubuntu_1204/AltasEnergias/ProfessorUFRGS/OrientacaoDeAlunos/IC_TCC/TopicosDeEstudo/D0_Selection/D0_MC_SkimmedTrees/tree_skim_MC_prompt.root"
treeName = "Dfinder/ntDkpi"
outFileNameTrain = "tree_skim_MC_promptTrain.root"
outFileNameTest = "tree_skim_MC_promptTest.root"

# We read the tree from the file and create a RDataFrame
d = ROOT.RDataFrame(treeName, fileName) 

d_train = d.Range(500000) ##select first 500k entries
d_test = d.Range(500001,0) ##from 500001 till the end of dataset, which has around 1M events

d1 = d_train.Snapshot(treeName, outFileNameTrain, {"centrality","PVx","PVy","PVz","DPt","DRapidity","DMass","DPhi","D2DDecayLength","DD0Err","D3DDecayLength","DDxyzErr","D3DDecayLengthSignificance","D2DDecayLengthSignificance","DVtxProb","D3DPointingAngle","D2DPointingAngle","DDca","DImpactParameter3D","DImpactParameter3Derror","DTtrk1Pt","DTrk2Pt","DTrk1PtErr","DTrk2PtErr","DTrk1Eta","DTrk2Eta","DxyDCASignificanceDaugther1","DxyDCASignificanceDaugther2","DzDCASignificanceDaugther1","DzDCASignificanceDaugther2","DTrk1Chi2n","DTrk2Chi2n","DGen","DGenIndex","DGennDa","DGenpt","DGenphi","DGeny"})

d2 = d_test.Snapshot(treeName, outFileNameTest, {"centrality","PVx","PVy","PVz","DPt","DRapidity","DMass","DPhi","D2DDecayLength","DD0Err","D3DDecayLength","DDxyzErr","D3DDecayLengthSignificance","D2DDecayLengthSignificance","DVtxProb","D3DPointingAngle","D2DPointingAngle","DDca","DImpactParameter3D","DImpactParameter3Derror","DTtrk1Pt","DTrk2Pt","DTrk1PtErr","DTrk2PtErr","DTrk1Eta","DTrk2Eta","DxyDCASignificanceDaugther1","DxyDCASignificanceDaugther2","DzDCASignificanceDaugther1","DzDCASignificanceDaugther2","DTrk1Chi2n","DTrk2Chi2n","DGen","DGenIndex","DGennDa","DGenpt","DGenphi","DGeny"})
