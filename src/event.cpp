#include <vector>
#include <map>
#include <cmath>
#include "neutrino.h"
#include "interaction.h"
#include "pmatch.h"
#include "imatch.h"
#include "crthit.h"
#include "fmatch.h"
#include "event.h"

Event::Event()
  : image_index(-1),
    triggering_volume(0),
    neutrinos(std::vector<Neutrino>()),
    interactions(std::vector<Interaction>()),
    reco_interactions(std::vector<Interaction>()),
    matches_ptt(std::vector<IMatch>()),
    matches_ttp(std::vector<IMatch>()),
    pmatches_ptt(std::vector<PMatch>()),
    pmatches_ttp(std::vector<PMatch>()),
    crthits(std::vector<CRTHit>()),
    fmatches(std::vector<FMatch>()),
    reco_fmatches(std::vector<FMatch>()),
    interaction_map(std::map<std::pair<uint16_t, uint16_t>, size_t>()),
    reco_interaction_map(std::map<std::pair<uint16_t, uint16_t>, size_t>()),
    particle_map(std::map<std::pair<uint16_t, uint16_t>, std::pair<size_t, size_t>>()),
    reco_particle_map(std::map<std::pair<uint16_t, uint16_t>, std::pair<size_t, size_t>>()),
    int_ptt_map(std::map<std::pair<uint16_t, uint16_t>, size_t>()),
    int_ttp_map(std::map<std::pair<uint16_t, uint16_t>, size_t>()),
    int_fmatch_map(std::map<std::pair<uint16_t, uint16_t>, size_t>()),
    int_reco_fmatch_map(std::map<std::pair<uint16_t, uint16_t>, size_t>()),
    pmatch_ttp_map(std::map<std::pair<uint16_t, uint16_t>, uint16_t>()),
    pmatch_ptt_map(std::map<std::pair<uint16_t, uint16_t>, uint16_t>()) { }

void Event::add_neutrino(const Neutrino& nu)
{
  neutrinos.push_back(nu);
  image_index = nu.image_index;
}

void Event::add_interaction(const Interaction& in)
{
  interactions.push_back(in);
  interaction_map.insert(std::make_pair(std::make_pair(in.interaction_index, in.volume), interactions.size()-1));
}

void Event::add_reco_interaction(const Interaction& in)
{
  reco_interactions.push_back(in);
  reco_interaction_map.insert(std::make_pair(std::make_pair(in.interaction_index, in.volume), reco_interactions.size()-1));
}

void Event::add_match(const IMatch& ma, bool ptt=true)
{
  if(ptt)
  {
    matches_ptt.push_back(ma);
    if(ma.to_index != -1)
      int_ptt_map.insert(std::make_pair(std::make_pair(ma.from_index, ma.volume), interaction_map.at(std::make_pair(ma.to_index, ma.volume))));
  }
  else
  {
    matches_ttp.push_back(ma);
    if(ma.to_index != -1)
      int_ttp_map.insert(std::make_pair(std::make_pair(ma.from_index, ma.volume), reco_interaction_map.at(std::make_pair(ma.to_index, ma.volume))));
  }
}

void Event::add_pmatch(const PMatch& ma, bool ptt=true)
{
  if(ptt)
  {
    pmatches_ptt.push_back(ma);
    if(ma.to_index != -1)
      pmatch_ptt_map.insert(std::make_pair(std::make_pair(ma.from_index, ma.volume), ma.to_index));
  }
  else 
  {
    pmatches_ttp.push_back(ma);
    if(ma.to_index != -1)
      pmatch_ttp_map.insert(std::make_pair(std::make_pair(ma.from_index, ma.volume), ma.to_index));
  }
}

void Event::add_crthit(const CRTHit& c)
{
  crthits.push_back(c);
}

void Event::add_fmatch(const FMatch& f)
{
  if(f.true_not_reco)
  {
    fmatches.push_back(f);
    int_fmatch_map.insert(std::make_pair(std::make_pair(f.interaction_index, f.volume), fmatches.size()-1));
  }
  else
  {
    reco_fmatches.push_back(f);
    int_reco_fmatch_map.insert(std::make_pair(std::make_pair(f.interaction_index, f.volume), reco_fmatches.size()-1));
  }
}

void Event::generate_pointers()
{
  for(size_t ii(0); ii < interactions.size(); ++ii)
  {
    for(size_t pi(0); pi < interactions[ii].particles.size(); ++pi)
      particle_map.insert(std::make_pair(std::make_pair(interactions[ii].particles[pi].particle_index, interactions[ii].volume), std::make_pair(ii, pi)));
  }
  for(size_t ii(0); ii < reco_interactions.size(); ++ii)
  {
    for(size_t pi(0); pi < reco_interactions[ii].particles.size(); ++pi)
      reco_particle_map.insert(std::make_pair(std::make_pair(reco_interactions[ii].particles[pi].particle_index, reco_interactions[ii].volume), std::make_pair(ii, pi)));
  }
  double min_time(99999);
  for(const Interaction& I : interactions)
  {
    if(std::abs(I.t0 - 0.8) < min_time)
    {
      min_time = std::abs(I.t0 - 0.8);
      triggering_volume = I.volume;
    }
  }
  for(const Neutrino& nu: neutrinos)
    if(nu.image_index != -1) triggering_volume = nu.x < 0 ? 0 : 1;
}

void Event::pid_reweight()
{
  std::vector<std::string> types = {"ph", "e", "mu", "pi", "p"};
  for(Interaction& i : reco_interactions)
  {
    for(Particle& p : i.particles)
    {
      bool prim = p.softmax_primary >= 0.1 ? true : false;
      float d0(std::sqrt(std::pow(i.vertex_x - p.vtx0x, 2) + std::pow(i.vertex_y - p.vtx0y, 2) + std::pow(i.vertex_z - p.vtx0z, 2)));
      float d1(std::sqrt(std::pow(i.vertex_x - p.vtx1x, 2) + std::pow(i.vertex_y - p.vtx1y, 2) + std::pow(i.vertex_z - p.vtx1z, 2)));
      if((d0 > 0 && d0 < 10) || (d1 > 0 && d1 < 10)) prim = true;

      size_t pid(0);
      if(p.softmax_photon != 0 || p.softmax_electron != 0)
        pid = p.softmax_photon > p.softmax_electron ? 0 : 1;
      else
      {
        if(p.softmax_proton >= 0.85) pid = 4;
        else pid = p.softmax_muon >= 0.1 ? 2 : 3;
      }

      i.particle_multiplicity.at(p.pid)--;
      if(p.primary) i.primary_multiplicity.at(p.pid)--;
      i.particle_multiplicity.at(pid)++;
      if(prim) i.primary_multiplicity.at(pid)++;
      p.primary = prim;
      p.pid = pid;
    }
    i.primary_string = "";
    for(size_t j(0); j < 5; ++j)
      i.primary_string += std::to_string(i.primary_multiplicity.at(j)) + types.at(j);
  }
}

bool Event::find_interaction(const Interaction& in) const
{
  if(in.true_not_reco)
    return int_ttp_map.find(std::make_pair(in.interaction_index, in.volume)) != int_ttp_map.end();
  else
    return int_ptt_map.find(std::make_pair(in.interaction_index, in.volume)) != int_ptt_map.end();
}

const Interaction& Event::get_interaction(const Interaction& in) const
{
  if(in.true_not_reco)
    return reco_interactions.at(int_ttp_map.at(std::make_pair(in.interaction_index, in.volume)));
  else
    return interactions.at(int_ptt_map.at(std::make_pair(in.interaction_index, in.volume)));
}

bool Event::find_fmatch(const Interaction& in) const
{
  if(in.true_not_reco)
    return int_fmatch_map.find(std::make_pair(in.interaction_index, in.volume)) != int_fmatch_map.end();
  else  
    return int_reco_fmatch_map.find(std::make_pair(in.interaction_index, in.volume)) != int_reco_fmatch_map.end();
}

const FMatch& Event::get_fmatch(const Interaction& in) const
{
  if(in.true_not_reco)
    return fmatches.at(int_fmatch_map.at(std::make_pair(in.interaction_index, in.volume)));
  else
    return reco_fmatches.at(int_reco_fmatch_map.at(std::make_pair(in.interaction_index, in.volume)));
}

bool Event::find_particle(const Particle& p) const
{
  const std::pair<uint16_t, uint16_t>& m = std::make_pair(p.particle_index, p.volume);
  if(p.true_not_reco)
    return pmatch_ttp_map.find(m) != pmatch_ttp_map.end() && reco_particle_map.find(std::make_pair(pmatch_ttp_map.at(m), p.volume)) != reco_particle_map.end();
  else
    return pmatch_ptt_map.find(m) != pmatch_ptt_map.end() && particle_map.find(std::make_pair(pmatch_ptt_map.at(m), p.volume)) != particle_map.end();
}

const Particle& Event::get_particle(const Particle& p) const
{
  const std::pair<uint16_t, uint16_t>& m = std::make_pair(p.particle_index, p.volume);
  if(p.true_not_reco)
  {
    auto& r = reco_particle_map.at(std::make_pair(pmatch_ttp_map.at(m), p.volume));
    return reco_interactions.at(r.first).particles.at(r.second);
  }
  else
  {
    auto& r = particle_map.at(std::make_pair(pmatch_ptt_map.at(m), p.volume));
    return interactions.at(r.first).particles.at(r.second);
  }
}

bool Event::find_parent_interaction(const Particle& p) const
{
  const std::pair<uint16_t, uint16_t>& m = std::make_pair(p.particle_index, p.volume);
  if(p.true_not_reco)
    return particle_map.find(m) != particle_map.end();
  else
    return reco_particle_map.find(m) != reco_particle_map.end();
}

const Interaction& Event::get_parent_interaction(const Particle& p) const
{
  const std::pair<uint16_t, uint16_t>& m = std::make_pair(p.particle_index, p.volume);
  if(p.true_not_reco)
    return interactions.at(particle_map.at(m).first);
  else
    return reco_interactions.at(reco_particle_map.at(m).first);
}
