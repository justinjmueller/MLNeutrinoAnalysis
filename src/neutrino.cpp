#include "csv_parser.h"
#include "neutrino.h"

Neutrino::Neutrino(const CSVRow& row)
: image_index(std::stoi(row[0])),
  pdg(std::stoi(row[1])),
  current_type(std::stoi(row[2])),
  interaction_type(std::stoi(row[3])),
  momentum(std::stod(row[4])),
  px(std::stod(row[5])),
  py(std::stod(row[6])),
  pz(std::stod(row[7])),
  x(std::stod(row[8])),
  y(std::stod(row[9])),
  z(std::stod(row[10])),
  t(std::stod(row[11])),
  interaction_index(9999) { }

Neutrino::Neutrino()
  : image_index(-1),
    pdg(-1),
    current_type(-1),
    interaction_type(-1),
    momentum(-1),
    px(-1),
    py(-1),
    pz(-1),
    x(-1),
    y(-1),
    z(-1),
    t(-1),
    interaction_index(9999) { }
