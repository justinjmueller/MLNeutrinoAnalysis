#ifndef DATASET_H
#define DATASET_H

#include <string>
#include <vector>
#include <map>
#include <functional>

#include "event.h"

typedef std::function<bool(const Event&, const Interaction&)> Selector;
typedef std::function<bool(const Event&, const Particle&)> PSelector;
typedef std::function<double(const Event&, const Interaction&)> VarReduced;
typedef std::function<double(const Event&, const Particle&)> PVarReduced;

class Dataset
{
public:
  Dataset(std::string path, std::string sub);
  void write_dataset(std::string out="output");
  void read_dataset(std::string in="output");
  void print_summary();
  void add_truth_variable(const std::string name, double (*v)(const Event&, const Interaction&, const Selector&), const Selector& s);
  void add_truth_pvariable(const std::string name, double (*v)(const Event&, const Particle&, const PSelector&), const PSelector& s);
  void add_reco_variable(const std::string name, double (*v)(const Event&, const Interaction&, const Selector&), const Selector& s);
  void add_reco_pvariable(const std::string name, double (*v)(const Event&, const Particle&, const PSelector&), const PSelector& s);
  void add_common_variable(const std::string name, double (*v)(const Event&, const Interaction&, const Selector&), const Selector& s);
  void add_common_pvariable(const std::string name, double (*v)(const Event&, const Particle&, const PSelector&), const PSelector& s);
  void process_analysis(const std::string name);

private:
  std::map<int32_t, Event> events;
  std::vector<VarReduced> tvariables;
  std::vector<VarReduced> rvariables;
  std::vector<PVarReduced> tpvariables;
  std::vector<PVarReduced> rpvariables;
  std::string tvar_tuple_string;
  std::string rvar_tuple_string;
  std::string tpvar_tuple_string;
  std::string rpvar_tuple_string;
};

#endif
