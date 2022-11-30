#include <string>
#include <vector>
#include <cctype>
#include "csv_parser.h"
#include "utilities.h"
#include "particle.h"
#include "interaction.h"

#define MUON_MASS 105.6583745
#define PION_MASS 139.57039
#define PROTON_MASS 938.2720813

Interaction::Interaction(const CSVRow& row)
  : image_index(std::stoi(row[0])),
    volume(std::stoi(row[1])),
    interaction_index(std::stoi(row[2])),
    nu_id(std::stoi(row[3])),
    particle_string(row[4]),
    primary_string(row[5]),
    particle_multiplicity(std::vector<uint16_t>(5,0)),
    primary_multiplicity(std::vector<uint16_t>(5,0)),
    michel_present(row[6] == "True"),
    t0(0),
    contained(true),
    vis_energy(0),
    reco_vis_energy(0),
    nu_index(9999),
    voxels(0)
{
  std::string vertex(row[7]);
  std::vector<std::string> v0(split(vertex.substr(1,vertex.length()-2), ' '));
  vertex_x = std::stod(v0.at(0));
  vertex_y = std::stod(v0.at(1));
  vertex_z = std::stod(v0.at(2));

  parse_particle_string(particle_string, particle_multiplicity);
  parse_particle_string(primary_string, primary_multiplicity);
}

Interaction::Interaction()
  : image_index(-1),
    volume(-1),
    interaction_index(-1),
    nu_id(-1),
    particle_string("None"),
    primary_string("None"),
    particle_multiplicity(std::vector<uint16_t>(5,0)),
    primary_multiplicity(std::vector<uint16_t>(5,0)),
    michel_present(false),
    vertex_x(-1),
    vertex_y(-1),
    vertex_z(-1),
    t0(0),
    contained(true),
    vis_energy(0),
    reco_vis_energy(0),
    nu_index(9999),
    voxels(0) { }

void Interaction::add_particle(const Particle& p)
{
  particles.push_back(p);
  particles.back().parent_primaries = this->primary_string;
  particles.back().parent_particles = this->particle_string;
  contained = (contained && p.contained);
  if(p.primary)
    t0 = (t0*(particles.size()-1) + p.t) / particles.size();
  if(p.pid == 2)
  {
    vis_energy += (MUON_MASS + p.energy_dep);
    reco_vis_energy += (MUON_MASS + p.reco_energy);
  }
  else if(p.pid == 3)
  {
    vis_energy += (PION_MASS + p.energy_dep);
    reco_vis_energy += (PION_MASS + p.reco_energy);
  }
  else if(p.pid == 4)
  {
    vis_energy += p.energy_dep;
    reco_vis_energy += p.reco_energy;
  }
  voxels += p.voxels;
}

void Interaction::update_particles(double thr)
{
  /*if(particles.size() > 0)
  {
    //bool condition;
    //for(auto& p : particles)
    //{
      condition = (p.primary || p.primary_heuristic);
      if(condition && !p.primary)
      {
	p.primary = true;
	primary_multiplicity.at(p.pid)++;
	std::cout << "updated primary id" << std::endl;
	}*/
      /*if(p.primary_heuristic && !p.primary)
      {
	p.primary = true;
	primary_multiplicity.at(p.pid)++;
      }
      else if(p.primary_heuristic && p.primary)
      {
	p.primary = false;
	primary_multiplicity.at(p.pid)--;
      }
      }*/
  /*bool has_primary(false);
    double longest(0);
    size_t longest_p(0), current_index(0);
    for(auto& p : particles)
    {
      has_primary = (has_primary || p.primary);
      if(p.length > longest)
      {
	longest = p.length;
	longest_p = current_index;
      }
      ++current_index;
    }
    if(!has_primary) particles.at(longest_p).primary = true;
    
    auto cluster = find_best_cluster(particles, thr);
    for(size_t i(0); i < particles.size(); ++i)
    {
      if(cluster.at(i) && particles.at(i).primary)
      {
	for(size_t j(0); j < particles.size(); ++j)
	{
	  if(!particles.at(j).primary && cluster.at(j))
	  {
	    primary_multiplicity.at(particles.at(j).pid)++;
	    particles.at(j).primary = true;
	  }
	}
	break;
      }
    }
  }*/
}
