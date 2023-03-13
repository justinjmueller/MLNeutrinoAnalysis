#ifndef FMATCH_H
#define FMATCH_H

#include "csv_parser.h"

class FMatch
{
 public:
  FMatch(const CSVRow& row);
  FMatch();
  int32_t image_index;
  int16_t volume;
  int32_t interaction_index;
  int32_t flash_id;
  double score;
  double totalpe;
  double totalpe0;
  double totalpe1;
  double tpc_x;
  double tpc_x_err;
  double tpc_y;
  double tpc_y_err;
  double tpc_z;
  double tpc_z_err;
  double flash_time;
  double flash_time_width;
  double flash_abs_time;
  double flash_frame;
  double flash_center_x;
  double flash_width_x;
  double flash_center_y;
  double flash_width_y;
  double flash_center_z;
  double flash_width_z;
  double flash_fast_to_total;
  bool flash_in_beam_frame;
  int32_t flash_on_beam_time;
};
#endif
