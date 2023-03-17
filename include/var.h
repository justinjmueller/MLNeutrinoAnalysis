#ifndef VAR_H
#define VAR_H

#include <functional>
#include "event.h"
#include "interaction.h"
#include "cut.h"

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
    if(S(evt, I) && evt.find_interaction(I))
        return evt.get_interaction(I).interaction_index;
    else
        return -1;
}

MAKEVAR(kFlashTime)
{
    if(evt.find_fmatch(I))
        return evt.get_fmatch(I).flash_time;
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

MAKEVAR(kInteractionType)
{
    return S(evt, I) ? evt.neutrinos.at(0).interaction_type : -1;
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

MAKEVAR(kFlashX)
{
    if(S(evt, I) && evt.find_fmatch(I))
        return evt.get_fmatch(I).tpc_x;
    else
        return 0;
}

MAKEVAR(kFlashPE)
{
    if(S(evt, I) && evt.find_fmatch(I))
        return evt.get_fmatch(I).totalpe;
    else
        return 0;
}

MAKEVAR(kVoxels)
{
    uint32_t vox(0);
    for(const Particle& p : I.particles)
        vox += p.voxels;
    return vox;
}

MAKEVAR(kVertexX)
{
    if(S(evt, I))
        return I.vertex_x;
    else return -1;
}

MAKEVAR(kFailureMode)
{
    if(S(evt, I))
    {
        uint16_t matches(0);
        for(const Interaction& ri : evt.reco_interactions)
            if(kMatchID(evt, ri, S) == I.interaction_index) ++matches;

        uint16_t primary_failure(0);
        for(const Particle& p : I.particles)
            if(evt.find_particle(p) && evt.get_particle(p).primary != p.primary) ++primary_failure;

        uint16_t pid_failure(0);
        for(const Particle& p : I.particles)
            if(evt.find_particle(p) && evt.get_particle(p).pid == p.pid) ++ pid_failure;

        return std::pow(2, matches!=1) * std::pow(3, primary_failure!=0) * std::pow(5, pid_failure!=0);
    }
    else
        return -1;
}

MAKEVAR(kVertexY)
{
    if(S(evt, I))
        return I.vertex_y;
    else return -1;
}

MAKEVAR(kVertexZ)
{
    if(S(evt, I))
        return I.vertex_z;
    else return -1;
}

MAKEVAR(kTPCExtX0)
{
    if(S(evt, I))
        return I.tpc_ext_x0;
    else
        return 0;
}

MAKEVAR(kTPCExtX1)
{
    if(S(evt, I))
        return I.tpc_ext_x1;
    else
        return 0;
}

MAKEVAR(kTPCExtY0)
{
    if(S(evt, I))
        return I.tpc_ext_y0;
    else
        return 0;
}

MAKEVAR(kTPCExtY1)
{
    if(S(evt, I))
        return I.tpc_ext_y1;
    else
        return 0;
}

MAKEVAR(kTPCExtZ0)
{
    if(S(evt, I))
        return I.tpc_ext_z0;
    else
        return 0;
}

MAKEVAR(kTPCExtZ1)
{
    if(S(evt, I))
        return I.tpc_ext_z1;
    else
        return 0;
}

MAKEVAR(kMuonEnergy)
{
    if(S(evt, I))
    {   
        double max(-1.);
        for(const Particle& p : I.particles)
        {
            if(p.pid == 2 && p.primary && (I.true_not_reco ? p.energy_dep : p.range_reco_energy) > max)
                max = (I.true_not_reco ? p.energy_dep : p.range_reco_energy);
        }
        return max;
    }
    else
        return -1;
}

MAKEVAR(kProtonEnergy)
{
    if(S(evt, I))
    {   
        double max(-1.);
        for(const Particle& p : I.particles)
        {
            if(p.pid == 4 && p.primary && (I.true_not_reco ? p.energy_dep : p.range_reco_energy) > max)
                max = (I.true_not_reco ? p.energy_dep : p.range_reco_energy);
        }
        return max;
    }
    else
        return -1;
}

MAKEVAR(kCRTPMT)
{
    if(S(evt, I) && evt.find_fmatch(I))
    {
        std::vector<double> match_offsets;
        for(const CRTHit& h : evt.crthits)
        {
            auto off = h.ts1_ns / 1000.0 - evt.get_fmatch(I).flash_time;
            if(h.tagger == "Top" && std::abs(off) < 0.25) match_offsets.push_back(off);
        }
        if(match_offsets.size() == 1) return 1000.0*match_offsets.at(0);
        else return -99999;
    }
    else
        return -99999;
}

#endif