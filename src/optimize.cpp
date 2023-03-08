#include "optimize.h"
#include "event.h"
#include "cut.h"

Optimize::Optimize(Dataset& d)
{
    std::map<int32_t, Event>& e = d.get_events();
    for(std::pair<int32_t, Event> v : e)
        events.push_back(Event(v.second));
}

double Optimize::operator()(const std::vector<double>& m) const
{
    uint32_t purity_numerator(0), purity_denominator(0);
    uint32_t efficiency_numerator(0), efficiency_denominator(0);
    for(const Event& evt : events)
    {
        Event new_event = reweight_event(evt, m);
        for(Interaction& i : new_event.interactions)
        {
            if(new_event.find_interaction(i))
            {
                if(sMatch1mu1p(new_event, i)) ++efficiency_numerator;
                if(sSignalTrue(new_event, i)) ++efficiency_denominator;
            }
        }
        for(Interaction& i : new_event.reco_interactions)
        {
            if(new_event.find_interaction(i))
            {
                if(sMatch1mu1p(new_event, i)) ++purity_numerator;
                if(sSignalReco(new_event, i)) ++purity_denominator;
            }
        }
    }
    return -1 * ((double(purity_numerator) / purity_denominator) * (double(efficiency_numerator) / efficiency_denominator));
}

Event Optimize::reweight_event(const Event& evt, const std::vector<double>& m) const
{
    std::vector<std::string> types = {"ph", "e", "mu", "pi", "p"};
    Event new_event(evt);
    for(Interaction& i : new_event.reco_interactions)
    {
        for(Particle& p : i.particles)
        {
            bool prim = m[0]*p.softmax_primary > m[1]*p.softmax_nonprimary;
            std::vector<double> psm = {m[2]*p.softmax_photon, m[3]*p.softmax_electron, m[4]*p.softmax_muon, m[5]*p.softmax_pion, m[6]*p.softmax_proton};
            size_t pid = std::max_element(psm.begin(), psm.end()) - psm.begin();
            i.particle_multiplicity.at(p.pid)--;
            if(p.primary) i.primary_multiplicity.at(p.pid)--;
            i.particle_multiplicity.at(pid)++;
            if(prim) i.primary_multiplicity.at(pid)++;
        }
        i.primary_string = "";
        for(size_t j(0); j < 5; ++j)
            i.primary_string += std::to_string(i.primary_multiplicity.at(j)) + types.at(j);
    }
    return new_event;
}