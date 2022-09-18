#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include "Math/Point3D.h"
#include "interaction.h"
#include "pmatch.h"
#include "imatch.h"
#include "particle.h"

std::vector<std::string> split(const std::string &s, char delim);
std::pair<size_t, size_t> find_match(const IMatch& m,
				     const std::vector<Interaction>& from_int,
				     const std::vector<Interaction>& to_int);
std::pair<size_t, size_t> find_match(const PMatch& m,
				     const std::vector<Particle>& from_part,
				     const std::vector<Particle>& to_part);
std::vector<bool> find_best_cluster(const std::vector<Particle>& part, double thr);
void parse_particle_string(std::string s, std::vector<uint16_t>& counts);
bool match_strings(std::string s1, std::string s2);
#endif
