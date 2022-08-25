#include <vector>
#include <string>
#include "utilities.h"
#include "event.h"
#include "analyzer.h"

#define MUON_MASS 105.6583745
#define PION_MASS 139.57039
#define PROTON_MASS 938.2720813

class Unmatched : public Analyzer<double>
{
public:
  Unmatched(std::string n)
  { name = n; };
  void operator()(const Event& evt)
  {
    std::vector<double> v(2,0);
    for(const IMatch& m : evt.matches_ptt)
    {
      v[0]++;
      if(m.to_index == -1) v[1]++;
    }
    this->add_vars(v);
  }
};

class SplitInteraction : public Analyzer<double>
{
public:
  SplitInteraction(std::string n)
  { name = n; };
  void operator()(const Event& evt)
  {
    std::vector<double> v(2,0);
    std::map<int16_t, int16_t> from_counts, to_counts;
    for(const IMatch& m : evt.matches_ptt)
    {
      if(from_counts.find(m.from_index) == from_counts.end())
	from_counts.insert(std::make_pair(m.from_index,1));
      else from_counts.at(m.from_index)++;
      if(to_counts.find(m.to_index) == to_counts.end())
	to_counts.insert(std::make_pair(m.to_index,1));
      else to_counts.at(m.to_index)++;
    }
    for(auto &c : to_counts)
      if(c.second > 1 && c.first != -1) v[1] += c.second;
    if(to_counts.find(-1) != to_counts.end()) v[1]--;
    for(const Interaction& I : evt.interactions) v[0]++;
    this->add_vars(v);
  }
};


class VertexResolution : public Analyzer<double>
{
public:
  VertexResolution(std::string n)
  { name = n; };
  void operator()(const Event& evt)
  {
    std::vector<double> v;
    for(const IMatch& m : evt.matches_ttp)
    {
      auto mindices = find_match(m, evt.interactions, evt.reco_interactions);
      if(mindices.first != -1 && mindices.second != -1
	 && evt.interactions.at(mindices.first).vertex_x != -1
	 && evt.reco_interactions.at(mindices.second).vertex_x != -1)
      {
	auto& p0 = evt.interactions.at(mindices.first);
	auto& p1 = evt.reco_interactions.at(mindices.second);
	v.push_back(sqrt(std::pow(p0.vertex_x - p1.vertex_x, 2)
			 + std::pow(p0.vertex_y - p1.vertex_y, 2)
			 + std::pow(p0.vertex_z - p1.vertex_z, 2)));
      }
    }
    this->add_vars(v);
  }
};

class ParticleMultiplicity : public Analyzer<double>
{
  int16_t pid;
public:
  ParticleMultiplicity(std::string n, int16_t p)
  { name = n; pid = p; };
  void operator()(const Event& evt)
  {
    std::vector<double> v;
    for(const Interaction& I : evt.interactions)
      v.push_back(I.primary_multiplicity.at(pid));
    this->add_vars(v);
  }
};

class PrimaryMisidentification : public Analyzer<double>
{
  bool ptt;
public:
  PrimaryMisidentification(std::string n, bool ptt)
  { name = n; ptt = ptt; };
  void operator()(const Event& evt)
  {
    std::vector<double> v;
    for(const IMatch& m : (ptt ? evt.matches_ptt : evt.matches_ttp))
    {
      auto& from_ints = (ptt ? evt.reco_interactions : evt.interactions);
      auto& to_ints = (ptt ? evt.interactions : evt.reco_interactions);
      auto mindices = find_match(m, from_ints, to_ints);
      if(mindices.first != -1 && mindices.second != -1)
      {
	double fromc = std::accumulate(from_ints.at(mindices.first).primary_multiplicity.begin(),
				       from_ints.at(mindices.first).primary_multiplicity.end(), 0.0);
	double toc = std::accumulate(to_ints.at(mindices.second).primary_multiplicity.begin(),
				     to_ints.at(mindices.second).primary_multiplicity.end(), 0.0);
	v.push_back(fromc-toc);
      }
    }
    this->add_vars(v);
  }
};

class ParticleEnergy : public Analyzer<std::pair<double, double> >
{
  int16_t pid;
public:
  ParticleEnergy(std::string n, int16_t p)
  { name = n; pid = p; };
  void operator()(const Event& evt)
  {
    std::vector<std::pair<double, double>> v;
    for(const Interaction& I : evt.interactions)
    {
      for(const Particle& p : I.particles)
      {
	if(p.pid == pid && p.contained && p.primary && p.nchildren <= 1)
	  v.push_back(std::make_pair(p.energy_dep, p.reco_energy));
      }
    }
    this->add_vars(v);
  }
};

class VisEnergy : public Analyzer<std::pair<double, double> >
{
  std::string target;
public:
  VisEnergy(std::string n, std::string t)
  { name = n; target = t; };
  void operator()(const Event& evt)
  {
    std::vector<std::pair<double, double> > v;
    for(const Interaction& I : evt.interactions)
    {
      v.push_back(std::make_pair(I.vis_energy, I.reco_vis_energy));
      if(!I.contained || v.back().first == 0.0) v.pop_back();
    }
    this->add_vars(v);
  }
  
};

class NeutrinoMomentum : public Analyzer<double>
{
public:
  NeutrinoMomentum(std::string n)
  { name = n; };
  void operator()(const Event& evt)
  {
    std::vector<double> v;
    for(const Neutrino& nu : evt.neutrinos)
    {
      if(nu.pdg == 14)
	v.push_back(nu.momentum);
    }
    this->add_vars(v);
  }
};

class PrimaryMatching : public Analyzer<double>
{
public:
  PrimaryMatching(std::string n) { name = n; };
  void operator()(const Event& evt)
  {
    std::vector<double> v(2, 0);
    for(const IMatch& m : evt.matches_ttp)
    {
      v[0]++;
      if(m.from_primaries == m.to_primaries) v[1]++;
    }
    this->add_vars(v);
  }
};

class PurityEfficiency : public Analyzer<double>
{
private:
  std::string target;
  bool do_purity;
public:
  PurityEfficiency(std::string n, std::string t, bool purity=true)
  { name = n; target = t; do_purity = purity; };
  void operator()(const Event& evt)
  {
    std::vector<double> v(2, 0);
    for(const IMatch& m : (do_purity ? evt.matches_ptt : evt.matches_ttp))
    {
      if(match_strings(m.from_primaries, target))
      {
	v[0]++;
	if(match_strings(m.to_primaries, target)) v[1]++;
      }     
    }
    this->add_vars(v);
  }
};

class DirectionResolution : public Analyzer<double>
{
private:
  int16_t pid;
public:
  DirectionResolution(std::string n, int16_t p)
  { name = n; pid = p; };
  void operator()(const Event& evt)
  {
    std::vector<double> v;
    for(const Interaction& I : evt.interactions)
    {
      for(const Particle& p : I.particles)
      {
	if(p.primary && p.contained && p.pid == pid)
	{
	  double cosine = (p.px * p.reco_dir_x
			   + p.py * p.reco_dir_y
			   + p.pz * p.reco_dir_z) / (p.momentum);
	  if(p.reco_dir_z < 0) cosine = -1*cosine;
	  v.push_back(acos(cosine) * (180/3.14159265));
	}
      }
    }
    this->add_vars(v);
  }
};

class NeutrinoMomentumRes : public Analyzer<double>
{
  bool use_truth;
public:
  NeutrinoMomentumRes(std::string n, bool t=false)
  { name = n; use_truth = t; };
  void operator()(const Event& evt)
  {
    std::vector<double> v;
    std::vector<double> p_muon, p_proton, p_nu;
    for(const Interaction& I : evt.interactions)
    {
      if(I.primary_string == "0ph0e1mu0pi1p")
      {
	for(const Particle& p : I.particles)
	{
	  if(p.primary && p.contained && p.pid == 2)
	  {
	    // Found muon.
	    if(use_truth)
	    {
	      p_muon.push_back(p.px);
	      p_muon.push_back(p.py);
	      p_muon.push_back(p.pz);
	    }
	    else
	    {
	      p_muon.push_back(p.reco_energy * p.reco_dir_x);
	      p_muon.push_back(p.reco_energy * p.reco_dir_y);
	      p_muon.push_back(p.reco_energy * p.reco_dir_z);
	    }
	  }
	  else if(p.primary && p.contained && p.pid == 4)
	  {
	    // Found proton.
	    if(use_truth)
	    {
	      p_proton.push_back(p.reco_energy * p.reco_dir_x);
	      p_proton.push_back(p.reco_energy * p.reco_dir_y);
	      p_proton.push_back(p.reco_energy * p.reco_dir_z);
	    }
	    else
	    {
	      p_proton.push_back(p.px);
	      p_proton.push_back(p.px);
	      p_proton.push_back(p.px);
	    }
	  }
	}
      }
    }
    if(p_muon.size() == 3 && p_proton.size() == 3)
    {
      auto& nu = evt.neutrinos.at(0);
      std::vector<double> recop = {p_muon.at(0) + p_proton.at(0),
				   p_muon.at(1) + p_proton.at(1),
				   p_muon.at(2) + p_proton.at(2)};
      double mag = sqrt(std::inner_product(recop.begin(),
					   recop.end(),
					   recop.begin(),
					   0.0));
      double cosine = (nu.px * recop.at(0)
		       + nu.py * recop.at(1)
		       + nu.pz * recop.at(2));
      cosine /= (nu.momentum * mag);
      //if(p.reco_dir_z < 0) cosine = -1*cosine;
      v.push_back(acos(cosine) * (180/3.14159265));
    }
    this->add_vars(v);
  }
};
