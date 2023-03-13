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
  all_data.push_back(Dataset("../csv/nucosmic/", "analysis"));
  for(Dataset &data : all_data)
  {
    data.print_summary(sMatch1mu1p, sSignalTrue, sSignalReco);

    // Common interaction variables.
    data.add_common_variable("event_id", kEventID, sEverything);
    data.add_common_variable("volume", kVolume, sEverything);
    data.add_common_variable("interaction_id", kInteractionID, sEverything);
    data.add_common_variable("match_id", kMatchID, sEverything);
    data.add_common_variable("primaries", kPrimaries, sEverything);
    data.add_common_variable("contained", kCount, sContained);
    data.add_common_variable("is1mu1p", kCount, s1mu1p);
    data.add_common_variable("voxels", kVoxels, sEverything);
    data.add_common_variable("vertex_x", kVertexX, sEverything);
    data.add_common_variable("vertex_y", kVertexY, sEverything);
    data.add_common_variable("vertex_z", kVertexZ, sEverything);
    data.add_common_variable("flash_time", kFlashTime, sEverything);
    data.add_common_variable("flash_x", kFlashX, sEverything);
    data.add_common_variable("flash_pe", kFlashPE, sEverything);
    data.add_common_variable("fiducial", kCount, sFiducial);
    data.add_common_variable("tpc_ext_x0", kTPCExtX0, sEverything);
    data.add_common_variable("tpc_ext_x1", kTPCExtX1, sEverything);
    data.add_common_variable("tpc_ext_y0", kTPCExtY0, sEverything);
    data.add_common_variable("tpc_ext_y1", kTPCExtY1, sEverything);
    data.add_common_variable("tpc_ext_z0", kTPCExtZ0, sEverything);
    data.add_common_variable("tpc_ext_z1", kTPCExtZ1, sEverything);
    data.add_common_variable("signal_1mu1pvis", kCount, s1mu1pVis);
    data.add_common_variable("signal_1muNpvis", kCount, s1muNpVis);
    data.add_common_variable("signal_1muIncvis", kCount, s1muIncVis);

    // Truth interaction variables.
    data.add_truth_variable("signal_true", kCount, sSignalTrue);
    data.add_truth_variable("neutrino", kCount, sNeutrino);
    data.add_truth_variable("triggering_volume", kCount, sTriggeringVolume);
    data.add_truth_variable("interaction_time", kInteractionTime, sEverything);
    data.add_truth_variable("neutrino_energy", kNeutrinoEnergy, sNeutrino);
    data.add_truth_variable("interaction_type", kInteractionType, sNeutrino);
    data.add_truth_variable("contained_adj", kCount, sContainedAdj);
    data.add_truth_variable("pmtcrt", kCRTPMT, sEverything);

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
    data.add_reco_pvariable("sm_photon", kSMPhoton, sPEverything);
    data.add_reco_pvariable("sm_electron", kSMElectron, sPEverything);
    data.add_reco_pvariable("sm_muon", kSMMuon, sPEverything);
    data.add_reco_pvariable("sm_pion", kSMPion, sPEverything);
    data.add_reco_pvariable("sm_proton", kSMProton, sPEverything);

    data.process_analysis();
  }

  return 0;
}
