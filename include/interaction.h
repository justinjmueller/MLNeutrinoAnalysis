#ifndef INTERACTION_H
#define INTERACTION_H

#include <string>
#include <vector>
#include "csv_parser.h"
#include "particle.h"

class Interaction
{
 public:
  Interaction(const CSVRow& row);
  Interaction();
  void add_particle(const Particle& p);
  void update_particles(double thr);
  int32_t image_index;
  int16_t volume;
  int16_t interaction_index;
  int16_t nu_id;
  std::string particle_string;
  std::string primary_string;
  std::vector<uint16_t> particle_multiplicity;
  std::vector<uint16_t> primary_multiplicity;
  bool michel_present;
  double vertex_x;
  double vertex_y;
  double vertex_z;
  float t0;
  bool contained;
  double vis_energy;
  double reco_vis_energy;
  std::vector<Particle> particles;
  size_t nu_index;
  uint32_t voxels;
};
#endif
