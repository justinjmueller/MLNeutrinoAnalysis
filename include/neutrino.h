#ifndef NEUTRINO_H
#define NEUTRINO_H
#include "csv_parser.h"

class Neutrino
{
 public:
  Neutrino(const CSVRow& row);
  Neutrino();
  int32_t image_index;
  int16_t pdg;
  int16_t current_type;
  int16_t interaction_type;
  double momentum;
  double px;
  double py;
  double pz;
  double x;
  double y;
  double z;
  double t;
  size_t interaction_index;
};
#endif
