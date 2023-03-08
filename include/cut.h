#ifndef CUT_H
#define CUT_H

#include <functional>
#include "event.h"
#include "interaction.h"

#define MAKECUT(NAME) bool NAME(const Event& evt, const Interaction& I)
#define CNTPAD 5 // Containment padding around volume.

MAKECUT(sEverything)
{
    return true;
}

MAKECUT(sNeutrino)
{
    return I.nu_id > 0;
}

MAKECUT(sTriggeringVolume)
{
    return I.volume == evt.triggering_volume;
}

MAKECUT(sContained)
{
    return I.contained;
}

MAKECUT(sContainedAdj)
{
    if(evt.find_fmatch(I))
    {
        const FMatch& f(evt.get_fmatch(I));
        bool contained(false);
        contained = contained || ((f.tpc_ext_x0 > -358.49 + CNTPAD) && (f.tpc_ext_x1 < -61.94 - CNTPAD));
        contained = contained || ((f.tpc_ext_x1 < 358.49 - CNTPAD) && (f.tpc_ext_x0 > 61.94 + CNTPAD));
        contained = contained && ((f.tpc_ext_y0 > -181.86 + CNTPAD) && (f.tpc_ext_y1 < 134.96 - CNTPAD));
        contained = contained && ((f.tpc_ext_z0 > -895.95 + CNTPAD) && (f.tpc_ext_z1 < 894.95 - CNTPAD));
        return contained;
    }
    return false;
}

MAKECUT(sFlashTime)
{
    if(evt.find_interaction(I) && evt.find_fmatch(I))
        return std::abs(evt.get_fmatch(I).flash_time - 0.8) <= 0.8;
    else
        return false;
}

MAKECUT(s1mu1p)
{
    return I.primary_string == "0ph0e1mu0pi1p";
}

MAKECUT(sSignalTrue)
{
    return s1mu1p(evt, I) && sNeutrino(evt, I) && sTriggeringVolume(evt, I);
}

MAKECUT(sSignalReco)
{
    return s1mu1p(evt, I) && sTriggeringVolume(evt, I);
}

MAKECUT(sMatch1mu1p)
{
    if(I.true_not_reco)
        return evt.find_interaction(I) && sSignalTrue(evt, I) && sSignalReco(evt, evt.get_interaction(I));
    else
        return evt.find_interaction(I) && sSignalReco(evt, I) && sSignalTrue(evt, evt.get_interaction(I));
}

#endif