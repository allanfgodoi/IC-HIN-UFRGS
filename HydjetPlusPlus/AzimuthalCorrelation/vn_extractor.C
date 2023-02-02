//#include<cstdio>
//#include<cstdlib>
//#include<iostream>

//#include "TROOT.h"
//#include "TTree.h"
//#include "TFile.h"
//#include "TFile.h"
//#include "TTree.h"
//#include "TVector3.h"
//#include "TH1.h"
//#include "TH2.h"
//#include "TCanvas.h"

//#include <cmath>
//#include "TF1.h"
//#include "TH1D.h"


void FourierFit(const char* filename)
{

  TFile file(filename, "signal_dphi_projection.root");

  TH1D *signal_dphi_projection = (TH1D*) file.Get("signal_dphi_projection.root");


  // Cria a função de fit

  TF1 *fit = new TF1("fit", "[0]*cos(x*[1]+[2])+[3]*cos(2*x*[4]+[5])+[6]", 0, 5);

  //fit->SetParNames("V_{1}", "f_{1}", "phi_{1}", "V_{2}", "f_{2}", "phi_{2}", "bkg");

  fit->SetParameters(1, 1, 1, 0.5, 2, 0.5, 0);


  // Faz o fit no histograma do dphi_signal

  signal_dphi_projection->Fit(fit, "R");


  // Cria um canvas para desenhar o histograma e o fit

  TCanvas *c = new TCanvas("c", "c", 800, 600);

  c->cd();


  // Desenha o histograma e o fit no canvas

  signal_dphi_projection->Draw("hist");

  fit->Draw("same");


  // Salva o canvas como um arquivo de imagem

  c->SaveAs("FourierFit.png");

}


int main(int argc, char *argv[])
{

  FourierFit("signal_dphi_projection.root");

  return 0;

}

