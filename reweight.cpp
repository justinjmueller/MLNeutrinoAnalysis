#include <iostream>
#include <vector>

#include "dataset.h"
#include "optimize.h"

#include "TFile.h"
#include "TTree.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnUserParameterState.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnMinos.h"
#include "Minuit2/MnScan.h"

int main()
{
    Dataset data("../csv/", "analysis");

    Optimize op(data);
    ROOT::Minuit2::MnUserParameters upar;
    upar.Add("sm_primary", 1.00, 0.05);
    upar.Add("sm_nonprimary", 0.93, 0.05);
    upar.Add("sm_photon", 1.00, 0.05);
    upar.Add("sm_electron", 1.00, 0.01);
    upar.Add("sm_muon", 4.00, 0.050);
    upar.Add("sm_pion", 1.00, 0.05);
    upar.Add("sm_proton", 0.30, 0.005);

    std::cout << op({1.000, 0.933, 1.000, 1.000, 4.000, 1.000, 0.300}) << std::endl;

    ROOT::Minuit2::MnMigrad migrad = ROOT::Minuit2::MnMigrad(op, upar);
    migrad.Fix("sm_primary");
    migrad.Fix("sm_photon");
    migrad.Fix("sm_electron");
    migrad.Fix("sm_pion");
    ROOT::Minuit2::FunctionMinimum min = migrad();
    std::cout << min << std::endl;

    ROOT::Minuit2::MnScan scan(op, upar);
    std::vector<std::pair<double, double>> sres = scan.Scan(4);
    for(std::pair<double, double> v : sres)
    {
        std::cout << "(" << v.first << ", " << v.second << ")" << std::endl;
    }

    /*ROOT::Minuit2::MnMinos minos(op, min);
    std::pair<double, double> tmp_par;
    for(uint16_t p(0); p < 7; ++p)
    {
        tmp_par = minos(p);
        std::cout << "Parameter " << p << ": (" << tmp_par.first << ", " << tmp_par.second << ")" << std::endl;
    }*/
}