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
    interaction_index(std::stoi(row[1])),
    nu_id(std::stoi(row[2])),
    particle_string(row[3]),
    primary_string(row[4]),
    particle_multiplicity(std::vector<uint16_t>(5,0)),
    primary_multiplicity(std::vector<uint16_t>(5,0)),
    michel_present(row[5] == "True"),
    contained(true),
    vis_energy(0),
    reco_vis_energy(0)
{
  std::string vertex(row[6]);
  std::vector<std::string> v0(split(vertex.substr(1,vertex.length()-2), ' '));
  vertex_x = std::stod(v0.at(0));
  vertex_y = std::stod(v0.at(1));
  vertex_z = std::stod(v0.at(2));

  parse_particle_string(particle_string, particle_multiplicity);
  parse_particle_string(primary_string, primary_multiplicity);
}

Interaction::Interaction()
  : image_index(-1),
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
    contained(true),
    vis_energy(0),
    reco_vis_energy(0) { }

void Interaction::add_particle(const Particle& p)
{
  particles.push_back(p);
  particles.back().parent_primaries = this->primary_string;
  particles.back().parent_particles = this->particle_string;
  contained = (contained && p.contained);
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
    vis_energy += (PROTON_MASS + p.energy_dep);
    reco_vis_energy += (PROTON_MASS + p.reco_energy);
  }
}

void Interaction::update_particles(double thr)
{
  if(particles.size() > 0)
  {
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
  }
}
