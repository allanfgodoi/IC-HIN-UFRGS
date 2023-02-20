#include "TF1.h"
#include "TRandom.h"
#include <random>
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <thread>

using namespace std;

// Simulation variables
const double
        pi              = TMath::Pi(),
        rho0            = 3,
        r0              = 6.62,
        a               = 0.542,
        secIn           = 65,
        radiusSquared   = (secIn / 10) / pi;

const ::int64_t nThreads = std::thread::hardware_concurrency();
mutex mtx;

// Structures used to generate and save data.
struct Nucleon {double x, y;};
struct Data {int nCol, nPart; double dist;};

void collide (const string& filename = "./data.root", int nucleons = 208, int simulations = 1'000'000){

    // Configure multithreading
    ROOT::EnableImplicitMT();
    ROOT::EnableThreadSafety();

    // Configure the program to export in a Root file
    Data data{};
    auto *file = new TFile(filename.c_str(),"recreate");
    auto *save = new TTree("save", "Simulation");
    save->Branch("Collisions", &data, "col/I:part/I:dist/D");

    vector<thread> threads;

    // Set up the functions to be used in the generators
    auto *distF = new TF1("distF", "x*2*pi", 0, 18);
    auto *posF = new TF1("posF", "(x*[0]/(1+exp((x-[1])/[2])))/309.310706654", 0, 14);
    posF->SetParameters(rho0, r0, a);

    // Config RNG
    mt19937 rng(random_device{}());
    uniform_real_distribution<> dis(0.0, 1.0);
    TRandom random;
    random.SetSeed();

    for (int k = 0; k < nThreads; k++) {
        threads.emplace_back([&, k] {

            // Run the simulations
            for (int p = k; p < simulations;) {
                unordered_set<int> nucleonPartTemp;

                // Create the array of Nucleon to represent the Nuclei.
                Nucleon first[nucleons];
                Nucleon second[nucleons];
                int colT = 0;
                int partT = 0;
                double distT;

                // Generate collision data
                distT = distF->GetRandom();
                for (int i = 0; i < nucleons; i++) {
                    double position = posF->GetRandom();
                    double var = dis(rng) * pi * 2;
                    first[i].x = position * sin(var);
                    first[i].y = position * cos(var);
                    position = posF->GetRandom();
                    var = dis(rng) * pi * 2;
                    second[i].x = position * sin(var) + distT;
                    second[i].y = position * cos(var);
                }


                // Verify collision partTemp and number
                for (int i = 0; i < nucleons; i++) {
                    bool passTrough = false;
                    for (int j = 0; j < nucleons; j++) {
                        if ((pow((first[i].x - second[j].x), 2) +
                             pow((first[i].y - second[j].y), 2)) < radiusSquared) {
                            colT++;
                            passTrough = true;
                            if (nucleonPartTemp.insert(j).second) {
                                partT++;
                            }
                        }
                    }
                    if (passTrough) { partT++; }
                }
                if (partT > 0) {
                    mtx.lock();
                    data.nCol = colT;
                    data.nPart = partT;
                    data.dist = distT;
                    save->Fill();
                    mtx.unlock();
                    p+= (int) nThreads;
                }
            }
        });
    }
    for (auto &t : threads) t.join();
    save->Write();
    file->Close();
}

int main () {
    collide();
}