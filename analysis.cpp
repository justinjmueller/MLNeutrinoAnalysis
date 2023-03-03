#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <numeric>
#include <functional>

#include "csv_parser.h"
#include "particle.h"
#include "interaction.h"
#include "neutrino.h"
#include "pmatch.h"
#include "imatch.h"
#include "crthit.h"
#include "fmatch.h"
#include "event.h"
#include "analyzer.h"
#include "var.h"
#include "cut.h"
#include "pvar.h"
#include "pcut.h"
#include "dataset.h"

int main()
{
  std::vector<Dataset> all_data;
  all_data.push_back(Dataset("/Users/mueller/Downloads/intime/", "analysis_intime"));
  all_data.push_back(Dataset("../csv/", "analysis"));
  for(Dataset &data : all_data)
  {
    data.print_summary();

    // Common interaction variables.
    data.add_common_variable("event_id", kEventID, sEverything);
    data.add_common_variable("volume", kVolume, sEverything);
    data.add_common_variable("interaction_id", kInteractionID, sEverything);
    data.add_common_variable("match_id", kMatchID, sEverything);
    data.add_common_variable("primaries", kPrimaries, sEverything);
    data.add_common_variable("contained", kCount, sContained);
    data.add_common_variable("is1mu1p", kCount, s1mu1p);

    // Truth interaction variables.
    data.add_truth_variable("signal_true", kCount, sSignalTrue);
    data.add_truth_variable("neutrino", kCount, sNeutrino);
    data.add_truth_variable("flash_time", kFlashTime, sEverything);
    data.add_truth_variable("flash_x", kFlashX, sEverything);
    data.add_truth_variable("interaction_time", kInteractionTime, sEverything);
    data.add_truth_variable("neutrino_energy", kNeutrinoEnergy, sNeutrino);
    data.add_truth_variable("category", kCategory, sEverything);

    // Common particle variables.
    data.add_common_pvariable("event_id", kPEventID, sPEverything);
    data.add_common_pvariable("volume", kPVolume, sPEverything);
    data.add_common_pvariable("interaction_id", kPInteractionID, sPEverything);
    data.add_common_pvariable("particle_id", kParticleID, sPEverything);
    data.add_common_pvariable("match_id", kPMatchID, sPEverything);
    data.add_common_pvariable("pid", kPID, sPEverything);
    data.add_common_pvariable("primary", kPrimary, sPEverything);

    // Truth particle variables.
    data.add_truth_pvariable("neutrino", kPCount, sPNeutrino);
    data.add_truth_pvariable("energy_dep", kParticleEDep, sPEverything);

    // Reco particle variables.
    data.add_reco_pvariable("sm_primary", kSMPrimary, sPEverything);
    data.add_reco_pvariable("sm_nonprimary", kSMNonPrimary, sPEverything);

    data.process_analysis();
  }

  return 0;
}
