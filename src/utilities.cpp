#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include "Math/Point3D.h"
#include "interaction.h"
#include "particle.h"
#include "utilities.h"

std::vector<std::string> split(const std::string &s, char delim)
{
  std::istringstream iss(s);
  std::string item;
  std::vector<std::string> elems;
  while(std::getline(iss, item, delim))
  {
    if(!item.empty()) elems.push_back(item);
  }
  return elems;
}

std::pair<size_t, size_t> find_match(const IMatch& m, const std::vector<Interaction>& from_int, const std::vector<Interaction>& to_int)
{
  std::pair<size_t, size_t> res(std::make_pair(-1,-1));
  for(size_t i(0); i < from_int.size(); ++i)
    if(from_int.at(i).interaction_index == m.from_index) res.first = i;
  for(size_t i(0); i < to_int.size(); ++i)
    if(to_int.at(i).interaction_index == m.to_index) res.second = i;
  return res;
}

std::pair<size_t, size_t> find_match(const PMatch& m, const std::vector<Particle>& from_part, const std::vector<Particle>& to_part)
{
  std::pair<size_t, size_t> res(std::make_pair(-1,-1));
  for(size_t i(0); i < from_part.size(); ++i)
    if(from_part.at(i).particle_index == m.from_index) res.first = i;
  for(size_t i(0); i < to_part.size(); ++i)
    if(to_part.at(i).particle_index == m.to_index) res.second = i;
  return res;
}

void parse_particle_string(std::string s, std::vector<uint16_t>& counts)
{
  std::vector<std::string> types = {"ph", "e", "mu", "pi", "p"};
  size_t pos(0), prv(0);
  for(size_t t(0); t < 5; ++t)
  {
    pos = s.find(types[t], prv);
    auto sub = s.substr(prv, pos-prv);
    try
    {
    if(sub != "*")
      counts.at(t) = std::stoi(sub);
    else
      counts.at(t) = 999;
    }
    catch (const std::exception&)
    {
      std::cerr << "std::stoi called with argument: " << sub << std::endl;
      std::cerr << pos << ", " << prv << ", " << s.size() << std::endl;
      std::cerr << "string requested is: " << s << std::endl;
      throw;
    }
    prv = pos + types[t].length();
  }
}

bool match_strings(std::string s1, std::string s2)
{
  if(s2.find("*") == std::string::npos)
    return (s1 == s2);
  else
  {
    std::vector<uint16_t> c1(5,0), c2(5,0);
    parse_particle_string(s1, c1);
    parse_particle_string(s2, c2);
    bool matches(true);
    for(size_t i(0); i < 5; ++i)
    {
      if(c2.at(i) != 999)
	matches = matches && (c1.at(i) == c2.at(i));
    }
    return matches;
  }
}

double drift_adjust(double x, double t0)
{
  int16_t sign(x < CATHODE_LOCATION * (x < 0 ? -1 : 1) ? -1 : 1);
  return sign * t0 * DRIFTV + x;
}

void find_vis_primaries(const Interaction& I, std::vector<uint16_t>& vis_primaries)
{
  for(const Particle& p : I.particles)
  {
    if(p.primary)
    {
        switch (p.pid)
        {
        case 0:
            vis_primaries.at(0)++;
            break;
        case 1:
            vis_primaries.at(1)++;
            break;
        case 2:
            if(p.energy_dep > 40.0) vis_primaries.at(2)++;
            break;
        case 3:
            vis_primaries.at(3)++;
            break;
        case 4:
            if(p.energy_dep > 25.0) vis_primaries.at(4)++;
            break;
        default:
            break;
        }
    }
  }
}
