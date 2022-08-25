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

std::pair<size_t, size_t> find_match(const IMatch& m,
				     const std::vector<Interaction>& from_int,
				     const std::vector<Interaction>& to_int)
{
  std::pair<size_t, size_t> res(std::make_pair(-1,-1));
  for(size_t i(0); i < from_int.size(); ++i)
    if(from_int.at(i).interaction_index == m.from_index) res.first = i;
  for(size_t i(0); i < to_int.size(); ++i)
    if(to_int.at(i).interaction_index == m.to_index) res.second = i;
  return res;
}

std::vector<bool> find_best_cluster(const std::vector<Particle>& part, double thr)
{
  std::vector<std::pair<ROOT::Math::XYZPoint, std::vector<bool> > > distances;
  std::vector<bool> primary;
  for(const auto& p : part)
  {
    distances.push_back(std::make_pair(ROOT::Math::XYZPoint(p.vtx0x, p.vtx0y, p.vtx0z),
				    std::vector<bool>()));
    distances.push_back(std::make_pair(ROOT::Math::XYZPoint(p.vtx1x, p.vtx1y, p.vtx1z),
				    std::vector<bool>()));
    primary.push_back(p.primary);
  }
  for(auto& p0 : distances)
  {
    for(auto& p1 : distances)
    {
      p0.second.push_back(sqrt(pow(p0.first.X() - p1.first.X(), 2) +
			       pow(p0.first.Y() - p1.first.Y(), 2) +
			       pow(p0.first.Z() - p1.first.Z(), 2)) < thr);
    }
  }
  uint16_t best_count(0);
  size_t best_match(0);
  for(size_t d(0); d < distances.size(); ++d)
  {
    auto c = std::count(distances.at(d).second.begin(),
			distances.at(d).second.end(),
			true);
    if(c > best_count) best_match = d;
    if(c == best_count && primary.at(d)) best_match = d;
  }
  return distances.at(best_match).second;
}
