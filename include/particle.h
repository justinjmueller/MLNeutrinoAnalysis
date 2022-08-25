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
  double px, py, pz;
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
};
#endif
