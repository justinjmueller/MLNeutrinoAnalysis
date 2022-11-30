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
    interaction_map(std::map<uint16_t, Interaction*>()),
    reco_interaction_map(std::map<uint16_t, Interaction*>()),
    particle_map(std::map<uint16_t, Particle*>()),
    reco_particle_map(std::map<uint16_t, Particle*>()) { }
    //particle_map(std::map<std::pair<uint16_t, uint16_t>, Particle*>()),
    //reco_particle_map(std::map<std::pair<uint16_t, uint16_t>, Particle*>()){ }

void Event::add_neutrino(const Neutrino& nu)
{
  neutrinos.push_back(nu);
  image_index = nu.image_index;
}

void Event::add_interaction(const Interaction& in)
{
  interactions.push_back(in);
  interaction_map.insert(std::make_pair(in.interaction_index, &interactions.back()));
}

void Event::add_reco_interaction(const Interaction& in)
{
  reco_interactions.push_back(in);
  reco_interaction_map.insert(std::make_pair(in.interaction_index, &reco_interactions.back()));
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
  for(auto& i : interactions)
  {
    for(auto& n : neutrinos)
    {
      //if(sqrt(pow(n.x - i.vertex_x, 2) + pow(n.y - i.vertex_y, 2) + pow(n.z - i.vertex_z, 2)) < 1)
      if(i.nu_index == 1 && abs(n.t - i.t0) < 5)
      {
	i.nu_index = &n - &neutrinos[0];
	n.interaction_index = &i - &interactions[0];
      }
      else i.nu_index = -1;
    }
    for(auto& p: i.particles)
      particle_map.insert(std::make_pair(p.particle_index, &p));
  }
  for(auto& i : reco_interactions)
  {
    for(auto& p: i.particles)
      reco_particle_map.insert(std::make_pair(p.particle_index, &p));
  }
}
