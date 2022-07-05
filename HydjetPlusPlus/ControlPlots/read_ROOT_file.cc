{

  TFile f("/home/pedrolunardi/IC/Hydjet/RunOutput.root");
  
  TTree *input_tree = (TTree*)f.Get("td");  
  Int_t nev; // n events.
  Float_t Bgen; // impact parameter.
  Float_t Sigin; // total inelastic NN cross section.
  Float_t Sigjet; // hard scattering NN cross section.
  Int_t Ntot; // total event multiplicity
  Int_t Nhyd; // multiplicity of hydro-induced particles
  Int_t Npyt; // multiplicity of jet-induced particles
  Int_t Njet; // number of hard parton-parton scatterings
  Int_t Nbcol; // mean number of NN sub-collisions
  Int_t Npart; // mean number of nucleon-participants
  Float_t Px[1000000]; // x-component of the momentum, in GeV/c
  Float_t Py[1000000]; // y-component of the momentum, in GeV/c
  Float_t Pz[1000000]; // z-component of the momentum, in GeV/c
  Float_t E[1000000]; // energy, in GeV
  Float_t X[1000000]; // x-coordinate at emission point, in fm
  Float_t Y[1000000]; // y-coordinate at emission point, in fm
  Float_t Z[1000000]; // z-coordinate at emission point, in fm
  Float_t T[1000000]; // proper time of particle emission at emission point, in fm/c
  Int_t pdg[1000000]; // Geant particle code
  Int_t Mpdg[1000000]; // Geant code of mother particle, -1 for primordinary particles
  Int_t type[1000000]; // origin of particle (=0 - from soft hydro part, >0 - from hard jet part)
  Int_t Index[1000000]; // unique zero based index of the particle
  Int_t MotherIndex[1000000]; // index of the mother particle (-1 if its a primary particle)
  Int_t NDaughters[1000000]; // number of daughter particles
  Int_t FirstDaughterIndex[1000000]; // index of the first daughter (-1 if it does not exist)
  Int_t LastDaughterIndex[1000000]; // index of the last daughter
  Int_t pythiaStatus[1000000]; // PYTHIA status code (-1 if the particle is from soft component)
  Int_t final[1000000]; // an integer branch keeping 1 for final state particles and 0 for decayed particles

  input_tree->SetBranchAddress("nev",&nev);
  input_tree->SetBranchAddress("Bgen",&Bgen);
  input_tree->SetBranchAddress("Sigin",&Sigin);
  input_tree->SetBranchAddress("Sigjet",&Sigjet);
  input_tree->SetBranchAddress("Ntot",&Ntot);
  input_tree->SetBranchAddress("Nhyd",&Nhyd);
  input_tree->SetBranchAddress("Npyt",&Npyt);
  input_tree->SetBranchAddress("Njet",&Njet);
  input_tree->SetBranchAddress("Nbcol",&Nbcol);
  input_tree->SetBranchAddress("Npart",&Npart);
  input_tree->SetBranchAddress("Px",&Px);
  input_tree->SetBranchAddress("Py",&Py);
  input_tree->SetBranchAddress("Pz",&Pz);
  input_tree->SetBranchAddress("E",&E);
  input_tree->SetBranchAddress("X",&X);
  input_tree->SetBranchAddress("Y",&Y);
  input_tree->SetBranchAddress("Z",&Z);
  input_tree->SetBranchAddress("T",&T);
  input_tree->SetBranchAddress("pdg",&pdg);
  input_tree->SetBranchAddress("Mpdg",&Mpdg);
  input_tree->SetBranchAddress("type",&type);
  input_tree->SetBranchAddress("Index",&Index);
  input_tree->SetBranchAddress("MotherIndex",&MotherIndex);
  input_tree->SetBranchAddress("NDaughters",&NDaughters);
  input_tree->SetBranchAddress("FirstDaughterIndex",&FirstDaughterIndex);
  input_tree->SetBranchAddress("LastDaughterIndex",&LastDaughterIndex);
  input_tree->SetBranchAddress("pythiaStatus",&pythiaStatus);
  input_tree->SetBranchAddress("final",&final);

  TH1I *h_nev = new TH1I("h_nev","nev distribution",1000,-0.5,1000);
  TH1F *h_Bgen = new TH1F("h_Bgen","Bgen distribution",100,-5,5);
  TH1F *h_Sigin = new TH1F("h_Sigin","Sigin distribution",100,-5,100);
  TH1F *h_Sigjet = new TH1F("h_Sigjet","Sigjet distribution",100,-5,5);
  TH1I *h_Ntot = new TH1I("h_Ntot","Ntot distribution",100,0,100000);
  TH1I *h_Nhyd = new TH1I("h_Nhyd","Nhyd distribution",100,0,100000);
  TH1I *h_Npyt = new TH1I("h_Npyt","Npyt distribution",100,0,100000);
  TH1I *h_Njet = new TH1I("h_Njet","Njet distribution",100,0,100);
  TH1I *h_Nbcol = new TH1I("h_Nbcol","Nbcol distribution",100,1500,2000);
  TH1I *h_Npart = new TH1I("h_Npart","Npart distribution",100,0,500);
  TH1F *h_Px = new TH1F("h_Px","Px distribution",100,-5,5);
  TH1F *h_Py = new TH1F("h_Py","Py distribution",100,-5,5);
  TH1F *h_Pz = new TH1F("h_Pz","Pz distribution",100,-20,20);
  TH1F *h_E = new TH1F("h_E","Energy distribution",100,-0.5,500);
  TH1F *h_X = new TH1F("h_X","X coordinate, in fm",100,-20,20);
  TH1F *h_Y = new TH1F("h_Y","Y coordinate, in fm",100,-20,20);
  TH1F *h_Z = new TH1F("h_Z","Z coordinate, in fm",100,-20,20);
  TH1F *h_T = new TH1F("h_T","Time of particle emission at emission point, in fm/c",100,0,1000);
  TH1I *h_pdg = new TH1I("h_pdg","Geant particle code",100,-50000,50000);
  TH1I *h_Mpdg = new TH1I("h_Mpdg","Geant code of mother particle",100,-50000,50000);
  TH1I *h_type = new TH1I("h_type","Origin of particle (type)",4,-0.5,1.5);
  TH1I *h_Index = new TH1I("h_Index","Unique zero based index of the particle",100,-100,100000);
  TH1I *h_MotherIndex = new TH1I("h_MotherIndex","Index of the mother particle",100,-5,50000);
  TH1I *h_NDaughters = new TH1I("h_NDaughters","Number of daughter particles",10,0,10);
  TH1I *h_FirstDaughterIndex = new TH1I("h_FirstDaughterIndex","Index of the first daughter",100,-5,50000);
  TH1I *h_LastDaughterIndex = new TH1I("h_LastDaughterIndex","Index of the last daughter",100,-5,50000);
  TH1I *h_pythiaStatus = new TH1I("h_pythiaStatus","PYTHIA status code",4,-2,2);
  TH1I *h_final = new TH1I("h_final","Final state",3,-1,2);

  // my variables (histograms)

  TH1F *h_Pt = new TH1F("h_Pt","Pt distribution",200,0,20);
  TH1D *h_Zcm = new TH1D("h_Zcm","Z coordinate, in cm",200,-100,100);

  TH1F *h_Pt_Soft = new TH1F("h_Pt_Soft","Pt_Soft distribution",200,0,20);
  TH1F *h_Pt_Hard = new TH1F("h_Pt_Hard","Pt_Hard distribution",200,0,20);
  TH1D *h_Zcm_Soft = new TH1D("h_Zcm_Soft","Soft Z coordinate, in cm",200,-100,100);
  TH1D *h_Zcm_Hard = new TH1D("h_Zcm_Hard","Hard Z coordinate, in cm",200,-100,100);

  TH1F *h_Px_Soft = new TH1F("h_Px_Soft","Px_Soft distribution",100,-5,5);
  TH1F *h_Px_Hard = new TH1F("h_Px_Hard","Px_Hard distribution",100,-5,5);
  TH1F *h_Py_Soft = new TH1F("h_Py_Soft","Py_Soft distribution",100,-5,5);
  TH1F *h_Py_Hard = new TH1F("h_Py_Hard","Py_Hard distribution",100,-5,5);
  TH1F *h_Pz_Soft = new TH1F("h_Pz_Soft","Pz_Soft distribution",100,-20,20);
  TH1F *h_Pz_Hard = new TH1F("h_Pz_Hard","Pz_Hard distribution",100,-20,20);
  TH1F *h_E_Soft = new TH1F("h_E_Soft","Soft Energy distribution",100,-0.5,500);
  TH1F *h_E_Hard = new TH1F("h_E_Hard","Hard Energy distribution",100,-0.5,500);
  TH1F *h_X_Soft = new TH1F("h_X_Soft","Soft X coordinate, in fm",100,-20,20);
  TH1F *h_X_Hard = new TH1F("h_X_Hard","Hard X coordinate, in fm",100,-20,20);
  TH1F *h_Y_Soft = new TH1F("h_Y_Soft","Soft Y coordinate, in fm",100,-20,20);
  TH1F *h_Y_Hard = new TH1F("h_Y_Hard","Hard Y coordinate, in fm",100,-20,20);
  TH1F *h_Z_Soft = new TH1F("h_Z_Soft","Soft Z coordinate, in fm",100,-20,20);
  TH1F *h_Z_Hard = new TH1F("h_Z_Hard","Hard Z coordinate, in fm",100,-20,20);
  TH1F *h_T_Soft = new TH1F("h_T_Soft","Time of soft particle emission at emission point, in fm/c",100,0,1000);
  TH1F *h_T_Hard = new TH1F("h_T_Hard","Time of hard particle emission at emission point, in fm/c",100,0,1000);
  TH1I *h_pdg_Soft = new TH1I("h_pdg_Soft","Geant soft particle code",100,-50000,50000);
  TH1I *h_pdg_Hard = new TH1I("h_pdg_Hard","Geant hard particle code",100,-50000,50000);
  TH1I *h_Mpdg_Soft = new TH1I("h_Mpdg_Soft","Geant code of soft mother particle",100,-50000,50000);
  TH1I *h_Mpdg_Hard = new TH1I("h_Mpdg_Hard","Geant code of hard mother particle",100,-50000,50000);
  TH1I *h_type_Soft = new TH1I("h_type_Soft","Origin of soft particle (type)",4,-0.5,1.5);
  TH1I *h_type_Hard = new TH1I("h_type_Hard","Origin of hard particle (type)",4,-0.5,1.5);
  TH1I *h_Index_Soft = new TH1I("h_Index_Soft","Unique zero based index of the soft particle",100,-100,100000);
  TH1I *h_Index_Hard = new TH1I("h_Index_Hard","Unique zero based index of the hard particle",100,-100,100000);
  TH1I *h_MotherIndex_Soft = new TH1I("h_MotherIndex_Soft","Index of the soft mother particle",100,-5,50000);
  TH1I *h_MotherIndex_Hard = new TH1I("h_MotherIndex_Hard","Index of the hard mother particle",100,-5,50000);
  TH1I *h_NDaughters_Soft = new TH1I("h_NDaughters_Soft","Number of soft daughter particles",10,0,10);
  TH1I *h_NDaughters_Hard = new TH1I("h_NDaughters_Hard","Number of hard daughter particles",10,0,10);
  TH1I *h_FirstDaughterIndex_Soft = new TH1I("h_FirstDaughterIndex_Soft","Index of the first soft daughter",100,-5,50000);
  TH1I *h_FirstDaughterIndex_Hard = new TH1I("h_FirstDaughterIndex_Hard","Index of the first hard daughter",100,-5,50000);
  TH1I *h_LastDaughterIndex_Soft = new TH1I("h_LastDaughterIndex_Soft","Index of the last soft daughter",100,-5,50000);
  TH1I *h_LastDaughterIndex_Hard = new TH1I("h_LastDaughterIndex_Hard","Index of the last hard daughter",100,-5,50000);
  TH1I *h_pythiaStatus_Soft = new TH1I("h_pythiaStatus_Soft","Soft PYTHIA status code",4,-2,2);
  TH1I *h_pythiaStatus_Hard = new TH1I("h_pythiaStatus_Hard","Hard PYTHIA status code",4,-2,2);
  TH1I *h_final_Soft = new TH1I("h_final_Soft","Final state (soft)",3,-1,2);
  TH1I *h_final_Hard = new TH1I("h_final_Hard","Final state (hard)",3,-1,2);

  Int_t nevents = input_tree->GetEntries();

  Double_t Pow = TMath::Power (10,-13);

  for (Int_t i=0;i<nevents;i++) {
	input_tree->GetEntry(i);
	
	h_nev->Fill(nev);
	h_Bgen->Fill(Bgen);
	h_Sigin->Fill(Sigin);
	h_Sigjet->Fill(Sigjet);
	h_Ntot->Fill(Ntot);
	h_Nhyd->Fill(Nhyd);
	h_Npyt->Fill(Npyt);
	h_Njet->Fill(Njet);
	h_Nbcol->Fill(Nbcol);
	h_Npart->Fill(Npart);

	for (Int_t j=0;j<Ntot;j++){
       		h_Px->Fill(Px[j]);
		h_Py->Fill(Py[j]);
		h_Pz->Fill(Pz[j]);
		h_E->Fill(E[j]);
		h_X->Fill(X[j]);
		h_Y->Fill(Y[j]);
		h_Z->Fill(Z[j]);
		h_T->Fill(T[j]);
		h_pdg->Fill(pdg[j]);
		h_Mpdg->Fill(Mpdg[j]);
		h_type->Fill(type[j]);
		h_Index->Fill(Index[j]);
		h_MotherIndex->Fill(MotherIndex[j]);
		h_NDaughters->Fill(NDaughters[j]);
		h_FirstDaughterIndex->Fill(FirstDaughterIndex[j]);
		h_LastDaughterIndex->Fill(LastDaughterIndex[j]);
		h_pythiaStatus->Fill(pythiaStatus[j]);
 		h_final->Fill(final[j]);

		// my variables
		
		Float_t Pt = TMath::Hypot (Px[j],Py[j]);
		Double_t Zcm = (Z[j] * Pow);

		h_Pt->Fill(Pt);
		h_Zcm->Fill(Zcm);
		//std::cout<<"Zcm:"<<Zcm<<std::endl;

		if (type[j] == 0){
			h_Px_Soft->Fill(Px[j]);
			h_Py_Soft->Fill(Py[j]);
			h_Pz_Soft->Fill(Pz[j]);
			h_E_Soft->Fill(E[j]);
			h_X_Soft->Fill(X[j]);
			h_Y_Soft->Fill(Y[j]);
			h_Z_Soft->Fill(Z[j]);
			h_T_Soft->Fill(T[j]);
			h_pdg_Soft->Fill(pdg[j]);
			h_Mpdg_Soft->Fill(Mpdg[j]);
			h_type_Soft->Fill(type[j]);
			h_Index_Soft->Fill(Index[j]);
			h_MotherIndex_Soft->Fill(MotherIndex[j]);
			h_NDaughters_Soft->Fill(NDaughters[j]);
			h_FirstDaughterIndex_Soft->Fill(FirstDaughterIndex[j]);
			h_LastDaughterIndex_Soft->Fill(LastDaughterIndex[j]);
			h_pythiaStatus_Soft->Fill(pythiaStatus[j]);
			h_final_Soft->Fill(final[j]);

			h_Pt_Soft->Fill(Pt);
			h_Zcm_Soft->Fill(Zcm);

		}else if (type[j] > 0){
			h_Px_Hard->Fill(Px[j]);
			h_Py_Hard->Fill(Py[j]);
			h_Pz_Hard->Fill(Pz[j]);
                        h_E_Hard->Fill(E[j]);
                        h_X_Hard->Fill(X[j]);
                        h_Y_Hard->Fill(Y[j]);
                        h_Z_Hard->Fill(Z[j]);
                        h_T_Hard->Fill(T[j]);
                        h_pdg_Hard->Fill(pdg[j]);
                        h_Mpdg_Hard->Fill(Mpdg[j]);
                        h_type_Hard->Fill(type[j]);
                        h_Index_Hard->Fill(Index[j]);
                        h_MotherIndex_Hard->Fill(MotherIndex[j]);
                        h_NDaughters_Hard->Fill(NDaughters[j]);
                        h_FirstDaughterIndex_Hard->Fill(FirstDaughterIndex[j]);
                        h_LastDaughterIndex_Hard->Fill(LastDaughterIndex[j]);
                        h_pythiaStatus_Hard->Fill(pythiaStatus[j]);
                        h_final_Hard->Fill(final[j]);

			h_Pt_Hard->Fill(Pt);
			h_Zcm_Hard->Fill(Zcm);

		}else{}
	}
  }

  TCanvas *c_nev = new TCanvas("c_nev", "Canvas of Event Number distribution", 800, 400);
  c_nev->cd(0);
  h_nev->Draw();
  h_nev->GetXaxis()->SetTitle("Number of events");
  h_nev->GetYaxis()->SetTitle("Number of entries per Bin");
  h_nev->SetLineColor(1);
  c_nev->SaveAs("Figures/3in1/h_nev.pdf");

  TCanvas *c_Bgen = new TCanvas("c_Bgen", "Canvas of Bgen distribution", 800, 400);
  c_Bgen->cd(0);
  h_Bgen->Draw();
  h_Bgen->GetXaxis()->SetTitle("Generated Impact Parameter");
  h_Bgen->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Bgen->SetLineColor(2);
  c_Bgen->SaveAs("Figures/3in1/h_Bgen.pdf");

  TCanvas *c_Sigin = new TCanvas("c_Sigin", "Canvas of Sigin distribution", 800, 400);
  c_Sigin->cd(0);
  h_Sigin->Draw();
  h_Sigin->GetXaxis()->SetTitle("Total inelastic NN cross section [mb] ");
  h_Sigin->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Sigin->SetLineColor(2);
  c_Sigin->SaveAs("Figures/3in1/h_Sigin.pdf");

  TCanvas *c_Sigjet = new TCanvas("c_Sigjet", "Canvas of Sigjet distribution", 800, 400);
  c_Sigjet->cd(0);
  h_Sigjet->Draw();
  h_Sigjet->GetXaxis()->SetTitle("Hard scattering NN cross section [mb]");
  h_Sigjet->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Sigjet->SetLineColor(2);
  c_Sigjet->SaveAs("Figures/3in1/h_Sigjet.pdf");

  TCanvas *c_Ntot = new TCanvas("c_Ntot", "Canvas of Ntot distribution", 800, 400);
  c_Ntot->cd(0);
  h_Ntot->Draw();
  h_Ntot->GetXaxis()->SetTitle("Total event multiplicity");
  h_Ntot->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Ntot->SetLineColor(2);
  c_Ntot->SaveAs("Figures/3in1/h_Ntot.pdf");

  TCanvas *c_Nhyd = new TCanvas("c_Nhyd", "Canvas of Nhyd distribution", 800, 400);
  c_Nhyd->cd(0);
  h_Nhyd->Draw();
  h_Nhyd->GetXaxis()->SetTitle("Multiplicity of hydro-induced particles");
  h_Nhyd->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Nhyd->SetLineColor(2);
  c_Nhyd->SaveAs("Figures/3in1/h_Nhyd.pdf");

  TCanvas *c_Npyt = new TCanvas("c_Npyt", "Canvas of Npyt distribution", 800, 400);
  c_Npyt->cd(0);
  h_Npyt->Draw();
  h_Npyt->GetXaxis()->SetTitle("Multiplicity of jet-induced particles");
  h_Npyt->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Npyt->SetLineColor(2);
  c_Npyt->SaveAs("Figures/3in1/h_Npyt.pdf");

  TCanvas *c_Njet = new TCanvas("c_Njet", "Canvas of Njet distribution", 800, 400);
  c_Njet->cd(0);
  h_Njet->Draw();
  h_Njet->GetXaxis()->SetTitle("Number of hard parton-parton scatterings");
  h_Njet->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Njet->SetLineColor(2);
  c_Njet->SaveAs("Figures/3in1/h_Njet.pdf");

  TCanvas *c_Nbcol = new TCanvas("c_Nbcol", "Canvas of Nbcol distribution", 800, 400);
  c_Nbcol->cd(0);
  h_Nbcol->Draw();
  h_Nbcol->GetXaxis()->SetTitle("Mean number of NN sub-collisions");
  h_Nbcol->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Nbcol->SetLineColor(2);
  c_Nbcol->SaveAs("Figures/3in1/h_Nbcol.pdf");

  TCanvas *c_Npart = new TCanvas("c_Npart", "Canvas of Npart distribution", 800, 400);
  c_Npart->cd(0);
  h_Npart->Draw();
  h_Npart->GetXaxis()->SetTitle("Mean number of nucleon-participants");
  h_Npart->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Npart->SetLineColor(2);
  c_Npart->SaveAs("Figures/3in1/h_Npart.pdf");

  TCanvas *c_Px = new TCanvas("c_Px", "Canvas of Px distribution", 800, 400);
  c_Px->cd(0);
  h_Px->Draw();
  h_Px->GetXaxis()->SetTitle("Px [GeV/c]");
  h_Px->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Px->SetLineColor(2);
  c_Px->SaveAs("Figures/h_Px.pdf");

  TCanvas *c_Py = new TCanvas("c_Py", "Canvas of Py distribution", 800, 400);
  c_Py->cd(0);
  h_Py->Draw();
  h_Py->GetXaxis()->SetTitle("Py [GeV/c]");
  h_Py->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Py->SetLineColor(2);
  c_Py->SaveAs("Figures/h_Py.pdf");

  TCanvas *c_Pz = new TCanvas("c_Pz", "Canvas of Pz distribution", 800, 400);
  c_Pz->cd(0);
  h_Pz->Draw();
  h_Pz->GetXaxis()->SetTitle("Pz [GeV/c]");
  h_Pz->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Pz->SetLineColor(2);
  c_Pz->SaveAs("Figures/h_Pz.pdf");

  TCanvas *c_E = new TCanvas("c_E", "Canvas of Energy distribution", 800, 400);
  c_E->cd(0);
  h_E->Draw();
  h_E->GetXaxis()->SetTitle("E [GeV/c]");
  h_E->GetYaxis()->SetTitle("Number of entries per Bin");
  h_E->SetLineColor(2);
  c_E->SaveAs("Figures/h_E.pdf");

  TCanvas *c_X = new TCanvas("c_X", "Canvas of X distribution", 800, 400);
  c_X->cd(0);
  h_X->Draw();
  h_X->GetXaxis()->SetTitle("X-coordinate at emission point, in fm");
  h_X->GetYaxis()->SetTitle("Number of entries per Bin");
  h_X->SetLineColor(2);
  c_X->SaveAs("Figures/h_X.pdf");

  TCanvas *c_Y = new TCanvas("c_Y", "Canvas of Y distribution", 800, 400);
  c_Y->cd(0);
  h_Y->Draw();
  h_Y->GetXaxis()->SetTitle("Y-coordinate at emission point, in fm");
  h_Y->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Y->SetLineColor(2);
  c_Y->SaveAs("Figures/h_Y.pdf");

  TCanvas *c_Z = new TCanvas("c_Z", "Canvas of Z distribution", 800, 400);
  c_Z->cd(0);
  h_Z->Draw();
  h_Z->GetXaxis()->SetTitle("Z-coordinate at emission point, in fm");
  h_Z->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Z->SetLineColor(2);
  c_Z->SaveAs("Figures/h_Z.pdf");

  TCanvas *c_T = new TCanvas("c_T", "Canvas of T distribution", 800, 400);
  c_T->cd(0);
  h_T->Draw();
  h_T->GetXaxis()->SetTitle("Proper time (T) of particle emission at emission point, in fm/c");
  h_T->GetYaxis()->SetTitle("Number of entries per Bin");
  h_T->SetLineColor(2);
  c_T->SaveAs("Figures/h_T.pdf");

  TCanvas *c_pdg = new TCanvas("c_pdg", "Canvas of pdg distribution", 800, 400);
  c_pdg->cd(0);
  h_pdg->Draw(); 
  h_pdg->GetXaxis()->SetTitle("Geant particle code");
  h_pdg->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pdg->SetLineColor(2);
  c_pdg->SaveAs("Figures/h_pdg.pdf");

  TCanvas *c_Mpdg = new TCanvas("c_Mpdg", "Canvas of Mpdg distribution", 800, 400);
  c_Mpdg->cd(0);
  h_Mpdg->Draw();
  h_Mpdg->GetXaxis()->SetTitle("Geant code of mother particle, -1 for primordinary particles");
  h_Mpdg->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Mpdg->SetLineColor(2);
  c_Mpdg->SaveAs("Figures/h_Mpdg.pdf");

  TCanvas *c_type = new TCanvas("c_type", "Canvas of type distribution", 800, 400);
  c_type->cd(0);
  h_type->Draw();
  h_type->GetXaxis()->SetTitle("Origin of particle (=0 -> from soft hydro part, >0 -> from hard jet part)");
  h_type->GetYaxis()->SetTitle("Number of entries per Bin");
  h_type->SetLineColor(2);
  c_type->SaveAs("Figures/h_type.pdf");

  TCanvas *c_Index = new TCanvas("c_Index", "Canvas of Index distribution", 800, 400);
  c_Index->cd(0);
  h_Index->Draw();
  h_Index->GetXaxis()->SetTitle("Unique zero based index of the particle");
  h_Index->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Index->SetLineColor(2);
  c_Index->SaveAs("Figures/h_Index.pdf");

  TCanvas *c_MotherIndex = new TCanvas("c_MotherIndex", "Canvas of MotherIndex distribution", 800, 400);
  c_MotherIndex->cd(0);
  h_MotherIndex->Draw();
  h_MotherIndex->GetXaxis()->SetTitle("Index of the mother particle (-1 if its a primary particle)");
  h_MotherIndex->GetYaxis()->SetTitle("Number of entries per Bin");
  h_MotherIndex->SetLineColor(2);
  c_MotherIndex->SaveAs("Figures/h_MotherIndex.pdf");

  TCanvas *c_NDaughters = new TCanvas("c_NDaughters", "Canvas of NDaughters distribution", 800, 400);
  c_NDaughters->cd(0);
  h_NDaughters->Draw();
  h_NDaughters->GetXaxis()->SetTitle("Number of daughter particles");
  h_NDaughters->GetYaxis()->SetTitle("Number of entries per Bin");
  h_NDaughters->SetLineColor(2);
  c_NDaughters->SaveAs("Figures/h_NDaughters.pdf");

  TCanvas *c_FirstDaughterIndex = new TCanvas("c_FirstDaughterIndex", "Canvas of FirstDaughterIndex distribution", 800, 400);
  c_FirstDaughterIndex->cd(0);
  h_FirstDaughterIndex->Draw();
  h_FirstDaughterIndex->GetXaxis()->SetTitle("Index of the first daughter (-1 if it does not exist)");
  h_FirstDaughterIndex->GetYaxis()->SetTitle("Number of entries per Bin");
  h_FirstDaughterIndex->SetLineColor(2);
  c_FirstDaughterIndex->SaveAs("Figures/h_FirstDaughterIndex.pdf");

  TCanvas *c_LastDaughterIndex = new TCanvas("c_LastDaughterIndex", "Canvas of LastDaughterIndex distribution", 800, 400);
  c_LastDaughterIndex->cd(0);
  h_LastDaughterIndex->Draw();
  h_LastDaughterIndex->GetXaxis()->SetTitle("Index of the last daughter");
  h_LastDaughterIndex->GetYaxis()->SetTitle("Number of entries per Bin");
  h_LastDaughterIndex->SetLineColor(2);
  c_LastDaughterIndex->SaveAs("Figures/h_LastDaughterIndex.pdf");

  TCanvas *c_pythiaStatus = new TCanvas("c_pythiaStatus", "Canvas of pythiaStatus distribution", 800, 400);
  c_pythiaStatus->cd(0);
  h_pythiaStatus->Draw();
  h_pythiaStatus->GetXaxis()->SetTitle("PYTHIA status code (-1 if the particle is from soft component)");
  h_pythiaStatus->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pythiaStatus->SetLineColor(2);
  c_pythiaStatus->SaveAs("Figures/h_pythiaStatus.pdf");

  TCanvas *c_final = new TCanvas("c_final", "Canvas of final distribution", 800, 400);
  c_final->cd(0);
  h_final->Draw();
  h_final->GetXaxis()->SetTitle("Integer branch keeping 0 for decayed particles and 1 for final state particles");
  h_final->GetYaxis()->SetTitle("Number of entries per Bin");
  h_final->SetLineColor(2);
  c_final->SaveAs("Figures/h_final.pdf");

  //my histogram plots

  TCanvas *c_Pt = new TCanvas("c_Pt", "Canvas of Pt distribution", 800, 400);
  c_Pt->cd(0);
  h_Pt->Draw();
  h_Pt->GetXaxis()->SetTitle("Pt [GeV/c]");
  h_Pt->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Pt->SetLineColor(2);
  c_Pt->SaveAs("Figures/h_Pt.pdf");

  TCanvas *c_Zcm = new TCanvas("c_Zcm", "Canvas of Z distribution", 800, 400);
  c_Zcm->cd(0);
  h_Zcm->Draw();
  h_Zcm->GetXaxis()->SetTitle("Z-coordinate at emission point, in cm");
  h_Zcm->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Zcm->SetLineColor(2);
  c_Zcm->SaveAs("Figures/h_Zcm.pdf");

  // Soft

  TCanvas *c_Px_Soft = new TCanvas("c_Px_Soft", "Canvas of Px_Soft distribution", 800, 400);
  c_Px_Soft->cd(0);
  h_Px_Soft->Draw();
  h_Px_Soft->GetXaxis()->SetTitle("Px_Soft [GeV/c]");
  h_Px_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Px_Soft->SetLineColor(2);
  c_Px_Soft->SaveAs("Figures/h_Px_Soft.pdf");

  TCanvas *c_Py_Soft = new TCanvas("c_Py_Soft", "Canvas of Py_Soft distribution", 800, 400);
  c_Py_Soft->cd(0);
  h_Py_Soft->Draw();
  h_Py_Soft->GetXaxis()->SetTitle("Py_Soft [GeV/c]");
  h_Py_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Py_Soft->SetLineColor(2);
  c_Py_Soft->SaveAs("Figures/h_Py_Soft.pdf");

  TCanvas *c_Pz_Soft = new TCanvas("c_Pz_Soft", "Canvas of Pz_Soft distribution", 800, 400);
  c_Pz_Soft->cd(0);
  h_Pz_Soft->Draw();
  h_Pz_Soft->GetXaxis()->SetTitle("Pz_Soft [GeV/c]");
  h_Pz_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Pz_Soft->SetLineColor(2);
  c_Pz_Soft->SaveAs("Figures/h_Pz_Soft.pdf");

  TCanvas *c_E_Soft = new TCanvas("c_E_Soft", "Canvas of E_Soft distribution", 800, 400);
  c_E_Soft->cd(0);
  h_E_Soft->Draw();
  h_E_Soft->GetXaxis()->SetTitle("E_Soft [GeV/c]");
  h_E_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_E_Soft->SetLineColor(2);
  c_E_Soft->SaveAs("Figures/h_E_Soft.pdf");

  TCanvas *c_X_Soft = new TCanvas("c_X_Soft", "Canvas of X_Soft distribution", 800, 400);
  c_X_Soft->cd(0);
  h_X_Soft->Draw();
  h_X_Soft->GetXaxis()->SetTitle("Soft X-coordinate at emission point, in fm");
  h_X_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_X_Soft->SetLineColor(2);
  c_X_Soft->SaveAs("Figures/h_X_Soft.pdf");

  TCanvas *c_Y_Soft = new TCanvas("c_Y_Soft", "Canvas of Y_Soft distribution", 800, 400);
  c_Y_Soft->cd(0);
  h_Y_Soft->Draw();
  h_Y_Soft->GetXaxis()->SetTitle("Soft Y-coordinate at emission point, in fm");
  h_Y_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Y_Soft->SetLineColor(2);
  c_Y_Soft->SaveAs("Figures/h_Y_Soft.pdf");

  TCanvas *c_Z_Soft = new TCanvas("c_Z_Soft", "Canvas of Z_Soft distribution", 800, 400);
  c_Z_Soft->cd(0);
  h_Z_Soft->Draw();
  h_Z_Soft->GetXaxis()->SetTitle("Soft Z-coordinate at emission point, in fm");
  h_Z_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Z_Soft->SetLineColor(2);
  c_Z_Soft->SaveAs("Figures/h_Z_Soft.pdf");

  TCanvas *c_T_Soft = new TCanvas("c_T_Soft", "Canvas of T_Soft distribution", 800, 400);
  c_T_Soft->cd(0);
  h_T_Soft->Draw();
  h_T_Soft->GetXaxis()->SetTitle("Proper time (T) of soft particle emission at emission point, in fm/c");
  h_T_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_T_Soft->SetLineColor(2);
  c_T_Soft->SaveAs("Figures/h_T_Soft.pdf");

  TCanvas *c_pdg_Soft = new TCanvas("c_pdg_Soft", "Canvas of pdg_Soft distribution", 800, 400);
  c_pdg_Soft->cd(0);
  h_pdg_Soft->Draw();
  h_pdg_Soft->GetXaxis()->SetTitle("Geant soft particle code");
  h_pdg_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pdg_Soft->SetLineColor(2);
  c_pdg_Soft->SaveAs("Figures/h_pdg_Soft.pdf");

  TCanvas *c_Mpdg_Soft = new TCanvas("c_Mpdg_Soft", "Canvas of Mpdg_Soft distribution", 800, 400);
  c_Mpdg_Soft->cd(0);
  h_Mpdg_Soft->Draw();
  h_Mpdg_Soft->GetXaxis()->SetTitle("Geant code of soft mother particle");
  h_Mpdg_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Mpdg_Soft->SetLineColor(2);
  c_Mpdg_Soft->SaveAs("Figures/h_Mpdg_Soft.pdf");

  TCanvas *c_type_Soft = new TCanvas("c_type_Soft", "Canvas of type_Soft distribution", 800, 400);
  c_type_Soft->cd(0);
  h_type_Soft->Draw();
  h_type_Soft->GetXaxis()->SetTitle("Origin of soft particle");
  h_type_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_type_Soft->SetLineColor(2);
  c_type_Soft->SaveAs("Figures/h_type_Soft.pdf");

  TCanvas *c_Index_Soft = new TCanvas("c_Index_Soft", "Canvas of Index_Soft distribution", 800, 400);
  c_Index_Soft->cd(0);
  h_Index_Soft->Draw();
  h_Index_Soft->GetXaxis()->SetTitle("Unique zero based index of the soft particle");
  h_Index_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Index_Soft->SetLineColor(2);
  c_Index_Soft->SaveAs("Figures/h_Index_Soft.pdf");

  TCanvas *c_MotherIndex_Soft = new TCanvas("c_MotherIndex_Soft", "Canvas of MotherIndex_Soft distribution", 800, 400);
  c_MotherIndex_Soft->cd(0);
  h_MotherIndex_Soft->Draw();
  h_MotherIndex_Soft->GetXaxis()->SetTitle("MotherIndex of the soft mother particle");
  h_MotherIndex_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_MotherIndex_Soft->SetLineColor(2);
  c_MotherIndex_Soft->SaveAs("Figures/h_MotherIndex_Soft.pdf");

  TCanvas *c_NDaughters_Soft = new TCanvas("c_NDaughters_Soft", "Canvas of NDaughters_Soft distribution", 800, 400);
  c_NDaughters_Soft->cd(0);
  h_NDaughters_Soft->Draw();
  h_NDaughters_Soft->GetXaxis()->SetTitle("Number of soft daughter particles");
  h_NDaughters_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_NDaughters_Soft->SetLineColor(2);
  c_NDaughters_Soft->SaveAs("Figures/h_NDaughters_Soft.pdf");

  TCanvas *c_FirstDaughterIndex_Soft = new TCanvas("c_FirstDaughterIndex_Soft", "Canvas of FirstDaughterIndex_Soft distribution", 800, 400);
  c_FirstDaughterIndex_Soft->cd(0);
  h_FirstDaughterIndex_Soft->Draw();
  h_FirstDaughterIndex_Soft->GetXaxis()->SetTitle("Index of the first soft daughter");
  h_FirstDaughterIndex_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_FirstDaughterIndex_Soft->SetLineColor(2);
  c_FirstDaughterIndex_Soft->SaveAs("Figures/h_FirstDaughterIndex_Soft.pdf");

  TCanvas *c_LastDaughterIndex_Soft = new TCanvas("c_LastDaughterIndex_Soft", "Canvas of LastDaughterIndex_Soft distribution", 800, 400);
  c_LastDaughterIndex_Soft->cd(0);
  h_LastDaughterIndex_Soft->Draw();
  h_LastDaughterIndex_Soft->GetXaxis()->SetTitle("Index of the last soft daughter");
  h_LastDaughterIndex_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_LastDaughterIndex_Soft->SetLineColor(2);
  c_LastDaughterIndex_Soft->SaveAs("Figures/h_LastDaughterIndex_Soft.pdf");

  TCanvas *c_pythiaStatus_Soft = new TCanvas("c_pythiaStatus_Soft", "Canvas of pythiaStatus_Soft distribution", 800, 400);
  c_pythiaStatus_Soft->cd(0);
  h_pythiaStatus_Soft->Draw();
  h_pythiaStatus_Soft->GetXaxis()->SetTitle("Soft PYTHIA status code");
  h_pythiaStatus_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pythiaStatus_Soft->SetLineColor(2);
  c_pythiaStatus_Soft->SaveAs("Figures/h_pythiaStatus_Soft.pdf");

  TCanvas *c_final_Soft = new TCanvas("c_final_Soft", "Canvas of final_Soft distribution", 800, 400);
  c_final_Soft->cd(0);
  h_final_Soft->Draw();
  h_final_Soft->GetXaxis()->SetTitle("Final soft particles");
  h_final_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_final_Soft->SetLineColor(2);
  c_final_Soft->SaveAs("Figures/h_final_Soft.pdf");

  //

  TCanvas *c_Pt_Soft = new TCanvas("c_Pt_Soft", "Canvas of Pt_Soft distribution", 800, 400);
  c_Pt_Soft->cd(0);
  h_Pt_Soft->Draw();
  h_Pt_Soft->GetXaxis()->SetTitle("Pt_Soft [GeV/c]");
  h_Pt_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Pt_Soft->SetLineColor(2);
  c_Pt_Soft->SaveAs("Figures/h_Pt_Soft.pdf");

  TCanvas *c_Zcm_Soft = new TCanvas("c_Zcm_Soft", "Canvas of Zcm_Soft distribution", 800, 400);
  c_Zcm_Soft->cd(0);
  h_Zcm_Soft->Draw();
  h_Zcm_Soft->GetXaxis()->SetTitle("Soft Z-coordinate at emission point, in cm");
  h_Zcm_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Zcm_Soft->SetLineColor(2);
  c_Zcm_Soft->SaveAs("Figures/h_Zcm_Soft.pdf");

  // Hard

  TCanvas *c_Px_Hard = new TCanvas("c_Px_Hard", "Canvas of Px_Hard distribution", 800, 400);
  c_Px_Hard->cd(0);
  h_Px_Hard->Draw();
  h_Px_Hard->GetXaxis()->SetTitle("Px_Hard [GeV/c]");
  h_Px_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Px_Hard->SetLineColor(2);
  c_Px_Hard->SaveAs("Figures/h_Px_Hard.pdf");

  TCanvas *c_Py_Hard = new TCanvas("c_Py_Hard", "Canvas of Py_Hard distribution", 800, 400);
  c_Py_Hard->cd(0);
  h_Py_Hard->Draw();
  h_Py_Hard->GetXaxis()->SetTitle("Py_Hard [GeV/c]");
  h_Py_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Py_Hard->SetLineColor(2);
  c_Py_Hard->SaveAs("Figures/h_Py_Hard.pdf");

  TCanvas *c_Pz_Hard = new TCanvas("c_Pz_Hard", "Canvas of Pz_Hard distribution", 800, 400);
  c_Pz_Hard->cd(0);
  h_Pz_Hard->Draw();
  h_Pz_Hard->GetXaxis()->SetTitle("Pz_Hard [GeV/c]");
  h_Pz_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Pz_Hard->SetLineColor(2);
  c_Pz_Hard->SaveAs("Figures/h_Pz_Hard.pdf");

  TCanvas *c_E_Hard = new TCanvas("c_E_Hard", "Canvas of E_Hard distribution", 800, 400);
  c_E_Hard->cd(0);
  h_E_Hard->Draw();
  h_E_Hard->GetXaxis()->SetTitle("E_Hard [GeV/c]");
  h_E_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_E_Hard->SetLineColor(2);
  c_E_Hard->SaveAs("Figures/h_E_Hard.pdf");

  TCanvas *c_X_Hard = new TCanvas("c_X_Hard", "Canvas of X_Hard distribution", 800, 400);
  c_X_Hard->cd(0);
  h_X_Hard->Draw();
  h_X_Hard->GetXaxis()->SetTitle("Hard X-coordinate at emission point, in fm");
  h_X_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_X_Hard->SetLineColor(2);
  c_X_Hard->SaveAs("Figures/h_X_Hard.pdf");

  TCanvas *c_Y_Hard = new TCanvas("c_Y_Hard", "Canvas of Y_Hard distribution", 800, 400);
  c_Y_Hard->cd(0);
  h_Y_Hard->Draw();
  h_Y_Hard->GetXaxis()->SetTitle("Hard Y-coordinate at emission point, in fm");
  h_Y_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Y_Hard->SetLineColor(2);
  c_Y_Hard->SaveAs("Figures/h_Y_Hard.pdf");

  TCanvas *c_Z_Hard = new TCanvas("c_Z_Hard", "Canvas of Z_Hard distribution", 800, 400);
  c_Z_Hard->cd(0);
  h_Z_Hard->Draw();
  h_Z_Hard->GetXaxis()->SetTitle("Hard Z-coordinate at emission point, in fm");
  h_Z_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Z_Hard->SetLineColor(2);
  c_Z_Hard->SaveAs("Figures/h_Z_Hard.pdf");

  TCanvas *c_T_Hard = new TCanvas("c_T_Hard", "Canvas of T_Hard distribution", 800, 400);
  c_T_Hard->cd(0);
  h_T_Hard->Draw();
  h_T_Hard->GetXaxis()->SetTitle("Proper time (T) of hard particle emission at emission point, in fm/c");
  h_T_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_T_Hard->SetLineColor(2);
  c_T_Hard->SaveAs("Figures/h_T_Hard.pdf");

  TCanvas *c_pdg_Hard = new TCanvas("c_pdg_Hard", "Canvas of pdg_Hard distribution", 800, 400);
  c_pdg_Hard->cd(0);
  h_pdg_Hard->Draw();
  h_pdg_Hard->GetXaxis()->SetTitle("Geant hard particle code");
  h_pdg_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pdg_Hard->SetLineColor(2);
  c_pdg_Hard->SaveAs("Figures/h_pdg_Hard.pdf");

  TCanvas *c_Mpdg_Hard = new TCanvas("c_Mpdg_Hard", "Canvas of Mpdg_Hard distribution", 800, 400);
  c_Mpdg_Hard->cd(0);
  h_Mpdg_Hard->Draw();
  h_Mpdg_Hard->GetXaxis()->SetTitle("Geant code of hard mother particle");
  h_Mpdg_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Mpdg_Hard->SetLineColor(2);
  c_Mpdg_Hard->SaveAs("Figures/h_Mpdg_Hard.pdf");

  TCanvas *c_type_Hard = new TCanvas("c_type_Hard", "Canvas of type_Hard distribution", 800, 400);
  c_type_Hard->cd(0);
  h_type_Hard->Draw();
  h_type_Hard->GetXaxis()->SetTitle("Origin of hard particle");
  h_type_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_type_Hard->SetLineColor(2);
  c_type_Hard->SaveAs("Figures/h_type_Hard.pdf");

  TCanvas *c_Index_Hard = new TCanvas("c_Index_Hard", "Canvas of Index_Hard distribution", 800, 400);
  c_Index_Hard->cd(0);
  h_Index_Hard->Draw();
  h_Index_Hard->GetXaxis()->SetTitle("Unique zero based index of the hard particle");
  h_Index_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Index_Hard->SetLineColor(2);
  c_Index_Hard->SaveAs("Figures/h_Index_Hard.pdf");

  TCanvas *c_MotherIndex_Hard = new TCanvas("c_MotherIndex_Hard", "Canvas of MotherIndex_Hard distribution", 800, 400);
  c_MotherIndex_Hard->cd(0);
  h_MotherIndex_Hard->Draw();
  h_MotherIndex_Hard->GetXaxis()->SetTitle("MotherIndex of the hard mother particle");
  h_MotherIndex_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_MotherIndex_Hard->SetLineColor(2);
  c_MotherIndex_Hard->SaveAs("Figures/h_MotherIndex_Hard.pdf");

  TCanvas *c_NDaughters_Hard = new TCanvas("c_NDaughters_Hard", "Canvas of NDaughters_Hard distribution", 800, 400);
  c_NDaughters_Hard->cd(0);
  h_NDaughters_Hard->Draw();
  h_NDaughters_Hard->GetXaxis()->SetTitle("Number of hard daughter particles");
  h_NDaughters_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_NDaughters_Hard->SetLineColor(2);
  c_NDaughters_Hard->SaveAs("Figures/h_NDaughters_Hard.pdf");

  TCanvas *c_FirstDaughterIndex_Hard = new TCanvas("c_FirstDaughterIndex_Hard", "Canvas of FirstDaughterIndex_Hard distribution", 800, 400);
  c_FirstDaughterIndex_Hard->cd(0);
  h_FirstDaughterIndex_Hard->Draw();
  h_FirstDaughterIndex_Hard->GetXaxis()->SetTitle("Index of the first hard daughter");
  h_FirstDaughterIndex_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_FirstDaughterIndex_Hard->SetLineColor(2);
  c_FirstDaughterIndex_Hard->SaveAs("Figures/h_FirstDaughterIndex_Hard.pdf");

  TCanvas *c_LastDaughterIndex_Hard = new TCanvas("c_LastDaughterIndex_Hard", "Canvas of LastDaughterIndex_Hard distribution", 800, 400);
  c_LastDaughterIndex_Hard->cd(0);
  h_LastDaughterIndex_Hard->Draw();
  h_LastDaughterIndex_Hard->GetXaxis()->SetTitle("Index of the last hard daughter");
  h_LastDaughterIndex_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_LastDaughterIndex_Hard->SetLineColor(2);
  c_LastDaughterIndex_Hard->SaveAs("Figures/h_LastDaughterIndex_Hard.pdf");

  TCanvas *c_pythiaStatus_Hard = new TCanvas("c_pythiaStatus_Hard", "Canvas of pythiaStatus_Hard distribution", 800, 400);
  c_pythiaStatus_Hard->cd(0);
  h_pythiaStatus_Hard->Draw();
  h_pythiaStatus_Hard->GetXaxis()->SetTitle("Hard PYTHIA status code");
  h_pythiaStatus_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pythiaStatus_Hard->SetLineColor(2);
  c_pythiaStatus_Hard->SaveAs("Figures/h_pythiaStatus_Hard.pdf");

  TCanvas *c_final_Hard = new TCanvas("c_final_Hard", "Canvas of final_Hard distribution", 800, 400);
  c_final_Hard->cd(0);
  h_final_Hard->Draw();
  h_final_Hard->GetXaxis()->SetTitle("Final hard particles");
  h_final_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_final_Hard->SetLineColor(2);
  c_final_Hard->SaveAs("Figures/h_final_Hard.pdf");

  //

  TCanvas *c_Pt_Hard = new TCanvas("c_Pt_Hard", "Canvas of Pt_Hard distribution", 800, 400);
  c_Pt_Hard->cd(0);
  h_Pt_Hard->Draw();
  h_Pt_Hard->GetXaxis()->SetTitle("Pt_Hard [GeV/c]");
  h_Pt_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Pt_Hard->SetLineColor(2);
  c_Pt_Hard->SaveAs("Figures/h_Pt_Hard.pdf");

  TCanvas *c_Zcm_Hard = new TCanvas("c_Zcm_Hard", "Canvas of Zcm_Hard distribution", 800, 400);
  c_Zcm_Hard->cd(0);
  h_Zcm_Hard->Draw();
  h_Zcm_Hard->GetXaxis()->SetTitle("Hard Z-coordinate at emission point, in cm");
  h_Zcm_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Zcm_Hard->SetLineColor(2);
  c_Zcm_Hard->SaveAs("Figures/h_Zcm_Hard.pdf");

  // All x Soft x Hard
  
  TCanvas *c_Px_All = new TCanvas("c_Px_All", "Canvas of Px_All distribution", 800, 400);
  c_Px_All->cd(0);
  TH1F *h_Px_clone = (TH1F *)h_Px->Clone("h_Px_Total");
  h_Px_clone->Draw();
  h_Px_clone->GetXaxis()->SetTitle("Px [GeV/c]");
  h_Px_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Px_clone->SetLineColor(1);
  h_Px_clone->SetLineWidth(3);
  TH1F *h_Px_Soft_clone = (TH1F *)h_Px_Soft->Clone("h_Px_Soft");
  h_Px_Soft_clone->Draw("same");
  h_Px_Soft_clone->SetLineColor(4);
  h_Px_Soft_clone->SetLineWidth(2);
  TH1F *h_Px_Hard_clone = (TH1F *)h_Px_Hard->Clone("h_Px_Hard");
  h_Px_Hard_clone->Draw("same");
  h_Px_Hard_clone->SetLineColor(2);
  h_Px_Hard_clone->SetLineWidth(1);
  c_Px_All->SaveAs("Figures/3in1/h_Px_All.pdf");

  TCanvas *c_Py_All = new TCanvas("c_Py_All", "Canvas of Py_All distribution", 800, 400);
  c_Py_All->cd(0);
  TH1F *h_Py_clone = (TH1F *)h_Py->Clone("h_Py_Total");
  h_Py_clone->Draw();
  h_Py_clone->GetXaxis()->SetTitle("Py [GeV/c]");
  h_Py_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Py_clone->SetLineColor(1);
  h_Py_clone->SetLineWidth(3);
  TH1F *h_Py_Soft_clone = (TH1F *)h_Py_Soft->Clone("h_Py_Soft");
  h_Py_Soft_clone->Draw("same");
  h_Py_Soft_clone->SetLineColor(4);
  h_Py_Soft_clone->SetLineWidth(2);
  TH1F *h_Py_Hard_clone = (TH1F *)h_Py_Hard->Clone("h_Py_Hard");
  h_Py_Hard_clone->Draw("same");
  h_Py_Hard_clone->SetLineColor(2);
  h_Py_Hard_clone->SetLineWidth(1);
  c_Py_All->SaveAs("Figures/3in1/h_Py_All.pdf");

  TCanvas *c_Pz_All = new TCanvas("c_Pz_All", "Canvas of Pz_All distribution", 800, 400);
  c_Pz_All->cd(0);
  TH1F *h_Pz_clone = (TH1F *)h_Pz->Clone("h_Pz_Total");
  h_Pz_clone->Draw();
  h_Pz_clone->GetXaxis()->SetTitle("Pz [GeV/c]");
  h_Pz_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Pz_clone->SetLineColor(1);
  h_Pz_clone->SetLineWidth(3);
  TH1F *h_Pz_Soft_clone = (TH1F *)h_Pz_Soft->Clone("h_Pz_Soft");
  h_Pz_Soft_clone->Draw("same");
  h_Pz_Soft_clone->SetLineColor(4);
  h_Pz_Soft_clone->SetLineWidth(2);
  TH1F *h_Pz_Hard_clone = (TH1F *)h_Pz_Hard->Clone("h_Pz_Hard");
  h_Pz_Hard_clone->Draw("same");
  h_Pz_Hard_clone->SetLineColor(2);
  h_Pz_Hard_clone->SetLineWidth(1);
  c_Pz_All->SaveAs("Figures/3in1/h_Pz_All.pdf");

  TCanvas *c_E_All = new TCanvas("c_E_All", "Canvas of E_All distribution", 800, 400);
  c_E_All->cd(0);
  TH1F *h_E_clone = (TH1F *)h_E->Clone("h_E_Total");
  h_E_clone->Draw();
  h_E_clone->GetXaxis()->SetTitle("E [GeV]");
  h_E_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_E_clone->SetLineColor(1);
  h_E_clone->SetLineWidth(3);
  TH1F *h_E_Soft_clone = (TH1F *)h_E_Soft->Clone("h_E_Soft");
  h_E_Soft_clone->Draw("same");
  h_E_Soft_clone->SetLineColor(4);
  h_E_Soft_clone->SetLineWidth(2);
  TH1F *h_E_Hard_clone = (TH1F *)h_E_Hard->Clone("h_E_Hard");
  h_E_Hard_clone->Draw("same");
  h_E_Hard_clone->SetLineColor(2);
  h_E_Hard_clone->SetLineWidth(1);
  c_E_All->SaveAs("Figures/3in1/h_E_All.pdf");

  TCanvas *c_X_All = new TCanvas("c_X_All", "Canvas of X_All distribution", 800, 400);
  c_X_All->cd(0);
  TH1F *h_X_clone = (TH1F *)h_X->Clone("h_X_Total");
  h_X_clone->Draw();
  h_X_clone->GetXaxis()->SetTitle("X-coordinate at emission point, in fm");
  h_X_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_X_clone->SetLineColor(1);
  h_X_clone->SetLineWidth(3);
  TH1F *h_X_Soft_clone = (TH1F *)h_X_Soft->Clone("h_X_Soft");
  h_X_Soft_clone->Draw("same");
  h_X_Soft_clone->SetLineColor(4);
  h_X_Soft_clone->SetLineWidth(2);
  TH1F *h_X_Hard_clone = (TH1F *)h_X_Hard->Clone("h_X_Hard");
  h_X_Hard_clone->Draw("same");
  h_X_Hard_clone->SetLineColor(2);
  h_X_Hard_clone->SetLineWidth(1);
  c_X_All->SaveAs("Figures/3in1/h_X_All.pdf");

  TCanvas *c_Y_All = new TCanvas("c_Y_All", "Canvas of Y_All distribution", 800, 400);
  c_Y_All->cd(0);
  TH1F *h_Y_clone = (TH1F *)h_Y->Clone("h_Y_Total");
  h_Y_clone->Draw();
  h_Y_clone->GetXaxis()->SetTitle("Y-coordinate at emission point, in fm");
  h_Y_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Y_clone->SetLineColor(1);
  h_Y_clone->SetLineWidth(3);
  TH1F *h_Y_Soft_clone = (TH1F *)h_Y_Soft->Clone("h_Y_Soft");
  h_Y_Soft_clone->Draw("same");
  h_Y_Soft_clone->SetLineColor(4);
  h_Y_Soft_clone->SetLineWidth(2);
  TH1F *h_Y_Hard_clone = (TH1F *)h_Y_Hard->Clone("h_Y_Hard");
  h_Y_Hard_clone->Draw("same");
  h_Y_Hard_clone->SetLineColor(2);
  h_Y_Hard_clone->SetLineWidth(1);
  c_Y_All->SaveAs("Figures/3in1/h_Y_All.pdf");

  TCanvas *c_Z_All = new TCanvas("c_Z_All", "Canvas of Z_All distribution", 800, 400);
  c_Z_All->cd(0);
  TH1F *h_Z_clone = (TH1F *)h_Z->Clone("h_Z_Total");
  h_Z_clone->Draw();
  h_Z_clone->GetXaxis()->SetTitle("Z-coordinate at emission point, in fm");
  h_Z_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Z_clone->SetLineColor(1);
  h_Z_clone->SetLineWidth(3);
  TH1F *h_Z_Soft_clone = (TH1F *)h_Z_Soft->Clone("h_Z_Soft");
  h_Z_Soft_clone->Draw("same");
  h_Z_Soft_clone->SetLineColor(4);
  h_Z_Soft_clone->SetLineWidth(2);
  TH1F *h_Z_Hard_clone = (TH1F *)h_Z_Hard->Clone("h_Z_Hard");
  h_Z_Hard_clone->Draw("same");
  h_Z_Hard_clone->SetLineColor(2);
  h_Z_Hard_clone->SetLineWidth(1);
  c_Z_All->SaveAs("Figures/3in1/h_Z_All.pdf");

  TCanvas *c_T_All = new TCanvas("c_T_All", "Canvas of T_All distribution", 800, 400);
  c_T_All->cd(0);
  TH1F *h_T_clone = (TH1F *)h_T->Clone("h_T_Total");
  h_T_clone->Draw();
  h_T_clone->GetXaxis()->SetTitle("Proper time (T) of particle emission at emission point, in fm/c");
  h_T_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_T_clone->SetLineColor(1);
  h_T_clone->SetLineWidth(3);
  TH1F *h_T_Soft_clone = (TH1F *)h_T_Soft->Clone("h_T_Soft");
  h_T_Soft_clone->Draw("same");
  h_T_Soft_clone->SetLineColor(4);
  h_T_Soft_clone->SetLineWidth(2);
  TH1F *h_T_Hard_clone = (TH1F *)h_T_Hard->Clone("h_T_Hard");
  h_T_Hard_clone->Draw("same");
  h_T_Hard_clone->SetLineColor(2);
  h_T_Hard_clone->SetLineWidth(1);
  c_T_All->SaveAs("Figures/3in1/h_T_All.pdf");

  TCanvas *c_pdg_All = new TCanvas("c_pdg_All", "Canvas of pdg_All distribution", 800, 400);
  c_pdg_All->cd(0);
  TH1I *h_pdg_clone = (TH1I *)h_pdg->Clone("h_pdg_Total");
  h_pdg_clone->Draw();
  h_pdg_clone->GetXaxis()->SetTitle("Geant particle code");
  h_pdg_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pdg_clone->SetLineColor(1);
  h_pdg_clone->SetLineWidth(3);
  TH1I *h_pdg_Soft_clone = (TH1I *)h_pdg_Soft->Clone("h_pdg_Soft");
  h_pdg_Soft_clone->Draw("same");
  h_pdg_Soft_clone->SetLineColor(4);
  h_pdg_Soft_clone->SetLineWidth(2);
  TH1I *h_pdg_Hard_clone = (TH1I *)h_pdg_Hard->Clone("h_pdg_Hard");
  h_pdg_Hard_clone->Draw("same");
  h_pdg_Hard_clone->SetLineColor(2);
  h_pdg_Hard_clone->SetLineWidth(1);
  c_pdg_All->SaveAs("Figures/3in1/h_pdg_All.pdf");

  TCanvas *c_Mpdg_All = new TCanvas("c_Mpdg_All", "Canvas of Mpdg_All distribution", 800, 400);
  c_Mpdg_All->cd(0);
  TH1I *h_Mpdg_clone = (TH1I *)h_Mpdg->Clone("h_Mpdg_Total");
  h_Mpdg_clone->Draw();
  h_Mpdg_clone->GetXaxis()->SetTitle("Geant code of mother particle, -1 for primordinary particles");
  h_Mpdg_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Mpdg_clone->SetLineColor(1);
  h_Mpdg_clone->SetLineWidth(3);
  TH1I *h_Mpdg_Soft_clone = (TH1I *)h_Mpdg_Soft->Clone("h_Mpdg_Soft");
  h_Mpdg_Soft_clone->Draw("same");
  h_Mpdg_Soft_clone->SetLineColor(4);
  h_Mpdg_Soft_clone->SetLineWidth(2);
  TH1I *h_Mpdg_Hard_clone = (TH1I *)h_Mpdg_Hard->Clone("h_Mpdg_Hard");
  h_Mpdg_Hard_clone->Draw("same");
  h_Mpdg_Hard_clone->SetLineColor(2);
  h_Mpdg_Hard_clone->SetLineWidth(1);
  c_Mpdg_All->SaveAs("Figures/3in1/h_Mpdg_All.pdf");

  TCanvas *c_type_All = new TCanvas("c_type_All", "Canvas of type_All distribution", 800, 400);
  c_type_All->cd(0);
  TH1I *h_type_clone = (TH1I *)h_type->Clone("h_type_Total");
  h_type_clone->Draw();
  h_type_clone->GetXaxis()->SetTitle("Origin of particle, (=0 -> from soft hydro part, >0 -> from hard jet part)");
  h_type_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_type_clone->SetLineColor(1);
  h_type_clone->SetLineWidth(3);
  TH1I *h_type_Soft_clone = (TH1I *)h_type_Soft->Clone("h_type_Soft");
  h_type_Soft_clone->Draw("same");
  h_type_Soft_clone->SetLineColor(4);
  h_type_Soft_clone->SetLineWidth(2);
  TH1I *h_type_Hard_clone = (TH1I *)h_type_Hard->Clone("h_type_Hard");
  h_type_Hard_clone->Draw("same");
  h_type_Hard_clone->SetLineColor(2);
  h_type_Hard_clone->SetLineWidth(1);
  c_type_All->SaveAs("Figures/3in1/h_type_All.pdf");

  TCanvas *c_Index_All = new TCanvas("c_Index_All", "Canvas of Index_All distribution", 800, 400);
  c_Index_All->cd(0);
  TH1I *h_Index_clone = (TH1I *)h_Index->Clone("h_Index_Total");
  h_Index_clone->Draw();
  h_Index_clone->GetXaxis()->SetTitle("Unique zero based index of the particle");
  h_Index_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Index_clone->SetLineColor(1);
  h_Index_clone->SetLineWidth(3);
  TH1I *h_Index_Soft_clone = (TH1I *)h_Index_Soft->Clone("h_Index_Soft");
  h_Index_Soft_clone->Draw("same");
  h_Index_Soft_clone->SetLineColor(4);
  h_Index_Soft_clone->SetLineWidth(2);
  TH1I *h_Index_Hard_clone = (TH1I *)h_Index_Hard->Clone("h_Index_Hard");
  h_Index_Hard_clone->Draw("same");
  h_Index_Hard_clone->SetLineColor(2);
  h_Index_Hard_clone->SetLineWidth(1);
  c_Index_All->SaveAs("Figures/3in1/h_Index_All.pdf");

  TCanvas *c_MotherIndex_All = new TCanvas("c_MotherIndex_All", "Canvas of MotherIndex_All distribution", 800, 400);
  c_MotherIndex_All->cd(0);
  TH1I *h_MotherIndex_clone = (TH1I *)h_MotherIndex->Clone("h_MotherIndex_Total");
  h_MotherIndex_clone->Draw();
  h_MotherIndex_clone->GetXaxis()->SetTitle("MotherIndex of the mother particle (-1 if its a primary particle)");
  h_MotherIndex_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_MotherIndex_clone->SetLineColor(1);
  h_MotherIndex_clone->SetLineWidth(3);
  TH1I *h_MotherIndex_Soft_clone = (TH1I *)h_MotherIndex_Soft->Clone("h_MotherIndex_Soft");
  h_MotherIndex_Soft_clone->Draw("same");
  h_MotherIndex_Soft_clone->SetLineColor(4);
  h_MotherIndex_Soft_clone->SetLineWidth(2);
  TH1I *h_MotherIndex_Hard_clone = (TH1I *)h_MotherIndex_Hard->Clone("h_MotherIndex_Hard");
  h_MotherIndex_Hard_clone->Draw("same");
  h_MotherIndex_Hard_clone->SetLineColor(2);
  h_MotherIndex_Hard_clone->SetLineWidth(1);
  c_MotherIndex_All->SaveAs("Figures/3in1/h_MotherIndex_All.pdf");

  TCanvas *c_NDaughters_All = new TCanvas("c_NDaughters_All", "Canvas of NDaughters_All distribution", 800, 400);
  c_NDaughters_All->cd(0);
  TH1I *h_NDaughters_clone = (TH1I *)h_NDaughters->Clone("h_NDaughters_Total");
  h_NDaughters_clone->Draw();
  h_NDaughters_clone->GetXaxis()->SetTitle("Number of daughter particles");
  h_NDaughters_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_NDaughters_clone->SetLineColor(1);
  h_NDaughters_clone->SetLineWidth(3);
  TH1I *h_NDaughters_Soft_clone = (TH1I *)h_NDaughters_Soft->Clone("h_NDaughters_Soft");
  h_NDaughters_Soft_clone->Draw("same");
  h_NDaughters_Soft_clone->SetLineColor(4);
  h_NDaughters_Soft_clone->SetLineWidth(2);
  TH1I *h_NDaughters_Hard_clone = (TH1I *)h_NDaughters_Hard->Clone("h_NDaughters_Hard");
  h_NDaughters_Hard_clone->Draw("same");
  h_NDaughters_Hard_clone->SetLineColor(2);
  h_NDaughters_Hard_clone->SetLineWidth(1);
  c_NDaughters_All->SaveAs("Figures/3in1/h_NDaughters_All.pdf");

  TCanvas *c_FirstDaughterIndex_All = new TCanvas("c_FirstDaughterIndex_All", "Canvas of FirstDaughterIndex_All distribution", 800, 400);
  c_FirstDaughterIndex_All->cd(0);
  TH1I *h_FirstDaughterIndex_clone = (TH1I *)h_FirstDaughterIndex->Clone("h_FirstDaughterIndex_Total");
  h_FirstDaughterIndex_clone->Draw();
  h_FirstDaughterIndex_clone->GetXaxis()->SetTitle("Index of the first daughter (-1 if it does not exist)");
  h_FirstDaughterIndex_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_FirstDaughterIndex_clone->SetLineColor(1);
  h_FirstDaughterIndex_clone->SetLineWidth(3);
  TH1I *h_FirstDaughterIndex_Soft_clone = (TH1I *)h_FirstDaughterIndex_Soft->Clone("h_FirstDaughterIndex_Soft");
  h_FirstDaughterIndex_Soft_clone->Draw("same");
  h_FirstDaughterIndex_Soft_clone->SetLineColor(4);
  h_FirstDaughterIndex_Soft_clone->SetLineWidth(2);
  TH1I *h_FirstDaughterIndex_Hard_clone = (TH1I *)h_FirstDaughterIndex_Hard->Clone("h_FirstDaughterIndex_Hard");
  h_FirstDaughterIndex_Hard_clone->Draw("same");
  h_FirstDaughterIndex_Hard_clone->SetLineColor(2);
  h_FirstDaughterIndex_Hard_clone->SetLineWidth(1);
  c_FirstDaughterIndex_All->SaveAs("Figures/3in1/h_FirstDaughterIndex_All.pdf");

  TCanvas *c_LastDaughterIndex_All = new TCanvas("c_LastDaughterIndex_All", "Canvas of LastDaughterIndex_All distribution", 800, 400);
  c_LastDaughterIndex_All->cd(0);
  TH1I *h_LastDaughterIndex_clone = (TH1I *)h_LastDaughterIndex->Clone("h_LastDaughterIndex_Total");
  h_LastDaughterIndex_clone->Draw();
  h_LastDaughterIndex_clone->GetXaxis()->SetTitle("Index of the last daughter)");
  h_LastDaughterIndex_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_LastDaughterIndex_clone->SetLineColor(1);
  h_LastDaughterIndex_clone->SetLineWidth(3);
  TH1I *h_LastDaughterIndex_Soft_clone = (TH1I *)h_LastDaughterIndex_Soft->Clone("h_LastDaughterIndex_Soft");
  h_LastDaughterIndex_Soft_clone->Draw("same");
  h_LastDaughterIndex_Soft_clone->SetLineColor(4);
  h_LastDaughterIndex_Soft_clone->SetLineWidth(2);
  TH1I *h_LastDaughterIndex_Hard_clone = (TH1I *)h_LastDaughterIndex_Hard->Clone("h_LastDaughterIndex_Hard");
  h_LastDaughterIndex_Hard_clone->Draw("same");
  h_LastDaughterIndex_Hard_clone->SetLineColor(2);
  h_LastDaughterIndex_Hard_clone->SetLineWidth(1);
  c_LastDaughterIndex_All->SaveAs("Figures/3in1/h_LastDaughterIndex_All.pdf");

  TCanvas *c_pythiaStatus_All = new TCanvas("c_pythiaStatus_All", "Canvas of pythiaStatus_All distribution", 800, 400);
  c_pythiaStatus_All->cd(0);
  TH1I *h_pythiaStatus_clone = (TH1I *)h_pythiaStatus->Clone("h_pythiaStatus_Total");
  h_pythiaStatus_clone->Draw();
  h_pythiaStatus_clone->GetXaxis()->SetTitle("PYTHIA status code (-1 if the particle is from soft component)");
  h_pythiaStatus_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pythiaStatus_clone->SetLineColor(1);
  h_pythiaStatus_clone->SetLineWidth(3);
  TH1I *h_pythiaStatus_Soft_clone = (TH1I *)h_pythiaStatus_Soft->Clone("h_pythiaStatus_Soft");
  h_pythiaStatus_Soft_clone->Draw("same");
  h_pythiaStatus_Soft_clone->SetLineColor(4);
  h_pythiaStatus_Soft_clone->SetLineWidth(2);
  TH1I *h_pythiaStatus_Hard_clone = (TH1I *)h_pythiaStatus_Hard->Clone("h_pythiaStatus_Hard");
  h_pythiaStatus_Hard_clone->Draw("same");
  h_pythiaStatus_Hard_clone->SetLineColor(2);
  h_pythiaStatus_Hard_clone->SetLineWidth(1);
  c_pythiaStatus_All->SaveAs("Figures/3in1/h_pythiaStatus_All.pdf");

  TCanvas *c_final_All = new TCanvas("c_final_All", "Canvas of final_All distribution", 800, 400);
  c_final_All->cd(0);
  TH1I *h_final_clone = (TH1I *)h_final->Clone("h_final_Total");
  h_final_clone->Draw();
  h_final_clone->GetXaxis()->SetTitle("Integer branch keeping 0 for decayed particles and 1 for final state particles");
  h_final_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_final_clone->SetLineColor(1);
  h_final_clone->SetLineWidth(3);
  TH1I *h_final_Soft_clone = (TH1I *)h_final_Soft->Clone("h_final_Soft");
  h_final_Soft_clone->Draw("same");
  h_final_Soft_clone->SetLineColor(4);
  h_final_Soft_clone->SetLineWidth(2);
  TH1I *h_final_Hard_clone = (TH1I *)h_final_Hard->Clone("h_final_Hard");
  h_final_Hard_clone->Draw("same");
  h_final_Hard_clone->SetLineColor(2);
  h_final_Hard_clone->SetLineWidth(1);
  c_final_All->SaveAs("Figures/3in1/h_final_All.pdf");

  //

  TCanvas *c_Pt_All = new TCanvas("c_Pt_All", "Canvas of Pt_All distribution", 800, 400);
  c_Pt_All->cd(0);
  TH1F *h_Pt_clone = (TH1F *)h_Pt->Clone("h_Pt_Total");
  h_Pt_clone->Draw();
  h_Pt_clone->GetXaxis()->SetTitle("Pt [GeV/c]");
  h_Pt_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Pt_clone->SetLineColor(1);
  h_Pt_clone->SetLineWidth(3);
  TH1F *h_Pt_Soft_clone = (TH1F *)h_Pt_Soft->Clone("h_Pt_Soft");
  h_Pt_Soft_clone->Draw("same");
  h_Pt_Soft_clone->SetLineColor(4);
  h_Pt_Soft_clone->SetLineWidth(2);
  TH1F *h_Pt_Hard_clone = (TH1F *)h_Pt_Hard->Clone("h_Pt_Hard");
  h_Pt_Hard_clone->Draw("same");
  h_Pt_Hard_clone->SetLineColor(2);
  h_Pt_Hard_clone->SetLineWidth(1);
  c_Pt_All->SaveAs("Figures/3in1/h_Pt_All.pdf");

  TCanvas *c_Zcm_All = new TCanvas("c_Zcm_All", "Canvas of Zcm_All distribution", 800, 400);
  c_Zcm_All->cd(0);
  TH1D *h_Zcm_clone = (TH1D *)h_Zcm->Clone("h_Zcm_Total");
  h_Zcm_clone->Draw();
  h_Zcm_clone->GetXaxis()->SetTitle("Z-coordinate at emission point, in cm");
  h_Zcm_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Zcm_clone->SetLineColor(1);
  h_Zcm_clone->SetLineWidth(3);
  TH1D *h_Zcm_Soft_clone = (TH1D *)h_Zcm_Soft->Clone("h_Zcm_Soft");
  h_Zcm_Soft_clone->Draw("same");
  h_Zcm_Soft_clone->SetLineColor(4);
  h_Zcm_Soft_clone->SetLineWidth(2);
  TH1D *h_Zcm_Hard_clone = (TH1D *)h_Zcm_Hard->Clone("h_Zcm_Hard");
  h_Zcm_Hard_clone->Draw("same");
  h_Zcm_Hard_clone->SetLineColor(2);
  h_Zcm_Hard_clone->SetLineWidth(1);
  c_Zcm_All->SaveAs("Figures/3in1/h_Zcm_All.pdf");

}
