#ifndef PCUT_H
#define PCUT_H

#include <functional>
#include "event.h"
#include "particle.h"

#define MAKEPCUT(NAME) bool NAME(const Event& evt, const Particle& p)

MAKEPCUT(sPEverything)
{
    return true;
}

MAKEPCUT(sPContained)
{
    return p.contained;
}

MAKEPCUT(sPNeutrino)
{
    if(evt.particle_map.find(std::make_pair(p.particle_index, p.volume)) != evt.particle_map.end())
        return evt.interactions.at(evt.particle_map.at(std::make_pair(p.particle_index, p.volume)).first).nu_id > 0;
    return false;
}

#endif