#include <Math/PdfFuncMathCore.h>
#include <TGraph2D.h>
#include <TFile.h>
#include <TLeaf.h>
#include <TTree.h>
#include <thread>
#include <TH2F.h>
#include <TF1.h>
#include <cmath>
#include <TSystem.h>

using namespace std;

// Structures used to generate and save data.
struct Data {int nCol, nPart; double dist;};
mutex mtx;

// Negative binomial distribution function as defined in Root/MathPdfFuncMathCore
double dist(const double *x, const double *par){
    double k = par[0];
    double p = (par[1] / k) / (1.0 + par[1] / k);
    double n = x[0];
    
    double d = ROOT::Math::lgamma(k + n) - ROOT::Math::lgamma(k + 1.0) - ROOT::Math::lgamma(n);
    return std::exp(d + n * std::log(p) + double(k) * ROOT::Math::log1p(-p));
}

int value (double x){
    return floor(3 * ( x/3 - floor(x/3)));
}

// Main code
void nbd (int         nRuns = 20,
          const char* dataFile = "./data.root",
          const char* compareFile = "./HFSumEt.root",
          const char* location = "./hist/"){

    gSystem->mkdir(location);

    // Setup fitting speed, the higher the number, the faster it goes. MUST be bigger than 1
    const double speedM = 4.0 / 3.0, speedK = 4.0 / 3.0, speedN = 4.0;

    // Setup fitting initial intervals
    double  maxK = 1,
            minK = 0.2,
            
            maxM = 2,
            minM = 0.5,
            
            maxN = 0.005,
            minN = 0.0005;

    // Read experimental values
    auto *expFile = new TFile(compareFile);
    auto *expTemp = (TH1F*)(expFile->Get("hfSum"));
    auto *exp = new TH1F("experimental", "sumETf vs events", 90, 0.5, 5);
    for (int i = expTemp->FindFirstBinAbove(0.5); i <= 100 ; i++){
        for (int j = 0; j < expTemp->GetBinContent(i); ++j) {
            exp->Fill(expTemp->GetBinCenter(i));
        }
    }

    // Read finalHist data
    auto *file = new TFile(dataFile);
    auto *tree = (TTree*)(file->Get("Data"));
    Data data{};
    tree->SetBranchAddress("Collisions", &data);

    // Config where to save the data
    vector<int> nCol;
    for (int i = 0; i < tree->GetEntries(); i++){
        tree->GetEntry(i);
        nCol.emplace_back(data.nCol);
    }

    // Setup Distribution
    int  bestRelativePos[3] = {0,0,0};
    auto parameter = new TGraph2D();
    parameter->SetTitle("par; Mu; K; N");

    // Run fitting algorithm
    for (int run = 0; run < nRuns; run++) {
        gSystem->RedirectOutput((location + string("log.txt")).c_str(), "a");
        double bestRelativeChi[3] = {INFINITY,INFINITY,INFINITY};
        double chi[9];

        double deltaN = (maxN - minN) / 2;
        double deltaK = (maxK - minK) / 2;
        double deltaM = (maxM - minM) / 2;

        int bestN = 0;

        for (int i = 0; i < 3; i++){
            vector<thread> threads;

            // Setup starting parameter values
            for (int j = 0; j < 9; j++) {
                parameter->SetPoint((9 * i + j + 27 * run),
                                    minM + deltaM * value(j/3.0),
                                    minK + deltaK * value(j),
                                    minN + deltaN * value(i));
            }

            // Run each simulation in a different thread
            for (int j = 0; j < 9; j++) {
                vector<int> nColTemp(nCol);
                threads.emplace_back([&, j]{

                    // Set parameters values
                    double k  = minK + deltaK * value(j);
                    double mu = minM + deltaM * value(j/3.0);

                    // Name root objects for each run
                    string const TFName = "sumET" + to_string(j);
                    string const THName = "sumETC" + to_string(j);
                    TF1 *sumETf = new TF1(TFName.c_str(), dist, 0, 18, 2);
                    TH1F *generated = new TH1F(THName.c_str(), "sumETf vs events", 90,
                                               0.5, 5);

                    // Setup RNG functions parameters
                    sumETf->SetParameter(0, k);
                    sumETf->SetParameter(1, mu);

                    // Generate the random data
                    for (int col : nCol) {
                        double sumET = 0;
                        for (int n = 0; n < col; n++) {
                            sumET += sumETf->GetRandom();
                        }
                        generated->Fill(sumET * (minN + deltaN * value(i)));
                    }

                    // Save values
                    chi[j] = exp->Chi2Test(generated, "UU CHI2");
                    string save = string(location) + "gen_" + to_string(run) + "." + to_string(i) +
                            "." + to_string(j) + ".root";
                    mtx.lock();
                    generated->SaveAs(save.c_str());
                    sumETf->Delete();
                    generated->Delete();
                    mtx.unlock();
                });
            }

            for (auto &t : threads) t.join();
            for (int j = 0; j < 9; j++)
                if (chi[j] < bestRelativeChi[i] and chi[j] != 0) {
                    bestRelativePos[i] = j;
                    bestRelativeChi[i] = chi[j];
                }

        }

        double bestPos = 0;
        double bestChi = INFINITY;
        for (int i = 0; i < 3; ++i)
            if (bestRelativeChi[i] < bestChi and bestRelativeChi[i] != 0){
                bestPos = bestRelativePos[i];
                bestChi = bestRelativeChi[i];
                bestN = i;
            }

        gSystem->RedirectOutput((location + string ("results.txt")).c_str(), "a");
        cout << "Gen = " << to_string(run) + "." + to_string(bestN) << "." + to_string((int)bestPos)
             << "\n"
             << "Chi2 = " << bestChi
             << "; Mu = " <<minM + deltaM * value(bestPos/3.0)
             << "; K = "  <<minK + deltaK * value(bestPos)
             << "; N =  " <<minN + deltaN * (bestN)
             << "\n";

        maxK = minK + deltaK * value(bestPos) + deltaK / speedK;
        minK = minK + deltaK * value(bestPos) - deltaK / speedK;
        maxM = minM + deltaM * value(bestPos/3.0) + deltaM / speedM;
        minM = minM + deltaM * value(bestPos/3.0) - deltaM / speedM;
        maxN = minN + deltaN * (bestN) + deltaN / speedN;
        minN = minN + deltaN * (bestN) - deltaN / speedN;
    }
    parameter->SaveAs((location + string("fit.root")).c_str());

    TH1F *finalHist = new TH1F("finalHist", "sumETf vs events", 90, 0, 5);
    TF1 *sumETf = new TF1("finalFunc", dist, 0, 18, 2);
    sumETf->SetParameter(1, (minM + maxM) / 2.);
    sumETf->SetParameter(0, (minK + maxK) / 2.);

    for (int col : nCol) {
        double sumET = 0;
        for (int n = 0; n < col; n++) {
            sumET += sumETf->GetRandom();
        }
        finalHist->Fill(sumET * ((minN + maxN)/2));
    }
    finalHist->SaveAs((location + string("final.root")).c_str());
}