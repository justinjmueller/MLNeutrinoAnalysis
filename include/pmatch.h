#ifndef PMATCH_H
#define PMATCH_H
#include "csv_parser.h"

class PMatch
{
 public:
  PMatch(const CSVRow& row);
  PMatch();
  int32_t image_index;
  int16_t from_index;
  int16_t from_pid;
  bool from_primary;
  int16_t to_index;
  int16_t to_pid;
  bool to_primary;
};
#endif
