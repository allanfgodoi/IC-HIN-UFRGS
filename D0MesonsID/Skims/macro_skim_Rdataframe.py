import ROOT
##from ROOT import TFile, TTree

# We prepare an input tree to run on
fileName = "/eos/user/c/caber/D0_MC_trainingTrees/MC_prompt.root"
treeName = "Dfinder/ntDkpi"
outFileName = "tree_skim_MC_prompt.root"


# fix problematic variables names in the tree
##f = ROOT.TFile.Open(fileName ,"READ")
##t = f.Get("Dfinder/ntDkpi")
##t.SetAlias("TwoDDecayLength","2DDecayLength.Dd0")


# We read the tree from the file and create a RDataFrame
d_old = ROOT.RDataFrame(treeName, fileName)
d = d_old.Alias("TwoDDecayLength","2DDecayLength.Dd0") \
         .Alias("ThreeDDecayLength","3DDecayLength.Ddxyz") \
         .Alias("ThreeDDecayLengthSignificance","3DDecayLengthSignificance.3DDecayLengthSignificance") \
         .Alias("TwoDDecayLengthSignificance","2DDecayLengthSignificance.2DDecayLengthSignificance") \
         .Alias("ThreeDPointingAngle","3DPointingAngle.Dalpha") \
         .Alias("TwoDPointingAngle","2DPointingAngle.Dalpha2D")  

centrality_cut = "centrality>20 && centrality<60"

getVector_code ='''
std::vector<float> getVector1 (int DCollSize, ROOT::VecOps::RVec<Float_t> vecPt, ROOT::VecOps::RVec<Float_t> vecY)
{
   std::vector<float> v;
   for (int i = 0; i < DCollSize; i++){ 
      if (vecPt[i]>3.5 && vecPt[i]<4.2 && std::abs(vecY[i])<0.8) v.push_back(vecPt[i]);
   }
   return v;
}

std::vector<float> getVector2 (int DCollSize, ROOT::VecOps::RVec<Float_t> vecPt, ROOT::VecOps::RVec<Float_t> vecY)
{
   std::vector<float> v;
   for (int i = 0; i < DCollSize; i++){ 
      if (vecPt[i]>3.5 && vecPt[i]<4.2 && std::abs(vecY[i])<0.8) v.push_back(vecY[i]);
   }
   return v;
}

std::vector<float> getVector3 (int DCollSize, ROOT::VecOps::RVec<Float_t> vecPt, ROOT::VecOps::RVec<Float_t> vecY, ROOT::VecOps::RVec<Float_t> vecAux)
{
   std::vector<float> v;
   for (int i = 0; i < DCollSize; i++){ 
      if (vecPt[i]>3.5 && vecPt[i]<4.2 && std::abs(vecY[i])<0.8) v.push_back(vecAux[i]);
   }
   return v;
}
'''

ROOT.gInterpreter.Declare(getVector_code)

d_cut = d.Filter(centrality_cut,"Centrality cut") \
         .Define("DPt","getVector1(Dsize, pT.Dpt, y.Dy)") \
         .Define("DRapidity","getVector2(Dsize, pT.Dpt, y.Dy)") \
         .Define("DMass","getVector3(Dsize, pT.Dpt, y.Dy, mass.mass)") \
         .Define("DPhi","getVector3(Dsize, pT.Dpt, y.Dy, phi.Dphi)") \
         .Define("D2DDecayLength","getVector3(Dsize, pT.Dpt, y.Dy, TwoDDecayLength)") \
         .Define("DD0Err","getVector3(Dsize, pT.Dpt, y.Dy, Dd0Err.Dd0Err)") \
         .Define("D3DDecayLength","getVector3(Dsize, pT.Dpt, y.Dy, ThreeDDecayLength)") \
         .Define("DDxyzErr","getVector3(Dsize, pT.Dpt, y.Dy, DdxyzErr.DdxyzErr)") \
         .Define("D3DDecayLengthSignificance","getVector3(Dsize, pT.Dpt, y.Dy, ThreeDDecayLengthSignificance)") \
         .Define("D2DDecayLengthSignificance","getVector3(Dsize, pT.Dpt, y.Dy, TwoDDecayLengthSignificance)") \
         .Define("DVtxProb","getVector3(Dsize, pT.Dpt, y.Dy, VtxProb.Dchi2cl)") \
         .Define("D3DPointingAngle","getVector3(Dsize, pT.Dpt, y.Dy, ThreeDPointingAngle)") \
         .Define("D2DPointingAngle","getVector3(Dsize, pT.Dpt, y.Dy, TwoDPointingAngle)") \
         .Define("DDca","getVector3(Dsize, pT.Dpt, y.Dy, dca.Ddca)") \
         .Define("DImpactParameter3D","getVector3(Dsize, pT.Dpt, y.Dy, ImpactParameter3D.Dip3d)") \
         .Define("DImpactParameter3Derror","getVector3(Dsize, pT.Dpt, y.Dy, ImpactParameter3Derror.Dip3derr)") \
         .Define("DTtrk1Pt","getVector3(Dsize, pT.Dpt, y.Dy, Dtrk1Pt.Dtrk1Pt)") \
         .Define("DTrk2Pt","getVector3(Dsize, pT.Dpt, y.Dy, Dtrk2Pt.Dtrk2Pt)") \
         .Define("DTrk1PtErr","getVector3(Dsize, pT.Dpt, y.Dy, Dtrk1PtErr.Dtrk1PtErr)") \
         .Define("DTrk2PtErr","getVector3(Dsize, pT.Dpt, y.Dy, Dtrk2PtErr.Dtrk2PtErr)") \
         .Define("DTrk1Eta","getVector3(Dsize, pT.Dpt, y.Dy, Dtrk1Eta.Dtrk1Eta)") \
         .Define("DTrk2Eta","getVector3(Dsize, pT.Dpt, y.Dy, Dtrk2Eta.Dtrk2Eta)") \
         .Define("DxyDCASignificanceDaugther1","getVector3(Dsize, pT.Dpt, y.Dy, xyDCASignificanceDaugther1.xyDCASignificanceDaugther1)") \
         .Define("DxyDCASignificanceDaugther2","getVector3(Dsize, pT.Dpt, y.Dy, xyDCASignificanceDaugther2.xyDCASignificanceDaugther2)") \
         .Define("DzDCASignificanceDaugther1","getVector3(Dsize, pT.Dpt, y.Dy, zDCASignificanceDaugther1.zDCASignificanceDaugther1)") \
         .Define("DzDCASignificanceDaugther2","getVector3(Dsize, pT.Dpt, y.Dy, zDCASignificanceDaugther2.zDCASignificanceDaugther2)") \
         .Define("DTrk1Chi2n","getVector3(Dsize, pT.Dpt, y.Dy, Dtrk1Chi2n.Dtrk1Chi2n)") \
         .Define("DTrk2Chi2n","getVector3(Dsize, pT.Dpt, y.Dy, Dtrk2Chi2n.Dtrk2Chi2n)") \
         .Define("DGen","getVector3(Dsize, pT.Dpt, y.Dy, Dgen.Dgen)") \
         .Define("DGenIndex","getVector3(Dsize, pT.Dpt, y.Dy, DgenIndex.DgenIndex)") \
         .Define("DGennDa","getVector3(Dsize, pT.Dpt, y.Dy, DgennDa.DgennDa)") \
         .Define("DGenpt","getVector3(Dsize, pT.Dpt, y.Dy, Dgenpt.Dgenpt)") \
         .Define("DGenphi","getVector3(Dsize, pT.Dpt, y.Dy, Dgenphi.Dgenphi)") \
         .Define("DGeny","getVector3(Dsize, pT.Dpt, y.Dy, Dgeny.Dgeny)")
         ##.Range(10) ###only for cross-checks


d2 = d_cut.Snapshot(treeName, outFileName, {"centrality","PVx","PVy","PVz","DPt","DRapidity","DMass","DPhi","D2DDecayLength","DD0Err","D3DDecayLength","DDxyzErr","D3DDecayLengthSignificance","D2DDecayLengthSignificance","DVtxProb","D3DPointingAngle","D2DPointingAngle","DDca","DImpactParameter3D","DImpactParameter3Derror","DTtrk1Pt","DTrk2Pt","DTrk1PtErr","DTrk2PtErr","DTrk1Eta","DTrk2Eta","DxyDCASignificanceDaugther1","DxyDCASignificanceDaugther2","DzDCASignificanceDaugther1","DzDCASignificanceDaugther2","DTrk1Chi2n","DTrk2Chi2n","DGen","DGenIndex","DGennDa","DGenpt","DGenphi","DGeny"})


##rep = d2.Report()
##rep.Print()
