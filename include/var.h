#ifndef VAR_H
#define VAR_H

#include <functional>
#include "event.h"
#include "interaction.h"

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
    if(I.t0 != -99999999 && evt.int_ttp_map.find(I.interaction_index) != evt.int_ttp_map.end())
        return S(evt, I) ? evt.reco_interactions.at(evt.int_ttp_map.at(I.interaction_index)).interaction_index : -1;
    else if(I.t0 == -99999999 && evt.int_ptt_map.find(I.interaction_index) != evt.int_ptt_map.end())
        return S(evt, I) ? evt.interactions.at(evt.int_ptt_map.at(I.interaction_index)).interaction_index : -1;
    return -1;
}

MAKEVAR(kFlashTime)
{
    if(evt.int_fmatch_map.find(I.interaction_index) != evt.int_fmatch_map.end())
        return evt.fmatches.at(evt.int_fmatch_map.at(I.interaction_index)).flash_time;
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

#endif