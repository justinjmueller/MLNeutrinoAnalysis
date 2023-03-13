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
  : true_not_reco(false),
    image_index(std::stoi(row[0])),
    volume(std::stoi(row[1])),
    interaction_index(std::stoi(row[2])),
    nu_id(std::stoi(row[3])),
    particle_string(row[4]),
    primary_string(row[5]),
    particle_multiplicity(std::vector<uint16_t>(5,0)),
    primary_multiplicity(std::vector<uint16_t>(5,0)),
    michel_present(row[6] == "True"),
    t0(std::stod(row[8])),
    contained(true),
    vis_energy(0),
    reco_vis_energy(0),
    voxels(0)
{
  std::string vertex(row[7]);
  if(vertex != "-1")
  {
    std::vector<std::string> v0(split(vertex.substr(1,vertex.length()-2), ' '));
    vertex_x = std::stod(v0.at(0));
    vertex_y = std::stod(v0.at(1));
    vertex_z = std::stod(v0.at(2));
  }
  else
  {
    vertex_x = 0;
    vertex_y = 0;
    vertex_z = 0;
  }

  parse_particle_string(particle_string, particle_multiplicity);
  parse_particle_string(primary_string, primary_multiplicity);

  std::cerr << row[9] << std::endl;
  std::string ext(row[9]);
  if(ext != "0" && ext != "-1" && ext != "1")
  {
    std::vector<std::string> v0(split(ext.substr(1,ext.length()-2), ' '));
    tpc_ext_x0 = std::stod(v0.at(0));//drift_adjust(std::stod(v0.at(0)), flash_time);
    tpc_ext_x1 = std::stod(v0.at(1));//drift_adjust(std::stod(v0.at(1)), flash_time);

    ext = row[10];
    v0 = split(ext.substr(1,ext.length()-2), ' ');
    tpc_ext_y0 = std::stod(v0.at(0));
    tpc_ext_y1 = std::stod(v0.at(1));

    ext = row[11];
    v0 = split(ext.substr(1,ext.length()-2), ' ');
    tpc_ext_z0 = std::stod(v0.at(0));
    tpc_ext_z1 = std::stod(v0.at(1));
  }
}

Interaction::Interaction()
  : true_not_reco(false),
    image_index(-1),
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
    voxels(0),
    tpc_ext_x0(-1),
    tpc_ext_x1(-1),
    tpc_ext_y0(-1),
    tpc_ext_y1(-1),
    tpc_ext_z0(-1),
    tpc_ext_z1(-1) { }

void Interaction::add_particle(const Particle& p)
{
  particles.push_back(p);
  particles.back().parent_primaries = this->primary_string;
  particles.back().parent_particles = this->particle_string;
  contained = (contained && p.contained);
  if(p.pid == 2)
  {
    vis_energy += (MUON_MASS + p.energy_dep);
    reco_vis_energy += (MUON_MASS + p.range_reco_energy);
  }
  else if(p.pid == 3)
  {
    vis_energy += (PION_MASS + p.energy_dep);
    reco_vis_energy += (PION_MASS + p.range_reco_energy);
  }
  else if(p.pid == 4)
  {
    vis_energy += p.energy_dep;
    reco_vis_energy += p.range_reco_energy;
  }
  voxels += p.voxels;
}
