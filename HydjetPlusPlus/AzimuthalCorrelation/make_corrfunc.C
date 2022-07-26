///*** To run it, please, do: <<root -l -b -q make_corrfunc.C>>


void make_corrfunc(){

int nbins=1;
float aptassmin[]={1.0};
float aptassmax[]={3.0};
float apttrigmin[]={1.0};
float apttrigmax[]={3.0};

TFile* hfile = new TFile("corr_cent_0_5.root");

TFile *output = new TFile("corrfunc1D_cent_0_5.root","recreate");

int ptassmin, ptassmax, pttrigmin, pttrigmax;

//std::vector<TH3D*> signal3D;
//std::vector<TH3D*> background3D;
//std::vector<TH2D*> signal;
TH2D *signal[nbins];
//std::vector<TH2D*> background;
//std::vector<TH1D*> dphi_bg;
//std::vector<TH1D*> dphi_signal;
TH1D *dphi_signal[nbins];

for (int i = 0; i < nbins; i++) {
   ptassmin = aptassmin[i]*10;
   ptassmax = aptassmax[i]*10;
   pttrigmin = apttrigmin[i]*10;
   pttrigmax = apttrigmax[i]*10;
   signal[i] = (TH2D*) hfile->Get(Form("signal_trig_%d_%d_ass_%d_%d",pttrigmin,pttrigmax,ptassmin,ptassmax));
}

for (int i =0; i < nbins; i++) {

   dphi_signal[i] = signal[i]->ProjectionY(Form("signal_dphi_projection_%d",i),4,10); //projection from -3.75 till -1.65 in deltaEta. Since was symmetrized, no need to get positive part
 
 
     	
   dphi_signal[i] ->Sumw2();
   //dphi_signal[i]->Divide(dphi_bg[i]);
 
     	
   //c= background1[i] -> GetBinContent(background1[i]->FindBin(0,0));
   //c1=background1[i] ->GetYaxis()->GetBinWidth(1);
   //c2=background1[i] ->GetXaxis()->GetBinWidth(1);
 
   //dphi_signal[i] -> Scale(c/(4.*c1*c2*c3));
   dphi_signal[i]->Write();		
   //dphi_bg[i]->Write();			
			
}


}
