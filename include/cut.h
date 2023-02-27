#ifndef CUT_H
#define CUT_H

#include <functional>
#include "event.h"
#include "interaction.h"

#define MAKECUT(NAME) bool NAME(const Event& evt, const Interaction& I)

MAKECUT(sEverything)
{
    return true;
}

MAKECUT(sNeutrino)
{
    return I.nu_id > 0;
}

MAKECUT(sContained)
{
    return I.contained;
}

MAKECUT(s1mu1p)
{
    return I.primary_string == "0ph0e1mu0pi1p";
}

MAKECUT(sSignalTrue)
{
    // Conditions:
    // 1. Must have exactly one muon and one proton (both primary).
    // 2. Must actually correspond to a generated neutrino.
    // 3. ???
    return I.primary_string == "0ph0e1mu0pi1p" && I.nu_id > 0;//&& selInTime(evt, I);
}

#endif