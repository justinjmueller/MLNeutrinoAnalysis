#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include "Math/Point3D.h"
#include "interaction.h"
#include "pmatch.h"
#include "imatch.h"
#include "particle.h"

#define DRIFTV 0.1578 // [cm/us]
#define CATHODE_LOCATION 210.215 // [cm] +/-

std::vector<std::string> split(const std::string &s, char delim);
std::pair<size_t, size_t> find_match(const IMatch& m, const std::vector<Interaction>& from_int, const std::vector<Interaction>& to_int);
std::pair<size_t, size_t> find_match(const PMatch& m, const std::vector<Particle>& from_part, const std::vector<Particle>& to_part);
void parse_particle_string(std::string s, std::vector<uint16_t>& counts);
bool match_strings(std::string s1, std::string s2);
double drift_adjust(double x, double t0);

#endif
