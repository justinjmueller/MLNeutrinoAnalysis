#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include <vector>
#include "event.h"
#include "dataset.h"
#include "Minuit2/FCNBase.h"

class Optimize : public ROOT::Minuit2::FCNBase
{
public:
    Optimize(Dataset& d);
    ~Optimize() {};
    virtual double Up() const {return 1.;}
    virtual double operator()(const std::vector<double>& m) const;
    Event reweight_event(const Event& evt, const std::vector<double>& m) const;

private:
    std::vector<Event> events;
};

#endif