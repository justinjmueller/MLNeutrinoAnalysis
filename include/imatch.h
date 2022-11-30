#ifndef IMATCH_H
#define IMATCH_H
#include <string>
#include "interaction.h"
#include "csv_parser.h"

class IMatch
{
 public:
  IMatch(const CSVRow& row);
  IMatch();
  void update(const Interaction& from_int, const Interaction& to_int);
  int32_t image_index;
  int16_t volume;
  int16_t from_index;
  int16_t to_index;
  std::string from_primaries;
  std::string from_particles;
  std::string to_primaries;
  std::string to_particles;
};
#endif
