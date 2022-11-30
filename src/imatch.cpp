#include <string>
#include "csv_parser.h"
#include "interaction.h"
#include "imatch.h"

IMatch::IMatch(const CSVRow& row)
: image_index(std::stoi(row[0])),
  volume(std::stoi(row[1])),
  from_index(std::stoi(row[6])),
  to_index(std::stoi(row[7])),
  from_primaries(row[3]),
  from_particles(row[2]),
  to_primaries(row[5]),
  to_particles(row[4]) { }

IMatch::IMatch()
: image_index(-1),
  volume(-1),
  from_index(-1),
  to_index(-1),
  from_primaries("None"),
  from_particles("None"),
  to_primaries("None"),
  to_particles("None") { }

void IMatch::update(const Interaction& from_int, const Interaction& to_int)
{
  auto& c = from_int.primary_multiplicity;
  from_primaries = (std::to_string(c[0]) + std::string("ph")
		    + std::to_string(c[1]) + std::string("e")
		    + std::to_string(c[2]) + std::string("mu")
		    + std::to_string(c[3]) + std::string("pi")
		    + std::to_string(c[4]) + std::string("p"));
  auto& d = to_int.primary_multiplicity;
  to_primaries = (std::to_string(d[0]) + std::string("ph")
		  + std::to_string(d[1]) + std::string("e")
		  + std::to_string(d[2]) + std::string("mu")
		  + std::to_string(d[3]) + std::string("pi")
		  + std::to_string(d[4]) + std::string("p"));
}
