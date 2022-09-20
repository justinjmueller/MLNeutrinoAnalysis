#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <math.h>
#include <numeric>

#include "csv_parser.h"
#include "particle.h"
#include "interaction.h"
#include "neutrino.h"
#include "pmatch.h"
#include "imatch.h"
#include "event.h"
#include "analyzer.h"
#include "variables.h"

#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#define VTX_THRESHOLD 30

template <class T>
void read_csv(std::string csv, std::string sub, std::vector<T>& obj_vector);

void read_all_events(std::map<int32_t, Event>& events, std::string sub);

int main()
{
  std::string base("bnb_numu");
  std::string sub("");
  //std::string sub("_sep19");
  
  std::map<int32_t, Event> events;
  read_all_events(events, sub);

  TFile output(std::string(base + sub + ".root").c_str(), "recreate");
  TTree event_tree("event_tree", "events");
  Event* evt;
  event_tree.Branch("events", &evt);

  typedef std::unique_ptr<Analyzer<double> > ptr;
  typedef std::unique_ptr<Analyzer<std::pair<double, double> > > pair_ptr;
  typedef std::unique_ptr<Analyzer<std::pair<std::string, std::string> > > string_pair_ptr;
  std::vector<ptr> ana;
  std::vector<pair_ptr> pair_ana;
  std::vector<string_pair_ptr> string_pair_ana;
  ana.push_back(ptr(new PurityEfficiency("purity_1mu1p", "0ph0e1mu0pi1p", true)));
  ana.push_back(ptr(new PurityEfficiency("efficiency_1mu1p", "0ph0e1mu0pi1p", false)));
  ana.push_back(ptr(new PurityEfficiency("purity_numucc", "*ph*e1mu*pi*p", true)));
  ana.push_back(ptr(new PurityEfficiency("efficiency_numucc", "*ph*e1mu*pi*p", false)));
  ana.push_back(ptr(new PurityEfficiency("purity_2ph1mu", "2ph0e1mu0pi0p", true)));
  ana.push_back(ptr(new PurityEfficiency("efficiency_2ph1mu", "2ph0e1mu0pi0p", false)));
  ana.push_back(ptr(new Unmatched("unmatched_ptt")));
  ana.push_back(ptr(new SplitInteraction("split_ptt")));
  ana.push_back(ptr(new DirectionResolution("muon_direction", 2)));
  ana.push_back(ptr(new DirectionResolution("pion_direction", 3)));
  ana.push_back(ptr(new DirectionResolution("proton_direction", 4)));
  ana.push_back(ptr(new NeutrinoMomentumRes("fulltruth_neutrino_direction", true)));
  ana.push_back(ptr(new NeutrinoMomentumRes("neutrino_direction", false)));
  ana.push_back(ptr(new PrimaryMatching("primary_matching")));
  ana.push_back(ptr(new PrimaryMisidentification("primary_misidentification", false)));
  ana.push_back(ptr(new ParticleMultiplicity("muon_multiplicity", 2)));
  ana.push_back(ptr(new ParticleMultiplicity("pion_multiplicity", 3)));
  ana.push_back(ptr(new ParticleMultiplicity("proton_multiplicity", 4)));
  ana.push_back(ptr(new NeutrinoMomentum("neutrino_momentum")));
  ana.push_back(ptr(new VertexResolution("vertex_resolution")));
  string_pair_ana.push_back(string_pair_ptr(new Confusion("confusion_1mu1p", "0ph0e1mu0pi1p")));
  string_pair_ana.push_back(string_pair_ptr(new Confusion("confusion_2ph1mu", "2ph0e1mu0pi0p")));
  pair_ana.push_back(pair_ptr(new ParticleEnergy("muon_energy", 2)));
  pair_ana.push_back(pair_ptr(new ParticleEnergy("pion_energy", 3)));
  pair_ana.push_back(pair_ptr(new ParticleEnergy("proton_energy", 4)));
  pair_ana.push_back(pair_ptr(new VisEnergy("vis_energy_1mu1p", "0ph0e1mu0pi1p")));
  pair_ana.push_back(pair_ptr(new VisEnergyToFull("vis_energy_full_1mu1p", "0ph0e1mu0pi1p", true)));
  pair_ana.push_back(pair_ptr(new VisEnergyToFull("reco_vis_energy_full_1mu1p", "0ph0e1mu0pi1p", false)));
  pair_ana.push_back(pair_ptr(new VisEnergy("vis_energy_numucc", "*ph*e1mu*pi*p")));
  pair_ana.push_back(pair_ptr(new VisEnergyToFull("vis_energy_full_numucc", "*ph*e1mu*pi*p", true)));
  pair_ana.push_back(pair_ptr(new VisEnergyToFull("reco_vis_energy_full_numucc", "*ph*e1mu*pi*p", false)));
  pair_ana.push_back(pair_ptr(new ParticleConfusion("confusion_particle")));
  pair_ana.push_back(pair_ptr(new PrimaryConfusion("confusion_primary")));
  
  for(auto& obj : events)
  { 
    evt = &(obj.second);
    event_tree.Fill();
    for(auto& a : ana) (*a)(obj.second);
    for(auto& a : string_pair_ana) (*a)(obj.second);
    for(auto& a : pair_ana) (*a)(obj.second);
  }
  event_tree.Write();

  for(auto& a: ana) output.WriteObject(&a->get_vars(), a->get_name().c_str());
  for(auto& a: string_pair_ana) output.WriteObject(&a->get_vars(), a->get_name().c_str());
  for(auto& a: pair_ana) output.WriteObject(&a->get_vars(), a->get_name().c_str());
  return 0;
}

template <class T>
void read_csv(std::string csv, std::string sub, std::vector<T>& obj_vector)
{
  csv += sub;
  csv += std::string(".csv");
  std::ifstream file(csv);
  int count(0);
  for(auto& row : CSVRange(file))
  {
    if(count > 0)
    {
      try
      {
	obj_vector.push_back(T(row));
      }
      catch(const std::invalid_argument& e)
      {
	std::cerr << "Invalid row detected at line "
		  << count
		  << " : "
		  << e.what()
		  << std::endl;
      }
    }
    ++count;
  }
  file.close();
}

void read_all_events(std::map<int32_t, Event>& events, std::string sub)
{
  std::vector<Particle> particles;
  read_csv("../csv/particles", sub, particles);
  std::vector<Particle> reco_particles;
  read_csv("../csv/reco_particles", sub, reco_particles);
  std::vector<Interaction> interactions;
  read_csv("../csv/interactions", sub, interactions);
  std::vector<Interaction> reco_interactions;
  read_csv("../csv/reco_interactions", sub, reco_interactions);
  std::vector<Neutrino> neutrinos;
  read_csv("../csv/neutrinos", sub, neutrinos);
  std::vector<IMatch> matches_ptt;
  read_csv("../csv/matched_ptt", sub, matches_ptt);
  std::vector<IMatch> matches_ttp;
  read_csv("../csv/matched_ttp", sub, matches_ttp);
  std::vector<PMatch> pmatches_ptt;
  read_csv("../csv/pmatch_ptt", sub, pmatches_ptt);
  std::vector<PMatch> pmatches_ttp;
  read_csv("../csv/pmatch_ttp", sub, pmatches_ttp);

  for(Neutrino& nu : neutrinos)
  {
    if(events.find(nu.image_index) == events.end())
    {
      events.insert(std::pair<int32_t,Event>(nu.image_index, Event()));
      events.at(nu.image_index).add_neutrino(nu);
    }
    else events.at(nu.image_index).add_neutrino(nu);
  }

  std::map<std::pair<int32_t,int16_t>, Interaction*> interaction_map;
  std::pair<int32_t, int16_t> key;
  for(Interaction& I : interactions)
  {
    key = std::make_pair(I.image_index, I.interaction_index);
    interaction_map.insert(std::pair<std::pair<int32_t,int16_t>, Interaction*>(key, &I));
  }
  for(Particle& p : particles)
  {
    key = std::make_pair(p.image_index, p.interaction_index);
    interaction_map.at(key)->add_particle(p);
  }
  for(Interaction& I : interactions)
  {
    if(events.find(I.image_index) != events.end())
      events.at(I.image_index).add_interaction(I);
    else std::cerr << "Interaction found without corresponding event!" << std::endl;
  }

  std::map<std::pair<int32_t,int16_t>, Interaction*> reco_interaction_map;
  for(Interaction& I : reco_interactions)
  {
    key = std::make_pair(I.image_index, I.interaction_index);
    reco_interaction_map.insert(std::pair<std::pair<int32_t,int16_t>, Interaction*>(key, &I));
  }
  for(Particle& p : reco_particles)
  {
    key = std::make_pair(p.image_index, p.interaction_index);
    reco_interaction_map.at(key)->add_particle(p);
  }
  for(Interaction& I : reco_interactions)
  {
    if(events.find(I.image_index) != events.end())
      events.at(I.image_index).add_reco_interaction(I);
    else std::cerr << "Interaction found without corresponding event!" << std::endl;
  }
  
  for(IMatch& m : matches_ptt)
  {
    if(events.find(m.image_index) != events.end()) events.at(m.image_index).add_match(m, true);
    else std::cerr << "Match found without corresponding event!" << std::endl;
  }
  for(IMatch& m : matches_ttp)
  {
    if(events.find(m.image_index) != events.end()) events.at(m.image_index).add_match(m, false);
    else std::cerr << "Match found without corresponding event!" << std::endl;
  }

  for(PMatch& m : pmatches_ptt)
  {
    if(events.find(m.image_index) != events.end()) events.at(m.image_index).add_pmatch(m, true);
    else std::cerr << "Particle Match found without corresponding event!" << std::endl;
  }
  for(PMatch& m : pmatches_ttp)
  {
    if(events.find(m.image_index) != events.end()) events.at(m.image_index).add_pmatch(m, false);
    else std::cerr << "Particle Match found without corresponding event!" << std::endl;
  }
  
  for(auto& evt : events)
  {
    for(Interaction& I : evt.second.interactions) I.update_particles(VTX_THRESHOLD);
    for(Interaction& I : evt.second.reco_interactions) I.update_particles(VTX_THRESHOLD);
    for(IMatch& m : evt.second.matches_ptt)
    {
      auto ptt_res = find_match(m, evt.second.reco_interactions,
				evt.second.interactions);
      if(ptt_res.first != -1 && ptt_res.second != -1)
	m.update(evt.second.reco_interactions.at(ptt_res.first),
		 evt.second.interactions.at(ptt_res.second));
    }
    for(IMatch& m : evt.second.matches_ttp)
    {
      auto ttp_res = find_match(m, evt.second.interactions,
				evt.second.reco_interactions);
      if(ttp_res.first != -1 && ttp_res.second != -1)
	m.update(evt.second.interactions.at(ttp_res.first),
		 evt.second.reco_interactions.at(ttp_res.second));
    }
  }
}
