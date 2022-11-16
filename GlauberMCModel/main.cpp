#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TRandom.h"
#include <random>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <TROOT.h>

using namespace std;

const double pi = TMath::Pi();

double calcProb(double *x, double *par){
    return (x[0] * par[0] / (1 + exp((x[0] - par[1])/par[2])))/309.310706654;
}
double calcD(double *x, double *par){
    return x[0]*2*pi;
}

int main (){

    ROOT::EnableImplicitMT();

    // Config RNG
    ofstream data("data.txt");
    auto *random = new TRandom();
    random->SetSeed();

    // Simulation variables
    const double rho0     = 3,
                 r0     = 6.62,
                 a      = 0.542,
                 secIn   = 65;

    const int n = 208;
    const double radius = sqrt((secIn / 10) / pi);
          double xFirst[n], yFirst[n],
                 xSecond[n], ySecond[n];

    for (int p = 0; p < 1'000'000; ) {
        int nCol = 0,
            secPart = 0,
            firstPart = 0;

        vector<double> xSecondPartTemp, ySecondPartTemp;

        // Set up the functions to be used in the generators
        auto *dist = new TF1("dist", calcD, 0, 20, 0);
        auto *pos = new TF1("pos", calcProb, 0, 14, 3);
        pos->SetParameters(rho0, r0, a);

        // Generate collision data
        double nucleiDistance = dist->GetRandom();
        for (int i = 0; i < n; i++) {
            double position = pos->GetRandom();
            double var = random->Rndm() * pi * 2;
            xFirst[i] = position * sin(var);
            yFirst[i] = position * cos(var);
            position = pos->GetRandom();
            var = random->Rndm() * pi * 2;
            xSecond[i] = position * sin(var) + nucleiDistance;
            ySecond[i] = position * cos(var);
        }

        // Verify collision part and number
        for (int i = 0; i < n; i++) {
            bool passTrough = false;
            for (int j = 0; j < n; j++) {
                if ((sqrt(pow((xFirst[i] - xSecond[j]), 2) + pow((yFirst[i] - ySecond[j]), 2))) < radius){
                    nCol++;
                    int position = 0;
                    bool done = true;
                    for (double k : xSecondPartTemp) {
                        if (k==xSecond[j])
                            if (ySecondPartTemp[position] == ySecond[j]){
                                done = false;
                                break;
                            }
                        position++;
                    }
                    if (done) {
                        xSecondPartTemp.push_back(xSecond[j]);
                        ySecondPartTemp.push_back(ySecond[j]);
                        secPart++;
                    }
                    passTrough = true;
                }
            }
            if (passTrough) {
                firstPart++;
            }
        }
        if (firstPart + secPart > 0) {
            data << (firstPart + secPart) << " " << nCol << " " << nucleiDistance << endl;
            p++;
        }
    }
    data.close();
    return 0;
}
