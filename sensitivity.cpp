#include <iostream>
#include <vector>
#include <random>
#include <math.h>

#include "TH1D.h"
#include "TFile.h"
#include "TNtuple.h"

#include <Eigen/Core>
#include <Eigen/LU>

#define NBINS 25
#define NTOYS 10000
#define SCALE 0.208
#define NGRID 1000

typedef Eigen::MatrixXd Matrix;
typedef Eigen::ArrayXd Array;
typedef Eigen::VectorXd Vector;

void make_grid(std::vector<double>& m0, std::vector<double>& m1)
{
  for(size_t i(0); i < NGRID; ++i)
  {
    for(size_t j(0); j < NGRID; ++j)
    {
      m0.push_back(i*(1.00 / NGRID));
      m1.push_back(j*(100.0 / NGRID));
    }
  }
}

void throw_toy_stat(std::vector<double>& l, std::vector<double>& e, Matrix& m, size_t t)
{
    TH1D *hist = new TH1D("hist", "hist", NBINS, 0.25, 2.5);
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<> d{0.00, 1.00};
    for(size_t i(0); i < l.size(); ++i)
        if(d(gen) < SCALE) hist->Fill(l[i] / (1000 * e[i]));
    for(size_t i(0); i < NBINS; ++i)
        m(t, i) = (hist->GetBinContent(i));
    delete hist;
}

void throw_toy_esys(std::vector<double>& l, std::vector<double>& e, Matrix& m, size_t t)
{
    TH1D *hist = new TH1D("hist", "hist", NBINS, 0.25, 2.5);
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> d{1.00, 0.10};
    for(size_t i(0); i < l.size(); ++i)
        hist->Fill(l[i] / (1000 * e[i] * d(gen)));
    for(size_t i(0); i < NBINS; ++i)
        m(t, i) = (hist->GetBinContent(i));
    delete hist;
}

void throw_toy_fsys(std::vector<double>& l, std::vector<double>& e, Matrix& m, size_t t)
{
    TH1D *hist = new TH1D("hist", "hist", NBINS, 0.25, 2.5);
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> d{1.00, 0.10};
    double flux_sys(d(gen));
    for(size_t i(0); i < l.size(); ++i)
        hist->Fill(l[i] / (1000 * e[i]), flux_sys);
    for(size_t i(0); i < NBINS; ++i)
        m(t, i) = (hist->GetBinContent(i));
    delete hist;
}

void calculate_partial(Matrix& data, Matrix& partial, Matrix& means)
{
    Matrix centered = data.rowwise() - data.colwise().mean();
    partial = (centered.adjoint() * centered) / double(data.rows() - 1);
    partial.array() /= means.array();
}

Matrix scale_partial(Matrix& partial, Vector& means)
{
    return partial.array() * Matrix(means * means.transpose()).array();
}

double throw_point(std::vector<double>& le, std::vector<Matrix>& partials, double theta, double dm)
{
    TH1D *nul = new TH1D("nul", "nul", NBINS, 0.25, 2.5);
    TH1D *alt = new TH1D("alt", "alt", NBINS, 0.25, 2.5);
    for(size_t i(0); i < le.size(); ++i)
    {
        nul->Fill(le[i], SCALE*(1-theta*std::sin(1.27*dm*le[i])*std::sin(1.27*dm*le[i])));
        alt->Fill(le[i], SCALE);
    }
    Vector nul_vec(NBINS), alt_vec(NBINS);
    Matrix M_stat(Eigen::MatrixXd::Zero(NBINS, NBINS));
    for(size_t b(0); b < NBINS; ++b)
    {
        nul_vec(b) = nul->GetBinContent(b);
        alt_vec(b) = alt->GetBinContent(b);
        M_stat(b, b) = 3.0 / (1.0 / nul_vec(b) + 2.0 / alt_vec(b));
    }
    delete nul;
    delete alt;
    
    Matrix M_syst(Eigen::MatrixXd::Zero(NBINS, NBINS));
    for(size_t s(0); s < partials.size(); ++s)
        M_syst += scale_partial(partials[s], alt_vec);
    
    Vector difference(nul_vec - alt_vec);
    return difference.transpose()*(((M_stat + M_syst).inverse())*difference);
}

int main()
{
    TFile input("/Users/mueller/Projects/likelihood/build/signal.root", "read");
    TNtuple *tuple = (TNtuple*)input.Get("signal");
    std::vector<double> length, energy, le;
    float l, e;
    tuple->SetBranchAddress("l", &l);
    tuple->SetBranchAddress("p", &e);
    for(size_t i(0); tuple->GetEntry(i) > 0; ++i)
    {
        length.push_back(l);
        energy.push_back(e);
        le.push_back(double(l) / double(1000*e));
    }
    input.Close();
    std::cout << "Read " << length.size() << " signal entries." << std::endl;
    std::vector<std::vector<double>> data;
    Matrix sample_stat(NTOYS, NBINS), sample_esys(NTOYS, NBINS), sample_fsys(NTOYS, NBINS);
    std::cout << "Throwing toys for covariance estimation...";
    for(size_t t(0); t < NTOYS; ++t) 
    {
        throw_toy_stat(length, energy, sample_stat, t);
        throw_toy_esys(length, energy, sample_esys, t);
        throw_toy_fsys(length, energy, sample_fsys, t);
    }
    std::cout << "done." << std::endl;
    Vector tmp(sample_stat.colwise().mean());
    Matrix mean(tmp * tmp.transpose());
    std::vector<Matrix> partials(2);
    calculate_partial(sample_esys, partials.at(0), mean);
    calculate_partial(sample_fsys, partials.at(1), mean);
    
    TFile output("chi2.root", "recreate");
    TNtuple *out_tuple = new TNtuple("sensitivity", "sensitivity", "m0:m1:chi2");
    std::vector<double> m0, m1;
    make_grid(m0, m1);
    std::cout << "Doing grid evaluation...";
    for(size_t i(0); i < m0.size(); ++i)
        out_tuple->Fill(m0[i], m1[i], throw_point(le, partials, m0[i], m1[i]));
    std::cout << "done." << std::endl;
    out_tuple->Write();
    output.Close();
}