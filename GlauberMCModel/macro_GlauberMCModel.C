///To run it, please, do: root -l -b -q 'macro_GlauberMCModel.C()'

///Auxiliar functions

//Function to customize Legends
void legendStyle(TLegend* tl, TF1 *f, TString formula){
   tl->AddEntry(f,formula,"");
   tl->SetFillStyle(0);
   tl->SetBorderSize(0);
   tl->SetTextSize(0.045);
   tl->SetTextFont(42);
}

void macro_GlauberMCModel(){

std::cout<<"Starting our dear macro of Monte Carlo Glauber Model for PbPb Collisions..."<<std::endl;

///Important constants used in the method

//Inelastic nucleon-nucleon cross-section for 2.76TeV center-of-mass energy in PbPb collisions in milibarn (mb): 1 barn = 10^{-28} m^2
const float sigma_inel_NN_ = 65.; //in mb

//Parameters for the nuclear density function (Woods-Saxon function)
const float r_0 = 6.62; //Nuclear radius in fm, 1 fm = 10^{-15} m
const float a = 0.542; //Skin depth in fm (characterizes how sharp the edge of the nucleus is)
const float rho_0 = 3.;//Nuclear density at the center in 1/(fm^3)
		    

///Woods-Saxon function
TF1 *func_WS = new TF1("func_WS","[0]*TMath::Power(1+TMath::Exp((x-[1])/[2]),-1)",0,14); //parameters: [0] := rho_0, [1] := r_0 and [2] := a
func_WS->SetParameter(0,rho_0);
func_WS->SetParameter(1,r_0);
func_WS->SetParameter(2,a);
func_WS->GetYaxis()->SetTitle("Nuclear density (1/fm^{3})");
func_WS->GetXaxis()->SetTitle("Distance from center of nucleus, r (fm)");
func_WS->SetLineColor(2);
func_WS->SetLineWidth(2);
func_WS->SetTitle("");
//Plot function
TCanvas tc("tc","tc",1000,500); //ROOT TCanvas to plot the distributions
tc.Divide(2,1,0.02,0.02); //divide it in two
tc.cd(1); //enter in the left Pad of the Canvas
func_WS->Draw("l");
TLegend *leg = new TLegend(0.6,0.68,0.85,0.9); //Create a legend for the plots
legendStyle(leg, func_WS, "#frac{#rho_{0}}{1+exp#left(#frac{r-r_{0}}{a}#right)}"); //call function to customize style of legend
leg->Draw();
tc.cd(2); //enter in the right Pad of the Canvas
tc.Print("fig_nucleus_distributions.pdf"); //save pdf file with plots

}
