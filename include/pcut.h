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
    if(evt.find_parent_interaction(p))
        return evt.get_parent_interaction(p).nu_id > 0;
    return false;
}

MAKEPCUT(sPMichel)
{
    if(evt.find_parent_interaction(p))
        return evt.get_parent_interaction(p).michel_present;
    return false;
}

MAKEPCUT(sPOneToOne)
{
    return evt.find_particle(p) && evt.find_particle(evt.get_particle(p)) && evt.get_particle(evt.get_particle(p)).particle_index == p.particle_index;
}

#endif