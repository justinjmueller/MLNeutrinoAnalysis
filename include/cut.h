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

MAKECUT(sTriggeringVolume)
{
    return I.volume == evt.triggering_volume;
}

MAKECUT(sContained)
{
    return I.contained;
}

MAKECUT(sFlashTime)
{
    if(evt.find_interaction(I))
    {
        uint64_t ii(evt.get_interaction(I).interaction_index);
        if(evt.int_fmatch_map.find(ii) != evt.int_fmatch_map.end())
            return std::abs(evt.fmatches.at(evt.int_fmatch_map.at(ii)).flash_time - 0.8) <= 0.8;
    }
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