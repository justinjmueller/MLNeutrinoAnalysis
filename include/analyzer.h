#ifndef ANALYZER_H
#define ANALYZER_H

#include <vector>
#include <string>
#include "event.h"

template <class T>
class Analyzer
{
public:
  Analyzer<T>() { var = std::vector<T>(); };
  virtual void operator()(const Event& evt) = 0;
  std::vector<T>& get_vars() { return var; };
  std::string get_name() { return name; };
  virtual ~Analyzer() = default;
protected:
  std::vector<T> var;
  std::string name;
  void add_vars(std::vector<T>& v) { var.insert(var.end(), v.begin(), v.end()); };
};

#endif
