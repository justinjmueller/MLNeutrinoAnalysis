#ifndef VAR_H
#define VAR_H

#include <functional>
#include "event.h"
#include "interaction.h"
#include "cut.h"

#define MAKEVAR(NAME) double NAME(const Event& evt, const Interaction& I, const Selector& S)

typedef std::function<bool(const Event&, const Interaction&)> Selector;

MAKEVAR(kCount)
{
    return S(evt, I) ? 1 : 0;
}

MAKEVAR(kEventID)
{
    return S(evt, I) ? evt.image_index : -1;
}

MAKEVAR(kInteractionID)
{
    return S(evt, I) ? I.interaction_index : -1;
}

MAKEVAR(kVolume)
{
    return S(evt, I) ? I.volume : -1;
}

MAKEVAR(kMatchID)
{
    if(I.true_not_reco && evt.find_interaction(I))
        return S(evt, I) ? evt.get_interaction(I).interaction_index : -1;
    else if(!I.true_not_reco && evt.find_interaction(I))
        return S(evt, I) ? evt.get_interaction(I).interaction_index : -1;
    return -1;
}

MAKEVAR(kFlashTime)
{
    if(evt.find_fmatch(I))
        return evt.get_fmatch(I).flash_time;
    else
        return -999999;
}

MAKEVAR(kInteractionTime)
{
    return S(evt, I) ? I.t0 : -9999;
}

MAKEVAR(kNeutrinoEnergy)
{
    return S(evt, I) ? evt.neutrinos.at(0).momentum : -1;
}

MAKEVAR(kInteractionType)
{
    return S(evt, I) ? evt.neutrinos.at(0).interaction_type : -1;
}

MAKEVAR(kParticles)
{
    auto &x = I.particle_multiplicity;
    return S(evt, I) ? (std::pow(2, x.at(0)) * std::pow(3, x.at(1)) * std::pow(5, x.at(2)) * std::pow(7, x.at(3)) * std::pow(11, x.at(4))) : -1;
}

MAKEVAR(kPrimaries)
{
    auto &x = I.primary_multiplicity;
    return S(evt, I) ? (std::pow(2, x.at(0)) * std::pow(3, x.at(1)) * std::pow(5, x.at(2)) * std::pow(7, x.at(3)) * std::pow(11, x.at(4))) : -1;
}

MAKEVAR(kCategory)
{
    double t(-1);
    if(S(evt, I))
    {
        if(s1mu1p(evt, I) && sNeutrino(evt, I)) t = 0;
        else if(sNeutrino(evt, I)) t = 1;
        else t = 2;
    }
    return t;
}

MAKEVAR(kFlashX)
{
    if(S(evt, I) && evt.find_fmatch(I))
        return evt.get_fmatch(I).tpc_x;
    else
        return 0;
}

MAKEVAR(kTPCExtX0)
{
    if(S(evt, I) && evt.find_fmatch(I))
        return evt.get_fmatch(I).tpc_ext_x0;
    else
        return 0;
}

MAKEVAR(kTPCExtX1)
{
    if(S(evt, I) && evt.find_fmatch(I))
        return evt.get_fmatch(I).tpc_ext_x1;
    else
        return 0;
}

MAKEVAR(kTPCExtY0)
{
    if(S(evt, I) && evt.find_fmatch(I))
        return evt.get_fmatch(I).tpc_ext_y0;
    else
        return 0;
}

MAKEVAR(kTPCExtY1)
{
    if(S(evt, I) && evt.find_fmatch(I))
        return evt.get_fmatch(I).tpc_ext_y1;
    else
        return 0;
}

MAKEVAR(kTPCExtZ0)
{
    if(S(evt, I) && evt.find_fmatch(I))
        return evt.get_fmatch(I).tpc_ext_z0;
    else
        return 0;
}

MAKEVAR(kTPCExtZ1)
{
    if(S(evt, I) && evt.find_fmatch(I))
        return evt.get_fmatch(I).tpc_ext_z1;
    else
        return 0;
}

MAKEVAR(kCRTPMT)
{
    if(S(evt, I) && evt.find_fmatch(I))
    {
        std::vector<double> match_offsets;
        for(const CRTHit& h : evt.crthits)
        {
            auto off = h.ts1_ns / 1000.0 - evt.get_fmatch(I).flash_time;
            if(h.tagger == "Top" && std::abs(off) < 0.25) match_offsets.push_back(off);
        }
        if(match_offsets.size() == 1) return 1000.0*match_offsets.at(0);
        else return -99999;
    }
    else
        return -99999;
}

#endif