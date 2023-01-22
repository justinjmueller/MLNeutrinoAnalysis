#ifndef CRTHIT_H
#define CRTHIT_H

#include <string>
#include "csv_parser.h"

class CRTHit
{
 public:
  CRTHit(const CSVRow& row);
  CRTHit();
  int32_t image_index;
  double peshit;
  double ts0_s;
  double ts0_corr;
  double ts0_ns;
  double ts0_ns_corr;
  double ts1_ns;
  int16_t plane;
  std::string tagger;
  double xpos;
  double xerr;
  double ypos;
  double yerr;
  double zpos;
  double zerr;
};
#endif
