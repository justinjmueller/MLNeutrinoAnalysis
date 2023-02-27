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

#endif