void vn_extractor(){

  TFile * f = TFile::Open("corrfunc1D_cent_0_5.root");

  TH1D* h = (TH1D*) f->Get("signal_dphi_projection_0");

  // Cria a função de fit

  TF1 *fit = new TF1("fit", "[0]*(1 + 2*[1]*cos(x)+2*[2]*cos(2*x) + 2*[3]*cos(3*x))", -1.4, 4.6);

  fit->SetParameters(60, 0.1, 0.1, 0.1);

  // Cria um canvas para desenhar o histograma e o fit

  TCanvas *c = new TCanvas("c", "c", 800, 600);

  c->cd();

  // Desenha o histograma no canvas

  h->Draw("p*Error");

  // Faz o fit no histograma do dphi_signal

  h->Fit(fit, "R");

  // Desenha o fit no canvas

  fit->Draw("same");

  // Salva o canvas como um pdf

  c->SaveAs("FourierFit.pdf");

}
