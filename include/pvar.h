#ifndef PVAR_H
#define PVAR_H

#include <functional>
#include "event.h"
#include "particle.h"

#define MAKEPVAR(NAME) double NAME(const Event& evt, const Particle& p, const PSelector& S)

typedef std::function<bool(const Event&, const Particle&)> PSelector;

MAKEPVAR(kPCount)
{
    return S(evt, p) ? 1 : 0;
}

MAKEPVAR(kPEventID)
{
    return S(evt, p) ? evt.image_index : -1;
}

MAKEPVAR(kPInteractionID)
{
    return S(evt, p) ? p.interaction_index : -1;
}

MAKEPVAR(kParticleID)
{
    return S(evt, p) ? p.particle_index : -1;
}

MAKEPVAR(kPVolume)
{
    return S(evt, p) ? p.volume : -1;
}

MAKEPVAR(kPMatchID)
{
    if(p.t != -1 && evt.pmatch_ttp_map.find(p.particle_index) != evt.pmatch_ttp_map.end())
        return S(evt, p) ? evt.pmatch_ttp_map.at(p.particle_index) : -1;
    else if(p.t == -1 && evt.pmatch_ptt_map.find(p.particle_index) != evt.pmatch_ptt_map.end())
        return S(evt, p) ? evt.pmatch_ptt_map.at(p.particle_index) : -1;
    return -1;
}

MAKEPVAR(kPID)
{
    return S(evt, p) ? p.pid : -1;
}

MAKEPVAR(kPrimary)
{
    return S(evt, p) ? p.primary : -1;
}

#endif