#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>
#include "csv_parser.h"

class Particle
{
public:
  Particle(const CSVRow& row);
  Particle();
  bool true_not_reco;
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
  double range_reco_energy;
  double calo_energy;
  double theta;
  double reco_theta;
  int16_t nchildren;
  double reco_dir_x;
  double reco_dir_y;
  double reco_dir_z;
  double vtx0x, vtx0y, vtx0z;
  double vtx1x, vtx1y, vtx1z;
  int32_t parent_pdg;
  int16_t parent_tid;
  std::string parent_primaries;
  std::string parent_particles;
  double softmax_nonprimary;
  double softmax_primary;
  double softmax_photon;
  double softmax_electron;
  double softmax_muon;
  double softmax_pion;
  double softmax_proton;
};
#endif
