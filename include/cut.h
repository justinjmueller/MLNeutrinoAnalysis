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

MAKECUT(sVertexInsideVolume)
{
    bool inside(false);
    inside = inside || ((I.vertex_x > -358.49) && (I.vertex_x < -61.94));
    inside = inside || ((I.vertex_x < 358.49) && (I.vertex_x > 61.94));
    inside = inside && ((I.vertex_y > -181.86) && (I.vertex_y < 134.96));
    inside = inside && ((I.vertex_z > -895.95) && (I.vertex_z < 894.95));
    return inside;
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

MAKECUT(sContainedPart)
{
    double x0(1000), x1(-1000), y0(1000), y1(-1000), z0(1000), z1(-1000);
    for(const Particle& p : I.particles)
    {
        if(p.vtx0x < x0) x0 = p.vtx0x;
        if(p.vtx1x < x0) x0 = p.vtx1x;
        if(p.vtx0x > x1) x1 = p.vtx0x;
        if(p.vtx1x > x1) x1 = p.vtx1x;
        if(p.vtx0y < y0) y0 = p.vtx0y;
        if(p.vtx1y < y0) y0 = p.vtx1y;
        if(p.vtx0y > y1) y1 = p.vtx0y;
        if(p.vtx1y > y1) y1 = p.vtx1y;
        if(p.vtx0z < z0) z0 = p.vtx0z;
        if(p.vtx1z < z0) z0 = p.vtx1z;
        if(p.vtx0z > z1) z1 = p.vtx0z;
        if(p.vtx1z > z1) z1 = p.vtx1z;
    }
    bool contained(false);
    contained = contained || ((x0 > -358.49 + CNTPAD) && (x1 < -61.94 - CNTPAD));
    contained = contained || ((x1 < 358.49 - CNTPAD) && (x0 > 61.94 + CNTPAD));
    contained = contained && ((y0 > -181.86 + CNTPAD) && (y1 < 134.96 - CNTPAD));
    contained = contained && ((z0 > -895.95 + CNTPAD) && (z1 < 894.95 - CNTPAD));
    return contained;
}

MAKECUT(sFiducial)
{
    typedef ROOT::Math::XYZPoint Pt;
    float vtxx(0), vtxy(0), vtxz(0);
    if(!sVertexInsideVolume(evt, I))
    {
        uint16_t pn(0);
        std::vector<Pt> points;
        for(const Particle& p : I.particles)
        {
            if(p.primary)
            {
                ++pn;
                points.push_back(Pt(p.vtx0x, p.vtx0y, p.vtx0z));
                points.push_back(Pt(p.vtx1x, p.vtx1y, p.vtx1z));
                if(pn == 2)
                {
                    std::vector<double> distances;
                    distances.push_back(std::sqrt(Pt(points[0].X() - points[2].X(), points[0].Y() - points[2].Y(), points[0].Z() - points[2].Z()).Mag2()));
                    distances.push_back(std::sqrt(Pt(points[0].X() - points[3].X(), points[0].Y() - points[3].Y(), points[0].Z() - points[3].Z()).Mag2()));
                    distances.push_back(std::sqrt(Pt(points[1].X() - points[2].X(), points[1].Y() - points[2].Y(), points[1].Z() - points[2].Z()).Mag2()));
                    distances.push_back(std::sqrt(Pt(points[1].X() - points[3].X(), points[1].Y() - points[3].Y(), points[1].Z() - points[3].Z()).Mag2()));
                    size_t min(std::min_element(distances.begin(), distances.end()) - distances.begin());
                    size_t i, j;
                    switch (min)
                    {
                    case 0:
                        i = 0;
                        j = 2;
                        break;
                    case 1:
                        i = 0;
                        j = 3;
                        break;
                    case 2:
                        i = 1;
                        j = 2;
                        break;
                    case 3:
                        i = 1;
                        j = 3;
                        break;
                    default:
                        i = 0;
                        j = 2;
                        break;
                    }
                    vtxx = (points[i].X() + points[j].X())/2.0;
                    vtxy = (points[i].Y() + points[j].Y())/2.0;
                    vtxz = (points[i].Z() + points[j].Z())/2.0;
                }
            }
        }
    }
    else
    {
        vtxx = I.vertex_x;
        vtxy = I.vertex_y;
        vtxz = I.vertex_z;
    }

    bool fiducial(false);
    fiducial = fiducial || ((vtxx > -358.49 + FVPAD) && (vtxx < -61.94 - FVPAD));
    fiducial = fiducial || ((vtxx < 358.49 - FVPAD) && (vtxx > 61.94 + FVPAD));
    fiducial = fiducial && ((vtxy > -181.86 + FVPAD) && (vtxy < 134.96 - FVPAD));
    fiducial = fiducial && ((vtxz > -895.95 + FVPADZD) && (vtxz < 894.95 - FVPADZU));
    return fiducial;
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