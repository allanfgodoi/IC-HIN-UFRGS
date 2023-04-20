#include "TF1.h"
#include "TH1F.h"
#include <TFile.h>
#include <TLeaf.h>
#include <TTree.h>
#include <cmath>
#include <TH2F.h>
#include <Math/IntegratorOptions.h>
#include <thread>

using namespace std;

// Structures used to generate and save data.
struct Data {int nCol, nPart; double dist;};

//Negative binomial distribution function
double NBD(double *x, double *par){
    return (pow(par[1]/par[0], x[0]) / pow(1 + par[1]/par[0], par[0] + x[0]))
           * (tgamma(par[0] + x[0])          / (tgamma(par[0] + 1) * tgamma(x[0])));
}
const double nValue = 0.000938889;

void negativeBinomialDist (const char* dataFile = "./data.root", const char*
compareFile = "./HFSumEt.root", const char* parameterGraph = "./fit.root"){
    double minK = 0.2, maxK = 1, minM = 0.5, maxM = 2, nRuns = 20;

    // Read experimental values
    auto *expFile = new TFile(compareFile);
    auto *exp = dynamic_cast<TH1F*>(expFile->Get("hfSum"));
    exp->SetBins(100, 0.5, 5);

    // Read generated data
    auto *file = new TFile(dataFile);
    auto *tree = dynamic_cast<TTree *>( file->Get("save"));
    Data data{};
    tree->SetBranchAddress("Collisions", &data);


    // Config where to save the data
    vector<int> nCol;
    for (int i = 0; i < tree->GetEntries(); i++){
        tree->GetEntry(i);
        nCol.emplace_back(data.nCol);
    }

    // Setup NBD
    int  bestPos = 0;
    auto parameter = new TH2F("Par", "Par", 50, 0, 2, 50, 0, 3);
    for (int run = 0; run < nRuns; run++) {
        vector<thread> threads(9);

        // Setup starting parameter values
        double deltaK = (maxK - minK) / 3;
        double deltaM = (maxM - minM) / 3;
        double bestChi = INFINITY;
        double chi[9];

        // Run each simulation in a different thread
        for (int i = 0; i < 9; i++) {
            threads[i] = thread([&, i]{
                string const TFName = "sumET" + to_string(i);
                string const THName = "sumETC" + to_string(i);
                TF1 *sumETf = new TF1(TFName.c_str(), NBD, 0, 3, 2);
                TH1F *generated = new TH1F(THName.c_str(), "sumETf vs events", 100, 0.5, 5);
                sumETf->SetParameters(minK + deltaK * (floor(i / 3)), minM + deltaM *(i % 3));
                for (int col : nCol) {
                    double sumET = 0;
                    for (int j = 0; j < col; j++) {
                        sumET += sumETf->GetRandom();
                    }
                    generated->Fill(sumET * nValue);
                }
                chi[i] = exp->Chi2Test(generated, "WW CHI2");
            });
        }
        for (auto &t : threads) t.join();
        for (int i = 0; i < 9; i++) {
            if (chi[i] < bestChi && chi[i] != 0) {
                bestPos = i;
                bestChi = chi[i];
                parameter->Fill(minM + deltaM *(i % 3), minK + deltaK * (floor(i / 3)));
            }
        }
        minK = minK + deltaK * (floor(bestPos / 3)) - deltaK/2;
        maxK = minK + deltaK * (floor(bestPos / 3)) + deltaK/2;
        minM = minM + deltaM * (bestPos % 3) - deltaM/2;
        maxM = minM + deltaM * (bestPos % 3) + deltaM/2;

        cout << bestChi << ", " << minM + deltaM / 2 << ", " << minK + deltaK / 2 << endl;
    }
    parameter->SaveAs(parameterGraph);
}