{

std::ofstream data_txt("./data_D0_daughtersPT_trk1_trk2.txt");

auto f = TFile::Open("/Users/cesarbernardes/Dropbox/Ubuntu_1204/AltasEnergias/ProfessorUFRGS/OrientacaoDeAlunos/IC_TCC/TopicosDeEstudo/D0_Selection/D0_MC_SkimmedTrees/tree_skim_MC_prompt.root");

auto t = (TTree*)f->Get("Dfinder/ntDkpi"); 

std::vector<float> *DTtrk1Pt=0;
std::vector<float> *DTrk2Pt=0;
std::vector<float> *DGen=0;

t->SetBranchAddress("DTtrk1Pt",&DTtrk1Pt);
t->SetBranchAddress("DTrk2Pt",&DTrk2Pt);
t->SetBranchAddress("DGen",&DGen);

Int_t nevents = t->GetEntries();

for (Int_t i=0;i<nevents;i++) {

   t->GetEntry(i);

   for(Int_t j=0;j<(*DGen).size();j++){
      if((*DGen)[j]==23333 || (*DGen)[j]==23344){//select signal only
         float aux_Dtrk1Pt = (*DTtrk1Pt)[j];
         float aux_Dtrk2Pt = (*DTrk2Pt)[j];       
         //std::cout<<"j= "<<j<<" aux_Dtrk1Pt: "<<aux_Dtrk1Pt<<" aux_Dtrk2Pt: "<<aux_Dtrk2Pt<<std::endl;
	 data_txt<<aux_Dtrk1Pt<<" "<<aux_Dtrk2Pt<<endl;
	 break;
      }
   }
      
}


}
