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
#include "Minuit2/MnContours.h"
#include "Minuit2/MnPlot.h"

int main()
{
    Dataset data("../csv/", "analysis");
    //data.read_dataset();

    Optimize op(data);
    ROOT::Minuit2::MnUserParameters upar;
    upar.Add("sm_nonprimary", 0.50, 0.05);
    //upar.SetValue("sm_nonprimary", 0.50);
    ROOT::Minuit2::MnMigrad migrad = ROOT::Minuit2::MnMigrad(op, upar);
    ROOT::Minuit2::FunctionMinimum min = migrad();
    std::cout << min << std::endl;
}