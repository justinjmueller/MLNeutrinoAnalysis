#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>

#include "dataset.h"
#include "event.h"
#include "neutrino.h"
#include "interaction.h"
#include "particle.h"
#include "imatch.h"
#include "pmatch.h"
#include "fmatch.h"
#include "crthit.h"
#include "csv_parser.h"
#include "utilities.h"

#include "TFile.h"
#include "TTree.h"
#include "TNtupleD.h"

template <class T>
void read_csv(std::string csv, std::vector<T>& obj_vector);

Dataset::Dataset(std::string path, std::string dname, std::string sub)
: name(dname)
{
  std::vector<Particle> particles;
  read_csv(path + "particles" + sub + ".csv", particles);
  std::vector<Particle> reco_particles;
  read_csv(path + "reco_particles" + sub + ".csv", reco_particles);
  std::vector<Interaction> interactions;
  read_csv(path + "interactions" + sub + ".csv", interactions);
  std::vector<Interaction> reco_interactions;
  read_csv(path + "reco_interactions" + sub + ".csv", reco_interactions);
  std::vector<Neutrino> neutrinos;
  read_csv(path + "neutrinos" + sub + ".csv", neutrinos);
  std::vector<IMatch> matches_ptt;
  read_csv(path + "imatches_ptt" + sub + ".csv", matches_ptt);
  std::vector<IMatch> matches_ttp;
  read_csv(path + "imatches_ttp" + sub + ".csv", matches_ttp);
  std::vector<PMatch> pmatches_ptt;
  read_csv(path + "pmatches_ptt" + sub + ".csv", pmatches_ptt);
  std::vector<PMatch> pmatches_ttp;
  read_csv(path + "pmatches_ttp" + sub + ".csv", pmatches_ttp);
  std::vector<CRTHit> crthits;
  read_csv(path + "crthits" + sub + ".csv", crthits);
  std::vector<FMatch> fmatches;
  read_csv(path + "fmatches" + sub + ".csv", fmatches);

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
    I.true_not_reco = true;
    key = std::make_pair(I.image_index, I.interaction_index);
    interaction_map.insert(std::pair<std::pair<int32_t,int16_t>, Interaction*>(key, &I));
  }
  for(Particle& p : particles)
  {
    p.true_not_reco = true;
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
    I.true_not_reco = false;
    key = std::make_pair(I.image_index, I.interaction_index);
    reco_interaction_map.insert(std::pair<std::pair<int32_t,int16_t>, Interaction*>(key, &I));
  }
  for(Particle& p : reco_particles)
  {
    p.true_not_reco = false;
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
  for(CRTHit& c : crthits)
  {
    if(events.find(c.image_index) != events.end()) events.at(c.image_index).add_crthit(c);
    else std::cerr << "CRTHit found without corresponding event!" << std::endl;
  }
  for(FMatch& f : fmatches)
  {
    if(events.find(f.image_index) != events.end()) events.at(f.image_index).add_fmatch(f);
    else std::cerr << "FMatch found without corresponding event!" << std::endl;
  }
  
  for(auto& evt : events)
  {
    evt.second.generate_pointers();
    evt.second.pid_reweight();
    for(IMatch& m : evt.second.matches_ptt)
    {
      auto ptt_res = find_match(m, evt.second.reco_interactions, evt.second.interactions);
      if(ptt_res.first != -1 && ptt_res.second != -1)
        m.update(evt.second.reco_interactions.at(ptt_res.first), evt.second.interactions.at(ptt_res.second));
    }
    for(IMatch& m : evt.second.matches_ttp)
    {
      auto ttp_res = find_match(m, evt.second.interactions, evt.second.reco_interactions);
      if(ttp_res.first != -1 && ttp_res.second != -1)
        m.update(evt.second.interactions.at(ttp_res.first), evt.second.reco_interactions.at(ttp_res.second));
    }
  }
}

void Dataset::write_dataset(std::string out)
{
  TFile output((out + ".root").c_str(), "recreate");
  TTree dataset("dataset", "dataset");
  Event *evt;
  dataset.Branch("events", &evt);

  for(auto& obj : events)
  {
    evt = &(obj.second);
    dataset.Fill();
  }
  dataset.Write();
  output.Close();
}

void Dataset::read_dataset(std::string in)
{
  TFile input((in + ".root").c_str(), "read");
  TTree *dataset = (TTree*)input.Get("dataset");
  Event *evt;
  dataset->SetBranchAddress("events", &evt);

  for(size_t e(0); e < dataset->GetEntries(); ++e)
  {
    dataset->GetEntry(e);
    events.insert(std::make_pair(int32_t(e), *evt));
  }

  input.Close();
}

void Dataset::print_summary(bool (*ncut)(const Event&, const Interaction&), bool (*dcut)(const Event&, const Interaction&), bool (*pcut)(const Event&, const Interaction&))
{
  std::cout << "Dataset has " << events.size() << " events." << std::endl;
  uint32_t ne(0), de(0), np(0), dp(0);
  for(std::pair<int32_t, Event> v: events)
  {
    for(Interaction& i : v.second.interactions)
    {
      if(v.second.find_interaction(i))
      {
        
        if(ncut(v.second, i)) ++ne;
        if(dcut(v.second, i)) ++de;
      }
    }
    for(Interaction& i : v.second.reco_interactions)
    {
      if(v.second.find_interaction(i))
      {
        if(ncut(v.second, i)) ++np;
        if(pcut(v.second, i)) ++dp;
      }
    }
  }
  std::cout << "Efficiency: " << 100*double(ne)/double(de) << "%" << std::endl;
  std::cout << "Purity: " << 100*double(np)/double(dp) << "%" << std::endl;
}

void Dataset::add_truth_variable(const std::string name, double (*v)(const Event&, const Interaction&, const Selector&), const Selector& s)
{
  tvariables.push_back(std::bind(v, std::placeholders::_1,
                                std::placeholders::_2, s));
  if(tvar_tuple_string != "")
    tvar_tuple_string += (":" + name);
  else
    tvar_tuple_string += name;
}

void Dataset::add_truth_pvariable(const std::string name, double (*v)(const Event&, const Particle&, const PSelector&), const PSelector& s)
{
  tpvariables.push_back(std::bind(v, std::placeholders::_1,
				  std::placeholders::_2, s));
  if(tpvar_tuple_string != "")
    tpvar_tuple_string += (":" + name);
  else
    tpvar_tuple_string += name;
}

void Dataset::add_reco_variable(const std::string name, double (*v)(const Event&, const Interaction&, const Selector&), const Selector& s)
{
  rvariables.push_back(std::bind(v, std::placeholders::_1,
                                std::placeholders::_2, s));
  if(rvar_tuple_string != "")
    rvar_tuple_string += (":" + name);
  else
    rvar_tuple_string += name;
}

void Dataset::add_reco_pvariable(const std::string name, double (*v)(const Event&, const Particle&, const PSelector&), const PSelector& s)
{
  rpvariables.push_back(std::bind(v, std::placeholders::_1,
				  std::placeholders::_2, s));
  if(rpvar_tuple_string != "")
    rpvar_tuple_string += (":" + name);
  else
    rpvar_tuple_string += name;
}

void Dataset::add_common_variable(const std::string name, double (*v)(const Event&, const Interaction&, const Selector&), const Selector& s)
{
  add_truth_variable(name, v, s);
  add_reco_variable(name, v, s);
}

void Dataset::add_common_pvariable(const std::string name, double (*v)(const Event&, const Particle&, const PSelector&), const PSelector& s)
{
  add_truth_pvariable(name, v, s);
  add_reco_pvariable(name, v, s);
}

void Dataset::process_analysis()
{
  TFile output((name+"_results.root").c_str(), "recreate");
  TNtupleD ttuple((name+"_truth").c_str(), (name+"_truth").c_str(), tvar_tuple_string.c_str());
  TNtupleD rtuple((name+"_reco").c_str(), (name+"_reco").c_str(), rvar_tuple_string.c_str());
  TNtupleD tptuple((name+"_truth_particle").c_str(), (name+"_truth_particle").c_str(), tpvar_tuple_string.c_str());
  TNtupleD rptuple((name+"_reco_particle").c_str(), (name+"_reco_particle").c_str(), rpvar_tuple_string.c_str());
  double vt[tvariables.size()];
  double vr[rvariables.size()];
  double vpt[tpvariables.size()];
  double vpr[rpvariables.size()];
  for(const auto& obj : events)
  {
    for(const Interaction& I : obj.second.interactions)
    {
      for(size_t vi(0); vi < tvariables.size(); ++vi)
        vt[vi] = tvariables.at(vi)(obj.second, I);
      ttuple.Fill(vt);
      for(const Particle& p : I.particles)
      {
	for(size_t pvi(0); pvi < tpvariables.size(); ++pvi)
	  vpt[pvi] = tpvariables.at(pvi)(obj.second, p);
	tptuple.Fill(vpt);
      }
    }
    for(const Interaction& I : obj.second.reco_interactions)
    {
      for(size_t vi(0); vi < rvariables.size(); ++vi)
        vr[vi] = rvariables.at(vi)(obj.second, I);
      rtuple.Fill(vr);
      for(const Particle& p : I.particles)
      {
	for(size_t pvi(0); pvi < rpvariables.size(); ++pvi)
	  vpr[pvi] = rpvariables.at(pvi)(obj.second, p);
	rptuple.Fill(vpr);
      }
    }
  }
  ttuple.Write();
  rtuple.Write();
  tptuple.Write();
  rptuple.Write();
  output.Close();
}

template <class T>
void read_csv(std::string csv, std::vector<T>& obj_vector)
{
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
	std::cerr << "Invalid row detected in file "
		  << csv
		  << " at line "
		  << count
		  << " : "
		  << e.what()
		  << std::endl
		  << row.get_line()
		  << std::endl;
      }
    }
    ++count;
  }
  file.close();
}
