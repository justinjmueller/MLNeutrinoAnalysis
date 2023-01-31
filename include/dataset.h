#ifndef DATASET_H
#define DATASET_H

#include <string>
#include <vector>
#include <map>

#include "event.h"

class Dataset
{
public:
  Dataset(std::string path, std::string sub);
  void write_dataset(std::string out="output");
  void read_dataset(std::string in="output");
  void print_summary();

private:
  std::map<int32_t, Event> events;

};

#endif
