//#include <stdio.h>
//#include <iostream>
//#include "TTree.h"
//#include "TFile.h"
//#include "TMath.h"
//#include "TH1.h"
//#include "TString.h"
//#include "TCanvas.h"
//void read_ROOT_file_partfinal ()

{

  Int_t particle_type = 3; // 0 for all (pions, kaons, protons); 1 for pions; 2 for kaons; 3 for protons

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
  Int_t pdg[1000000]; // Geant particle code
  Int_t type[1000000]; // origin of particle (=0 - from soft hydro part, >0 - from hard jet part)
  Int_t MotherIndex[1000000]; // index of the mother particle (-1 if its a primary particle)
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
  input_tree->SetBranchAddress("pdg",&pdg);
  input_tree->SetBranchAddress("type",&type);
  input_tree->SetBranchAddress("MotherIndex",&MotherIndex);
  input_tree->SetBranchAddress("final",&final);

  TString hist_title_label = "";
  if (particle_type == 0){hist_title_label = "Pions + Kaons + Protons - ";}
  else if (particle_type == 1){hist_title_label = "Pions - ";}
  else if (particle_type == 2){hist_title_label = "Kaons - ";}
  else if (particle_type == 3){hist_title_label = "Protons - ";}
  else{}

  TH1I *h_nev = new TH1I("h_nev","nev distribution",1000,-0.5,1000);
  TH1F *h_Bgen = new TH1F("h_Bgen","Bgen distribution",250,0,2.5);
  TH1F *h_Sigin = new TH1F("h_Sigin","Sigin distribution",100,-5,100);
  TH1F *h_Sigjet = new TH1F("h_Sigjet","Sigjet distribution",100,-5,5);
  TH1I *h_Ntot = new TH1I("h_Ntot","Ntot distribution",100,0,100000);
  TH1I *h_Nhyd = new TH1I("h_Nhyd","Nhyd distribution",100,0,100000);
  TH1I *h_Npyt = new TH1I("h_Npyt","Npyt distribution",100,0,100000);
  TH1I *h_Njet = new TH1I("h_Njet","Njet distribution",100,0,100);
  TH1I *h_Nbcol = new TH1I("h_Nbcol","Nbcol distribution",100,1500,2000);
  TH1I *h_Npart = new TH1I("h_Npart","Npart distribution",100,0,500);
  TH1F *h_Px = new TH1F("h_Px",hist_title_label + "Px distribution",100,-5,5);
  TH1F *h_Py = new TH1F("h_Py",hist_title_label + "Py distribution",100,-5,5);
  TH1F *h_Pz = new TH1F("h_Pz",hist_title_label + "Pz distribution",100,-20,20);
  TH1F *h_E = new TH1F("h_E",hist_title_label + "Energy distribution",100,-0.5,500);
  TH1I *h_pdg = new TH1I("h_pdg",hist_title_label + "Geant particle code",100,-2300,2300);
  TH1I *h_type = new TH1I("h_type",hist_title_label + "Origin of particle (type)",4,-0.5,1.5);
  TH1I *h_MotherIndex = new TH1I("h_MotherIndex",hist_title_label + "Index of the mother particle",4,-2,2);
  TH1I *h_final = new TH1I("h_final",hist_title_label + "Final state",3,-1,2);

  // my variables (histograms)

  TH1F *h_Pt = new TH1F("h_Pt",hist_title_label + "Pt distribution",200,0,20);

  TH1F *h_Pt_Soft = new TH1F("h_Pt_Soft",hist_title_label + "Soft Pt distribution",200,0,20);
  TH1F *h_Pt_Hard = new TH1F("h_Pt_Hard",hist_title_label + "Hard Pt distribution",200,0,20);

  // 4 - vetor
  
  TH1D *h_pt = new TH1D("h_pt",hist_title_label + "pt distribution",200,0,20);
  TH1D *h_eta = new TH1D("h_eta",hist_title_label + "Eta distribution",200,-10,10);
  TH1D *h_phi = new TH1D("h_phi",hist_title_label + "Phi distribution",100,-3.5,3.5);
  TH1D *h_mass = new TH1D("h_mass",hist_title_label + "Mass distribution",100,0,2);

  //4 - vetor soft
 
  TH1D *h_pt_Soft = new TH1D("h_pt_Soft",hist_title_label + "Soft pt distribution",200,0,20);
  TH1D *h_eta_Soft = new TH1D("h_eta_Soft",hist_title_label + "Soft eta distribution",200,-10,10);
  TH1D *h_phi_Soft = new TH1D("h_phi_Soft",hist_title_label + "Soft phi distribution",100,-3.5,3.5);
  TH1D *h_mass_Soft = new TH1D("h_mass_Soft",hist_title_label + "Soft mass distribution",100,0,2);

  //4 - vetor hard

  TH1D *h_pt_Hard = new TH1D("h_pt_Hard",hist_title_label + "Hard pt distribution",200,0,20);
  TH1D *h_eta_Hard = new TH1D("h_eta_Hard",hist_title_label + "Hard eta distribution",200,-10,10);
  TH1D *h_phi_Hard = new TH1D("h_phi_Hard",hist_title_label + "Hard phi distribution",100,-3.5,3.5);
  TH1D *h_mass_Hard = new TH1D("h_mass_Hard",hist_title_label + "Hard mass distribution",100,0,2);

  //

  TH1F *h_Px_Soft = new TH1F("h_Px_Soft",hist_title_label + "Soft Px distribution",100,-5,5);
  TH1F *h_Px_Hard = new TH1F("h_Px_Hard",hist_title_label + "Hard Px distribution",100,-5,5);
  TH1F *h_Py_Soft = new TH1F("h_Py_Soft",hist_title_label + "Soft Py distribution",100,-5,5);
  TH1F *h_Py_Hard = new TH1F("h_Py_Hard",hist_title_label + "Hard Py distribution",100,-5,5);
  TH1F *h_Pz_Soft = new TH1F("h_Pz_Soft",hist_title_label + "Soft Pz distribution",100,-20,20);
  TH1F *h_Pz_Hard = new TH1F("h_Pz_Hard",hist_title_label + "Hard Pz distribution",100,-20,20);
  TH1F *h_E_Soft = new TH1F("h_E_Soft",hist_title_label + "Soft Energy distribution",100,-0.5,500);
  TH1F *h_E_Hard = new TH1F("h_E_Hard",hist_title_label + "Hard Energy distribution",100,-0.5,500);
  TH1I *h_pdg_Soft = new TH1I("h_pdg_Soft",hist_title_label + "Geant soft particle code",100,-2300,2300);
  TH1I *h_pdg_Hard = new TH1I("h_pdg_Hard",hist_title_label + "Geant hard particle code",100,-2300,2300);
  TH1I *h_type_Soft = new TH1I("h_type_Soft",hist_title_label + "Origin of soft particle (type)",4,-0.5,1.5);
  TH1I *h_type_Hard = new TH1I("h_type_Hard",hist_title_label + "Origin of hard particle (type)",4,-0.5,1.5);
  TH1I *h_MotherIndex_Soft = new TH1I("h_MotherIndex_Soft",hist_title_label + "Index of the soft mother particle",4,-2,2);
  TH1I *h_MotherIndex_Hard = new TH1I("h_MotherIndex_Hard",hist_title_label + "Index of the hard mother particle",4,-2,2);
  TH1I *h_final_Soft = new TH1I("h_final_Soft",hist_title_label + "Final state (soft)",3,-1,2);
  TH1I *h_final_Hard = new TH1I("h_final_Hard",hist_title_label + "Final state (hard)",3,-1,2);

  Int_t nevents = input_tree->GetEntries();

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

		Bool_t select = false;
		if (particle_type == 0){
			select = MotherIndex[j] == -1 && final[j] == 1 && (pdg[j] == 211 || pdg[j] == -211 || pdg[j] == 321 || pdg[j] == -321 || pdg[j] == 2212 || pdg[j] == -2212);
		}
		else if (particle_type == 1){
			select = MotherIndex[j] == -1 && final[j] == 1 && (pdg[j] == 211 || pdg[j] == -211);
		}
		else if (particle_type == 2){
			select = MotherIndex[j] == -1 && final[j] == 1 && (pdg[j] == 321 || pdg[j] == -321);
		}
		else if (particle_type == 3){
			select = MotherIndex[j] == -1 && final[j] == 1 && (pdg[j] == 2212 || pdg[j] == -2212);
		}
		else{
			std::cout << "Número errado" << std::endl;
		}

		if (select == true){
       			h_Px->Fill(Px[j]);
			h_Py->Fill(Py[j]);
			h_Pz->Fill(Pz[j]);
			h_E->Fill(E[j]);
			h_pdg->Fill(pdg[j]);
			h_type->Fill(type[j]);
			h_MotherIndex->Fill(MotherIndex[j]);
 			h_final->Fill(final[j]);

			// my variables
		
			Float_t Pt = TMath::Hypot (Px[j],Py[j]);

			h_Pt->Fill(Pt);

			// 4 - vetor

			ROOT::Math::PtEtaPhiMVector fourvec_px_py_pz_e; //define uma variável no formato "ROOT::Math::PtEtaPhiMVector" que é um quadrivetor de Lorentz (4-vetor)

			fourvec_px_py_pz_e.SetPxPyPzE(Px[j],Py[j],Pz[j],E[j]); //assina os valores de Px,Py,Pz,E de uma dada partícula à variável "fourvec_px_py_pz_e"

			Double_t pt = fourvec_px_py_pz_e.Pt();

			h_pt->Fill(pt);			

			Double_t eta = fourvec_px_py_pz_e.Eta();
			
			h_eta->Fill(eta);

			Double_t phi = fourvec_px_py_pz_e.Phi();

			h_phi->Fill(phi);

			Double_t mass = fourvec_px_py_pz_e.M();

			h_mass->Fill(mass);

			//

			if (type[j] == 0){
				h_Px_Soft->Fill(Px[j]);
				h_Py_Soft->Fill(Py[j]);
				h_Pz_Soft->Fill(Pz[j]);
				h_E_Soft->Fill(E[j]);
				h_pdg_Soft->Fill(pdg[j]);
				h_type_Soft->Fill(type[j]);
				h_MotherIndex_Soft->Fill(MotherIndex[j]);
				h_final_Soft->Fill(final[j]);

				h_Pt_Soft->Fill(Pt);

				h_pt_Soft->Fill(pt);
				h_eta_Soft->Fill(eta);
				h_phi_Soft->Fill(phi);
				h_mass_Soft->Fill(mass);

			}else if (type[j] > 0){
				h_Px_Hard->Fill(Px[j]);
				h_Py_Hard->Fill(Py[j]);
				h_Pz_Hard->Fill(Pz[j]);
                        	h_E_Hard->Fill(E[j]);
                        	h_pdg_Hard->Fill(pdg[j]);
                        	h_type_Hard->Fill(type[j]);
                        	h_MotherIndex_Hard->Fill(MotherIndex[j]);
                        	h_final_Hard->Fill(final[j]);

				h_Pt_Hard->Fill(Pt);

				h_pt_Hard->Fill(pt);
				h_eta_Hard->Fill(eta);
                                h_phi_Hard->Fill(phi);
                                h_mass_Hard->Fill(mass);

			}else{}
		}else{}
	}
  }

  TCanvas *c_Bgen = new TCanvas("c_Bgen", "Canvas of Bgen distribution", 800, 400);
  c_Bgen->cd(0);
  h_Bgen->Draw();
  h_Bgen->GetXaxis()->SetTitle("Generated Impact Parameter");
  h_Bgen->GetYaxis()->SetTitle("Number of entries per Bin");
  h_Bgen->SetLineColor(2);
  c_Bgen->SaveAs("Figures/3in1/h_Bgen.pdf");

  TCanvas *c_Px = new TCanvas("c_Px", "Canvas of Px distribution ", 800, 400);
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

  TCanvas *c_pdg = new TCanvas("c_pdg", "Canvas of pdg distribution", 800, 400);
  c_pdg->cd(0);
  h_pdg->Draw(); 
  h_pdg->GetXaxis()->SetTitle("Geant particle code");
  h_pdg->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pdg->SetLineColor(2);
  c_pdg->SaveAs("Figures/h_pdg.pdf");

  TCanvas *c_type = new TCanvas("c_type", "Canvas of type distribution", 800, 400);
  c_type->cd(0);
  h_type->Draw();
  h_type->GetXaxis()->SetTitle("Origin of particle (=0 -> from soft hydro part, >0 -> from hard jet part)");
  h_type->GetYaxis()->SetTitle("Number of entries per Bin");
  h_type->SetLineColor(2);
  c_type->SaveAs("Figures/h_type.pdf");

  TCanvas *c_MotherIndex = new TCanvas("c_MotherIndex", "Canvas of MotherIndex distribution", 800, 400);
  c_MotherIndex->cd(0);
  h_MotherIndex->Draw();
  h_MotherIndex->GetXaxis()->SetTitle("Index of the mother particle (-1 if its a primary particle)");
  h_MotherIndex->GetYaxis()->SetTitle("Number of entries per Bin");
  h_MotherIndex->SetLineColor(2);
  c_MotherIndex->SaveAs("Figures/h_MotherIndex.pdf");

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

  // 4 - vector
  
  TCanvas *c_pt = new TCanvas("c_pt", "Canvas of pt distribution", 800, 400);
  c_pt->cd(0);
  h_pt->Draw();
  h_pt->GetXaxis()->SetTitle("pt [GeV/c]");
  h_pt->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pt->SetLineColor(2);
  c_pt->SaveAs("Figures/h_pt.pdf");

  TCanvas *c_eta = new TCanvas("c_eta", "Canvas of Eta distribution", 800, 400);
  c_eta->cd(0);
  h_eta->Draw();
  h_eta->GetXaxis()->SetTitle("Eta");
  h_eta->GetYaxis()->SetTitle("Number of entries per Bin");
  h_eta->SetLineColor(2);
  c_eta->SaveAs("Figures/h_eta.pdf");

  TCanvas *c_phi = new TCanvas("c_phi", "Canvas of Phi distribution", 800, 400);
  c_phi->cd(0);
  h_phi->Draw();
  h_phi->GetXaxis()->SetTitle("Phi");
  h_phi->GetYaxis()->SetTitle("Number of entries per Bin");
  h_phi->SetLineColor(2);
  c_phi->SaveAs("Figures/h_phi.pdf");

  TCanvas *c_mass = new TCanvas("c_mass", "Canvas of Mass distribution", 800, 400);
  c_mass->cd(0);
  h_mass->Draw();
  h_mass->GetXaxis()->SetTitle("Mass [GeV/c²]");
  h_mass->GetYaxis()->SetTitle("Number of entries per Bin");
  h_mass->SetLineColor(2);
  c_mass->SaveAs("Figures/h_mass.pdf");

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

  TCanvas *c_pdg_Soft = new TCanvas("c_pdg_Soft", "Canvas of pdg_Soft distribution", 800, 400);
  c_pdg_Soft->cd(0);
  h_pdg_Soft->Draw();
  h_pdg_Soft->GetXaxis()->SetTitle("Geant soft particle code");
  h_pdg_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pdg_Soft->SetLineColor(2);
  c_pdg_Soft->SaveAs("Figures/h_pdg_Soft.pdf");

  TCanvas *c_type_Soft = new TCanvas("c_type_Soft", "Canvas of type_Soft distribution", 800, 400);
  c_type_Soft->cd(0);
  h_type_Soft->Draw();
  h_type_Soft->GetXaxis()->SetTitle("Origin of soft particle");
  h_type_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_type_Soft->SetLineColor(2);
  c_type_Soft->SaveAs("Figures/h_type_Soft.pdf");

  TCanvas *c_MotherIndex_Soft = new TCanvas("c_MotherIndex_Soft", "Canvas of MotherIndex_Soft distribution", 800, 400);
  c_MotherIndex_Soft->cd(0);
  h_MotherIndex_Soft->Draw();
  h_MotherIndex_Soft->GetXaxis()->SetTitle("MotherIndex of the soft mother particle");
  h_MotherIndex_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_MotherIndex_Soft->SetLineColor(2);
  c_MotherIndex_Soft->SaveAs("Figures/h_MotherIndex_Soft.pdf");

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

  // 4 - vetor Soft

  TCanvas *c_pt_Soft = new TCanvas("c_pt_Soft", "Canvas of pt_Soft distribution", 800, 400);
  c_pt_Soft->cd(0);
  h_pt_Soft->Draw();
  h_pt_Soft->GetXaxis()->SetTitle("pt_Soft [GeV/c]");
  h_pt_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pt_Soft->SetLineColor(2);
  c_pt_Soft->SaveAs("Figures/h_pt_Soft.pdf");

  TCanvas *c_eta_Soft = new TCanvas("c_eta_Soft", "Canvas of Soft Eta distribution", 800, 400);
  c_eta_Soft->cd(0);
  h_eta_Soft->Draw();
  h_eta_Soft->GetXaxis()->SetTitle("Soft Eta");
  h_eta_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_eta_Soft->SetLineColor(2);
  c_eta_Soft->SaveAs("Figures/h_eta_Soft.pdf");

  TCanvas *c_phi_Soft = new TCanvas("c_phi_Soft", "Canvas of Soft Phi distribution", 800, 400);
  c_phi_Soft->cd(0);
  h_phi_Soft->Draw();
  h_phi_Soft->GetXaxis()->SetTitle("Soft Phi");
  h_phi_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_phi_Soft->SetLineColor(2);
  c_phi_Soft->SaveAs("Figures/h_phi_Soft.pdf");

  TCanvas *c_mass_Soft = new TCanvas("c_mass_Soft", "Canvas of Soft Mass distribution", 800, 400);
  c_mass_Soft->cd(0);
  h_mass_Soft->Draw();
  h_mass_Soft->GetXaxis()->SetTitle("Soft Mass [GeV/c²]");
  h_mass_Soft->GetYaxis()->SetTitle("Number of entries per Bin");
  h_mass_Soft->SetLineColor(2);
  c_mass_Soft->SaveAs("Figures/h_mass_Soft.pdf");

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

  TCanvas *c_pdg_Hard = new TCanvas("c_pdg_Hard", "Canvas of pdg_Hard distribution", 800, 400);
  c_pdg_Hard->cd(0);
  h_pdg_Hard->Draw();
  h_pdg_Hard->GetXaxis()->SetTitle("Geant hard particle code");
  h_pdg_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pdg_Hard->SetLineColor(2);
  c_pdg_Hard->SaveAs("Figures/h_pdg_Hard.pdf");

  TCanvas *c_type_Hard = new TCanvas("c_type_Hard", "Canvas of type_Hard distribution", 800, 400);
  c_type_Hard->cd(0);
  h_type_Hard->Draw();
  h_type_Hard->GetXaxis()->SetTitle("Origin of hard particle");
  h_type_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_type_Hard->SetLineColor(2);
  c_type_Hard->SaveAs("Figures/h_type_Hard.pdf");

  TCanvas *c_MotherIndex_Hard = new TCanvas("c_MotherIndex_Hard", "Canvas of MotherIndex_Hard distribution", 800, 400);
  c_MotherIndex_Hard->cd(0);
  h_MotherIndex_Hard->Draw();
  h_MotherIndex_Hard->GetXaxis()->SetTitle("MotherIndex of the hard mother particle");
  h_MotherIndex_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_MotherIndex_Hard->SetLineColor(2);
  c_MotherIndex_Hard->SaveAs("Figures/h_MotherIndex_Hard.pdf");

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

  // 4 - vetor Hard

  TCanvas *c_pt_Hard = new TCanvas("c_pt_Hard", "Canvas of pt_Hard distribution", 800, 400);
  c_pt_Hard->cd(0);
  h_pt_Hard->Draw();
  h_pt_Hard->GetXaxis()->SetTitle("pt_Hard [GeV/c]");
  h_pt_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pt_Hard->SetLineColor(2);
  c_pt_Hard->SaveAs("Figures/h_pt_Hard.pdf");

  TCanvas *c_eta_Hard = new TCanvas("c_eta_Hard", "Canvas of Hard Eta distribution", 800, 400);
  c_eta_Hard->cd(0);
  h_eta_Hard->Draw();
  h_eta_Hard->GetXaxis()->SetTitle("Hard Eta");
  h_eta_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_eta_Hard->SetLineColor(2);
  c_eta_Hard->SaveAs("Figures/h_eta_Hard.pdf");

  TCanvas *c_phi_Hard = new TCanvas("c_phi_Hard", "Canvas of Hard Phi distribution", 800, 400);
  c_phi_Hard->cd(0);
  h_phi_Hard->Draw();
  h_phi_Hard->GetXaxis()->SetTitle("Hard Phi");
  h_phi_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_phi_Hard->SetLineColor(2);
  c_phi_Hard->SaveAs("Figures/h_phi_Hard.pdf");

  TCanvas *c_mass_Hard = new TCanvas("c_mass_Hard", "Canvas of Hard Mass distribution", 800, 400);
  c_mass_Hard->cd(0);
  h_mass_Hard->Draw();
  h_mass_Hard->GetXaxis()->SetTitle("Hard Mass [GeV/c²]");
  h_mass_Hard->GetYaxis()->SetTitle("Number of entries per Bin");
  h_mass_Hard->SetLineColor(2);
  c_mass_Hard->SaveAs("Figures/h_mass_Hard.pdf");

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
  c_Px_All->SaveAs("Figures/3in1/Filtros/h_Px_All.pdf");

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
  c_Py_All->SaveAs("Figures/3in1/Filtros/h_Py_All.pdf");

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
  c_Pz_All->SaveAs("Figures/3in1/Filtros/h_Pz_All.pdf");

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
  c_E_All->SaveAs("Figures/3in1/Filtros/h_E_All.pdf");

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
  c_pdg_All->SaveAs("Figures/3in1/Filtros/h_pdg_All.pdf");

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
  c_type_All->SaveAs("Figures/3in1/Filtros/h_type_All.pdf");

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
  c_MotherIndex_All->SaveAs("Figures/3in1/Filtros/h_MotherIndex_All.pdf");

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
  c_final_All->SaveAs("Figures/3in1/Filtros/h_final_All.pdf");

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
  c_Pt_All->SaveAs("Figures/3in1/Filtros/h_Pt_All.pdf");

  // 4 - vetor All

  TCanvas *c_pt_All = new TCanvas("c_pt_All", "Canvas of pt_All distribution", 800, 400);
  c_pt_All->cd(0);
  TH1F *h_pt_clone = (TH1F *)h_pt->Clone("h_pt_Total");
  h_pt_clone->Draw();
  h_pt_clone->GetXaxis()->SetTitle("pt [GeV/c]");
  h_pt_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_pt_clone->SetLineColor(1);
  h_pt_clone->SetLineWidth(3);
  TH1F *h_pt_Soft_clone = (TH1F *)h_pt_Soft->Clone("h_pt_Soft");
  h_pt_Soft_clone->Draw("same");
  h_pt_Soft_clone->SetLineColor(4);
  h_pt_Soft_clone->SetLineWidth(2);
  TH1F *h_pt_Hard_clone = (TH1F *)h_pt_Hard->Clone("h_pt_Hard");
  h_pt_Hard_clone->Draw("same");
  h_pt_Hard_clone->SetLineColor(2);
  h_pt_Hard_clone->SetLineWidth(1);
  c_pt_All->SaveAs("Figures/3in1/Filtros/h_pt_All.pdf");

  TCanvas *c_eta_All = new TCanvas("c_eta_All", "Canvas of eta_All distribution", 800, 400);
  c_eta_All->cd(0);
  TH1F *h_eta_clone = (TH1F *)h_eta->Clone("h_eta_Total");
  h_eta_clone->Draw();
  h_eta_clone->GetXaxis()->SetTitle("Eta");
  h_eta_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_eta_clone->SetLineColor(1);
  h_eta_clone->SetLineWidth(3);
  TH1F *h_eta_Soft_clone = (TH1F *)h_eta_Soft->Clone("h_eta_Soft");
  h_eta_Soft_clone->Draw("same");
  h_eta_Soft_clone->SetLineColor(4);
  h_eta_Soft_clone->SetLineWidth(2);
  TH1F *h_eta_Hard_clone = (TH1F *)h_eta_Hard->Clone("h_eta_Hard");
  h_eta_Hard_clone->Draw("same");
  h_eta_Hard_clone->SetLineColor(2);
  h_eta_Hard_clone->SetLineWidth(1);
  c_eta_All->SaveAs("Figures/3in1/Filtros/h_eta_All.pdf");

  TCanvas *c_phi_All = new TCanvas("c_phi_All", "Canvas of phi_All distribution", 800, 400);
  c_phi_All->cd(0);
  TH1F *h_phi_clone = (TH1F *)h_phi->Clone("h_phi_Total");
  h_phi_clone->Draw();
  h_phi_clone->GetXaxis()->SetTitle("Phi");
  h_phi_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_phi_clone->SetLineColor(1);
  h_phi_clone->SetLineWidth(3);
  TH1F *h_phi_Soft_clone = (TH1F *)h_phi_Soft->Clone("h_phi_Soft");
  h_phi_Soft_clone->Draw("same");
  h_phi_Soft_clone->SetLineColor(4);
  h_phi_Soft_clone->SetLineWidth(2);
  TH1F *h_phi_Hard_clone = (TH1F *)h_phi_Hard->Clone("h_phi_Hard");
  h_phi_Hard_clone->Draw("same");
  h_phi_Hard_clone->SetLineColor(2);
  h_phi_Hard_clone->SetLineWidth(1);
  c_phi_All->SaveAs("Figures/3in1/Filtros/h_phi_All.pdf");

  TCanvas *c_mass_All = new TCanvas("c_mass_All", "Canvas of mass_All distribution", 800, 400);
  c_mass_All->cd(0);
  TH1F *h_mass_clone = (TH1F *)h_mass->Clone("h_mass_Total");
  h_mass_clone->Draw();
  h_mass_clone->GetXaxis()->SetTitle("Mass [GeV/c²]");
  h_mass_clone->GetYaxis()->SetTitle("Number of entries per Bin");
  h_mass_clone->SetLineColor(1);
  h_mass_clone->SetLineWidth(3);
  TH1F *h_mass_Soft_clone = (TH1F *)h_mass_Soft->Clone("h_mass_Soft");
  h_mass_Soft_clone->Draw("same");
  h_mass_Soft_clone->SetLineColor(4);
  h_mass_Soft_clone->SetLineWidth(2);
  TH1F *h_mass_Hard_clone = (TH1F *)h_mass_Hard->Clone("h_mass_Hard");
  h_mass_Hard_clone->Draw("same");
  h_mass_Hard_clone->SetLineColor(2);
  h_mass_Hard_clone->SetLineWidth(1);
  c_mass_All->SaveAs("Figures/3in1/Filtros/h_mass_All.pdf");

}
