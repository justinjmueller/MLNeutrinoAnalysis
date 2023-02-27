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
//#include "variables.h"
#include "var.h"
#include "cut.h"
#include "pvar.h"
#include "pcut.h"
//#include "refactor.h"
#include "dataset.h"

int main()
{
  Dataset data("../csv/", "");
  data.write_dataset();
  data.read_dataset();
  data.print_summary();

  // Common interaction variables.
  data.add_common_variable("event_id", kEventID, sEverything);
  data.add_common_variable("volume", kVolume, sEverything);
  data.add_common_variable("interaction_id", kInteractionID, sEverything);
  data.add_common_variable("match_id", kMatchID, sEverything);
  data.add_common_variable("primaries", kParticles, sEverything);
  data.add_common_variable("contained", kCount, sContained);
  data.add_common_variable("is1mu1p", kCount, s1mu1p);

  // Truth interaction variables.
  data.add_truth_variable("signal_true", kCount, sSignalTrue);
  data.add_truth_variable("neutrino", kCount, sNeutrino);
  data.add_truth_variable("flash_time", kFlashTime, sEverything);
  data.add_truth_variable("interaction_time", kInteractionTime, sEverything);
  data.add_truth_variable("neutrino_energy", kNeutrinoEnergy, sNeutrino);

  // Common particle variables.
  data.add_common_pvariable("event_id", kPEventID, sPEverything);
  data.add_common_pvariable("volume", kPVolume, sPEverything);
  data.add_common_pvariable("interaction_id", kPInteractionID, sPEverything);
  data.add_common_pvariable("particle_id", kParticleID, sPEverything);
  data.add_common_pvariable("match_id", kPMatchID, sPEverything);
  data.add_common_pvariable("pid", kPID, sPEverything);
  data.add_common_pvariable("primary", kPrimary, sPEverything);

  // Truth particle variables.

  data.process_analysis("analysis");

  return 0;
}
