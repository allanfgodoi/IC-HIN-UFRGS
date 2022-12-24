#include "TMath.h"
using namespace TMath;

double wsFunction(double *x, double *par){
    return  (TMath::Gamma(par[1] + x[0]) / (TMath::Gamma(x[0] + 1) * TMath::Gamma(par[1])))
    * ((TMath::Power((par[2] / par[1]),x[0])) * (TMath::Power(( 1 + (par[2] / par[1])),x[0] + par[1])));
}
