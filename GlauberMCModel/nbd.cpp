#include <TGraph2D.h>
#include <TFile.h>
#include <TLeaf.h>
#include <TTree.h>
#include <TH2F.h>
#include <TF1.h>
#include <TSystem.h>
#include "TLegend.h"
#include <TCanvas.h>
#include <TStyle.h>


using namespace std;

// Structures used to generate and save data.
struct Data {int nCol, nPart; double d;};

void nbd(const char* dataFile = "./data.root",
         const char* compareFile = "./HFSumEt.root"){

    double hScale = 1./1000.;
    const int scanSize = 5;
    const int nDim = 10;

    double kMin = 0.3;
    double kMax = 0.7;
    double kDelta = (kMax - kMin) / (scanSize - 1);

    double muMin = 1.0;
    double muMax = 1.8;
    double muDelta = (muMax - muMin) / (scanSize - 1);

    double nMax = 2.5e-2;
    double nMin = 1.5e-2;
    double nDelta = (nMax - nMin) / (nDim - 1);

    TF1 *NBD = new TF1("NBD","TMath::Gamma(x+[1])/(TMath::Gamma(x+1)*TMath::Gamma([1])) * (([0]/[1])**x)/(([0]/[1]+1)**(x+[1]))",0,28);
    TH2 *results = new TH2F("chiRes", "chiRes",scanSize, muMin-muDelta/2, muMax+muDelta/2,
                            scanSize, kMin-kDelta/2, kMax+kDelta/2);

    // Read experimental values
    auto *expFile = new TFile(compareFile);
    TH1F *exp = (TH1F*)(expFile->Get("hfSum"));

    // Read simulated values
    auto *file = new TFile(dataFile);
    auto *tree = (TTree*)(file->Get("Data"));
    Data data{};
    tree->SetBranchAddress("Collisions", &data);
    vector<int> nCol;
    for (int i = 0; i < tree->GetEntries(); i++){
        tree->GetEntry(i);
        nCol.emplace_back(data.nCol);
    }

    // Create graphing variables
    double bestK, bestMu;
    double chi2best = INFINITY;

    // Run through the Mu values
    for (int iMu = 0; iMu < scanSize; iMu++) {
        double mu = muMin + muDelta * iMu;

        // Run through the K values
        for (int ik = 0; ik < scanSize; ik++) {
            double localChi2best = INFINITY;
            double k = kMin + kDelta * ik;

            NBD->SetParameters(mu, k);
            TH1F *generated = new TH1F("Generated", "sumETf vs events", 100, 0, 5);

            // Generate the random data
            for (int col : nCol) {
                double sumET = 0;
                for (int n = 0; n < col; n++) {
                    sumET += NBD->GetRandom();
                }
                generated->Fill(sumET * hScale);
            }

            // Calculate the Chi^2 ofr the given Mu and K
            // through a range of normalization values
            double chi2min;
            double n;
            for (int iNorm = 0; iNorm < nDim; iNorm++) {
                generated->Scale(nMin + nDelta * iNorm);
                chi2min = 0;
                double Observed, Expected, diff;
                for (int i = 20; i < 100; i++) {
                    Expected = (double)exp->GetBinContent(i);
                    Observed = (double)generated->GetBinContent(i);
                    diff = Observed - Expected;
                    chi2min += (diff*diff)/Expected;
                }
                if (localChi2best > chi2min) {
                    localChi2best = chi2min;
                    n = nMin + nDelta * iNorm;
                }
                generated->Scale(1/(nMin + nDelta * iNorm));
            }

            // Output the resulting values in the terminal
            results->SetBinContent(iMu + 1, ik + 1, localChi2best);
            cout
                    << "Chi2 = " << localChi2best
                    << " MU = " << mu
                    << " K = " << k
                    << " N = " << n << endl;

            // Saves the best K and Mu values
            if (localChi2best < chi2best) {bestK = k; bestMu = mu;}
            localChi2best = INFINITY;
            delete generated;
        }
    }

    // Config the canvas
    auto *c = new TCanvas("canvas", "canvas", 1000, 743);
    c->SetLogy();
    c->SetLogz();
    gStyle->SetPalette(kBird);
    gStyle->SetOptStat(0);

    // Personalize resulting Chi2h histogram and draw it
    results->GetYaxis()->SetTitle("k");
    results->GetXaxis()->SetTitle("#mu");
    results->GetYaxis()->SetNdivisions(2*scanSize+1,0,0);
    results->GetXaxis()->SetNdivisions(2*scanSize+1,0,0);
    results->SetTitle("");
    results->DrawClone("colz");
    results->Draw("same text");
    c->SaveAs("ue.png");


    // Generate a new histogram with the best variable's values
    NBD->SetParameters(bestMu, bestK);
    TH1F *generated = new TH1F("Generated", "Energia gerada;#Sigma E_{T}", 100, 0, 5);
    for (int col : nCol) {
        double sumET = 0;
        for (int n = 0; n < col; n++) {
            sumET += NBD->GetRandom();
        }
        generated->Fill(sumET * hScale);
    }

    // Personalize the generated with the experimental histograms and draw them
    generated->Scale(0.021);
    generated->SetLineColor(kGreen);
    generated->Draw("HIST");
    exp->Draw("same E0");
    auto* fitLeg = new TLegend(0.60,0.80,0.85,0.88);
    fitLeg->SetTextSize(0.025);
    fitLeg->AddEntry(generated, "Simulacao");
    fitLeg->AddEntry(exp, "Dados experimentais");
    fitLeg->Draw();
    c->SaveAs("results.png");   

}
