#include <vector>
#include <map>
#include <cmath>
#include "neutrino.h"
#include "interaction.h"
#include "pmatch.h"
#include "imatch.h"
#include "event.h"

Event::Event()
  : image_index(-1),
    neutrinos(std::vector<Neutrino>()),
    interactions(std::vector<Interaction>()),
    reco_interactions(std::vector<Interaction>()),
    matches_ptt(std::vector<IMatch>()),
    matches_ttp(std::vector<IMatch>()),
    interaction_map(std::map<uint16_t, size_t>()),
    reco_interaction_map(std::map<uint16_t, size_t>()),
    particle_map(std::map<uint16_t, std::pair<size_t, size_t>>()),
    reco_particle_map(std::map<uint16_t, std::pair<size_t, size_t>>()) { }

void Event::add_neutrino(const Neutrino& nu)
{
  neutrinos.push_back(nu);
  image_index = nu.image_index;
}

void Event::add_interaction(const Interaction& in)
{
  interactions.push_back(in);
  interaction_map.insert(std::make_pair(in.interaction_index, interactions.size()-1));
}

void Event::add_reco_interaction(const Interaction& in)
{
  reco_interactions.push_back(in);
  reco_interaction_map.insert(std::make_pair(in.interaction_index, reco_interactions.size()-1));
}

void Event::add_match(const IMatch& ma, bool ptt=true)
{
  if(ptt) matches_ptt.push_back(ma);
  else matches_ttp.push_back(ma);
}

void Event::add_pmatch(const PMatch& ma, bool ptt=true)
{
  if(ptt) pmatches_ptt.push_back(ma);
  else pmatches_ttp.push_back(ma);
}

void Event::generate_pointers()
{
  for(size_t ii(0); ii < interactions.size(); ++ii)
  {
    for(auto& n : neutrinos)
    {
      if(interactions[ii].nu_id == 1)
      {
	interactions[ii].nu_index = &n - &neutrinos[0];
	n.interaction_index = ii;
      }
      else interactions[ii].nu_index = -1;
    }
    for(size_t pi(0); pi < interactions[ii].particles.size(); ++pi)
      particle_map.insert(std::make_pair(interactions[ii].particles[pi].particle_index, std::make_pair(ii, pi)));
  }
  for(size_t ii(0); ii < reco_interactions.size(); ++ii)
  {
    for(size_t pi(0); pi < reco_interactions[ii].particles.size(); ++pi)
      reco_particle_map.insert(std::make_pair(reco_interactions[ii].particles[pi].particle_index, std::make_pair(ii, pi)));
  }
}
