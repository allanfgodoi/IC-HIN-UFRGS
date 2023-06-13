#include "TF1.h"
#include "TRandom.h"
#include <random>
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <thread>
#include <mutex>

using namespace std;

// Const values used in the simulations
const double
        pi         = TMath::Pi(),
        rho0       = 3,                 //fm^-3
        r0         = 6.62,              //fm
        a          = 0.542,             //fm
        secIn      = 6.5,                //fm^2
        radiusSq   = (secIn) / pi; //fm^2

mutex mtx;

// Structures used to generate and save data.
struct Nucleon {double x, y;};
struct Data {
    int nCol;
    int nPart;
    double dist;
};

// Main code
void collide (const string& filename = "./data.root", int nucleons = 208,
int simulations = 1'000'000, const ::int64_t nThreads =std::thread::hardware_concurrency()){


    // Configure the program to export in a Root file
    Data data{};
    auto *file = new TFile(filename.c_str(),"recreate");
    auto *save = new TTree("Data", "Simulation");
    save->Branch("Collisions", &data, "NColl/I:NPart/I:Par/D");

    // Set up the functions to be used in the generators
    auto *distF = new TF1("distF", "x*2*pi", 0, 18);
    auto *posF = new TF1("posF",  "(x*x*[0]/(1+exp((x-[1])/[2])))", 0, 14);
    posF->SetParameters(rho0, r0, a);

    // Config RNG
    mt19937 rng(std::random_device{}());
    uniform_real_distribution<> dis(0.0, pi);
    TRandom random;
    random.SetSeed();

    // Configure multithreading
    ROOT::EnableImplicitMT();
    ROOT::EnableThreadSafety();
    vector<thread> threads;
    for (int k = 0; k < nThreads; k++) {
        threads.emplace_back([&, k] {

            // Run the simulations
            for (int p = k; p < simulations;) {
                unordered_set<int> nucleonPartTemp;

                // Create the array of Nucleon to represent the Nuclei.
                Nucleon frst[nucleons];
                Nucleon scnd[nucleons];
                int colT = 0;
                int partT = 0;
                double distT;

                // Generate collision data
                distT = distF->GetRandom();
                for (int i = 0; i < nucleons; i++) {

                    double position = posF->GetRandom();
                    double theta = dis(rng);
                    double phi =   dis(rng) * 2;
                    frst[i].x = position * sin(theta) * cos(phi);
                    frst[i].y = position * sin(theta) * sin(phi);


                    position = posF->GetRandom();
                    theta = dis(rng);
                    phi =   dis(rng) * 2;
                    scnd[i].x = position * sin(theta) * cos(phi) + distT;
                    scnd[i].y = position * sin(theta) * sin(phi);
                }


                // Verify collision partTemp and number
                for (int i = 0; i < nucleons; i++) {
                    bool passTrough = false;
                    for (int j = 0; j < nucleons; j++) {

                        double deltaX=frst[i].x-scnd[j].x;
                        double deltaY=frst[i].y-scnd[j].y;

                        if ((deltaX*deltaX + deltaY*deltaY) < radiusSq) {
                            colT++;
                            passTrough = true;
                            if (nucleonPartTemp.insert(j).second) {
                                partT++;
                            }
                        }
                    }
                    if (passTrough) {
                        partT++;
                    }
                }

                // Write the data to the root file
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