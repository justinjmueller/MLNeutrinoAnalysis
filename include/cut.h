#ifndef CUT_H
#define CUT_H

#include <functional>
#include "event.h"
#include "interaction.h"
#include "utilities.h"

#define MAKECUT(NAME) bool NAME(const Event& evt, const Interaction& I)
#define CNTPAD 5 // Containment padding around volume.
#define FVPAD 25 // Fiducial padding in non-z directions.
#define FVPADZU 50 // Fiducial padding along z (upstream).
#define FVPADZD 30 // Fiducial padding along z (downstream).

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

MAKECUT(sFiducial)
{
    if(evt.neutrinos.size() > 0)
    {
        const Neutrino& nu = evt.neutrinos.at(0);
        bool fiducial(false);
        fiducial = fiducial || ((nu.x > -358.49 + FVPAD) && (nu.x < -61.94 - FVPAD));
        fiducial = fiducial || ((nu.x < 358.49 - FVPAD) && (nu.x > 61.94 + FVPAD));
        fiducial = fiducial && ((nu.y > -181.86 + FVPAD) && (nu.y < 134.96 - FVPAD));
        fiducial = fiducial && ((nu.z > -895.95 + FVPADZD) && (nu.z < 894.95 - FVPADZU));
        return fiducial;
    }
    return true;
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

MAKECUT(s1mu1pVis)
{
    // For true signal definition.
    std::vector<uint16_t> vis_primaries(5, 0);
    find_vis_primaries(I, vis_primaries);
    return (vis_primaries[0] == 0 && vis_primaries[1] == 0 && vis_primaries[2] == 1 && vis_primaries[3] == 0 && vis_primaries[4] == 1);
}

MAKECUT(s1muNpVis)
{
    // For true signal definition.
    std::vector<uint16_t> vis_primaries(5, 0);
    find_vis_primaries(I, vis_primaries);
    return (vis_primaries[0] == 0 && vis_primaries[1] == 0 && vis_primaries[2] == 1 && vis_primaries[3] == 0 && vis_primaries[4] >= 1);
}

MAKECUT(s1muIncVis)
{
    // For true signal definition.
    std::vector<uint16_t> vis_primaries(5, 0);
    find_vis_primaries(I, vis_primaries);
    return (vis_primaries[2] == 1);
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