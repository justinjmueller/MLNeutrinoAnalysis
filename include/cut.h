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
    bool contained(false);
    contained = contained || ((I.tpc_ext_x0 > -358.49 + CNTPAD) && (I.tpc_ext_x1 < -61.94 - CNTPAD));
    contained = contained || ((I.tpc_ext_x1 < 358.49 - CNTPAD) && (I.tpc_ext_x0 > 61.94 + CNTPAD));
    contained = contained && ((I.tpc_ext_y0 > -181.86 + CNTPAD) && (I.tpc_ext_y1 < 134.96 - CNTPAD));
    contained = contained && ((I.tpc_ext_z0 > -895.95 + CNTPAD) && (I.tpc_ext_z1 < 894.95 - CNTPAD));
    return contained;
}

MAKECUT(sFiducial)
{
    bool fiducial(false);
    fiducial = fiducial || ((I.vertex_x > -358.49 + FVPAD) && (I.vertex_x < -61.94 - FVPAD));
    fiducial = fiducial || ((I.vertex_x < 358.49 - FVPAD) && (I.vertex_x > 61.94 + FVPAD));
    fiducial = fiducial && ((I.vertex_y > -181.86 + FVPAD) && (I.vertex_y < 134.96 - FVPAD));
    fiducial = fiducial && ((I.vertex_z > -895.95 + FVPADZD) && (I.vertex_z < 894.95 - FVPADZU));
    return fiducial;
}

MAKECUT(sVertexInsideVolume)
{
    bool inside(false);
    inside = inside || ((I.vertex_x > -358.49) && (I.vertex_x < -61.94));
    inside = inside || ((I.vertex_x < 358.49) && (I.vertex_x > 61.94));
    inside = inside && ((I.vertex_y > -181.86) && (I.vertex_y < 134.96));
    inside = inside && ((I.vertex_z > -895.95) && (I.vertex_z < 894.95));
    return inside;
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
    std::vector<uint16_t> vis_primaries(5, 0);
    find_vis_primaries(I, vis_primaries);
    return (vis_primaries[0] == 0 && vis_primaries[1] == 0 && vis_primaries[2] == 1 && vis_primaries[3] == 0 && vis_primaries[4] == 1);
}

MAKECUT(s1muNpVis)
{
    std::vector<uint16_t> vis_primaries(5, 0);
    find_vis_primaries(I, vis_primaries);
    return (vis_primaries[0] == 0 && vis_primaries[1] == 0 && vis_primaries[2] == 1 && vis_primaries[3] == 0 && vis_primaries[4] >= 1);
}

MAKECUT(s1muIncVis)
{
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