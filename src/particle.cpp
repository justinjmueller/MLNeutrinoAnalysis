#include <string>
#include <vector>
#include "csv_parser.h"
#include "utilities.h"
#include "particle.h"

Particle::Particle(const CSVRow& row)
  : true_not_reco(false),
    image_index(std::stoi(row[0])),
    volume(std::stoi(row[1])),
    particle_index(std::stoi(row[2])),
    interaction_index(std::stoi(row[3])),
    pid(std::stoi(row[4])),
    semantic(std::stoi(row[5])),
    primary(row[6] == "True"),
    contained(row[7] == "True"),
    cathode_crosser(row[8] == "True"),
    crosses_x(row[9] == "True"),
    crosses_y(row[10] == "True"),
    crosses_z(row[11] == "True"),
    voxels(std::stoi(row[12])),
    length(std::stod(row[13])),
    momentum(std::stod(row[14])),
    px(std::stod(row[15])),
    py(std::stod(row[16])),
    pz(std::stod(row[17])),
    t(std::stod(row[18])),
    energy_init(std::stod(row[19])),
    energy_dep(std::stod(row[20])),
    sed(std::stod(row[21])),
    range_reco_energy(std::stod(row[22])),
    calo_energy(std::stod(row[23])),
    theta(std::stod(row[24])),
    reco_theta(std::stod(row[25])),
    nchildren(std::stoi(row[26])),
    parent_pdg(std::stoi(row[31])),
    parent_tid(std::stoi(row[32])),
    softmax_nonprimary(std::stod(row[33])),
    softmax_primary(std::stod(row[34])),
    softmax_photon(std::stod(row[35])),
    softmax_electron(std::stod(row[36])),
    softmax_muon(std::stod(row[37])),
    softmax_pion(std::stod(row[38])),
    softmax_proton(std::stod(row[39]))
{
  std::string dir(row[27]);
  if(dir != "None")
  {
    std::vector<std::string> sdir(split(dir.substr(1,dir.length()-2), ' '));
    reco_dir_x = std::stod(sdir.at(0));
    reco_dir_y = std::stod(sdir.at(1));
    reco_dir_z = std::stod(sdir.at(2));
  }
  else
  {
    reco_dir_x = -1.0;
    reco_dir_y = -1.0;
    reco_dir_z = -1.0;
  }

  std::string vtx(row[29]);
  std::vector<std::string> svtx(split(vtx.substr(1,vtx.length()-2), ' '));
  if(vtx != "None")
  {
    vtx0x = std::stod(svtx.at(0));
    vtx0y = std::stod(svtx.at(1));
    vtx0z = std::stod(svtx.at(2));
  }
  else
  {
    vtx0x = 0.0;
    vtx0y = 0.0;
    vtx0z = 0.0;
  }

  vtx = row[30];
  if(vtx != "None")
  {
    svtx = split(vtx.substr(1,vtx.length()-2), ' ');
    vtx1x = std::stod(svtx.at(0));
    vtx1y = std::stod(svtx.at(1));
    vtx1z = std::stod(svtx.at(2));
  }
  else
  {
    vtx1x = 0.0;
    vtx1y = 0.0;
    vtx1z = 0.0;
  }
  
  parent_primaries = "None";
  parent_particles = "None";
}

Particle::Particle()
  : true_not_reco(false),
    image_index(-1),
    volume(-1),
    particle_index(-1),
    interaction_index(-1),
    pid(-1),
    semantic(-1),
    primary(false),
    contained(false),
    cathode_crosser(false),
    crosses_x(false),
    crosses_y(false),
    crosses_z(false),
    voxels(-1),
    length(-1),
    momentum(-1),
    px(-1),
    py(-1),
    pz(-1),
    t(-1),
    energy_init(-1),
    energy_dep(-1),
    sed(-1),
    range_reco_energy(-1),
    calo_energy(-1),
    theta(-1),
    reco_theta(-1),
    nchildren(-1),
    reco_dir_x(-1),
    reco_dir_y(-1),
    reco_dir_z(-1),
    vtx0x(0.),
    vtx0y(0.),
    vtx0z(0.),
    vtx1x(0.),
    vtx1y(0.),
    vtx1z(0.),
    parent_pdg(-1),
    parent_tid(-1),
    softmax_nonprimary(0),
    softmax_primary(0),
    softmax_photon(0),
    softmax_electron(0),
    softmax_muon(0),
    softmax_pion(0),
    softmax_proton(0)
{
  parent_primaries = "None";
  parent_particles = "None";
}
