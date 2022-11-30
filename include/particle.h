#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>
#include "csv_parser.h"

class Particle
{
public:
  Particle(const CSVRow& row);
  Particle();
  int32_t image_index;
  int16_t volume;
  int16_t particle_index;
  int16_t interaction_index;
  int16_t pid;
  int16_t semantic;
  bool primary;
  bool contained;
  bool cathode_crosser;
  bool crosses_x;
  bool crosses_y;
  bool crosses_z;
  int32_t voxels;
  double length;
  double momentum;
  double px, py, pz, t;
  double energy_init;
  double energy_dep;
  double sed;
  double reco_energy;
  double theta;
  double reco_theta;
  int16_t nchildren;
  double reco_dir_x;
  double reco_dir_y;
  double reco_dir_z;
  bool p0_localQ_lowest;
  double expa;
  double expb;
  double vtx0x, vtx0y, vtx0z;
  double vtx1x, vtx1y, vtx1z;
  std::string parent_primaries;
  std::string parent_particles;
  bool neutron_ancestor;
  bool primary_heuristic;
  double softmax_nonprimary;
  double softmax_primary;
  double softmax_photon;
  double softmax_electron;
  double softmax_muon;
  double softmax_pion;
  double softmax_proton;
};
#endif
