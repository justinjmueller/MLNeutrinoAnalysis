#include <string>
#include <vector>
#include "csv_parser.h"
#include "utilities.h"
#include "particle.h"

Particle::Particle(const CSVRow& row)
  : image_index(std::stoi(row[0])),
    particle_index(std::stoi(row[1])),
    interaction_index(std::stoi(row[2])),
    pid(std::stoi(row[3])),
    semantic(std::stoi(row[4])),
    primary(row[5] == "True"),
    contained(row[6] == "True"),
    cathode_crosser(row[7] == "True"),
    crosses_x(row[8] == "True"),
    crosses_y(row[9] == "True"),
    crosses_z(row[10] == "True"),
    voxels(std::stoi(row[11])),
    length(std::stod(row[12])),
    momentum(std::stod(row[13])),
    px(std::stod(row[14])),
    py(std::stod(row[15])),
    pz(std::stod(row[16])),
    energy_init(std::stod(row[17])),
    energy_dep(std::stod(row[18])),
    sed(std::stod(row[19])),
    reco_energy(std::stod(row[20])),
    theta(std::stod(row[21])),
    reco_theta(std::stod(row[22])),
    nchildren(std::stoi(row[24])),
    p0_localQ_lowest(row[27] == "True"),
    expa(std::stod(row[28])),
    expb(std::stod(row[29]))
{
  std::string dir(row[25]);
  std::vector<std::string> sdir(split(dir.substr(1,dir.length()-2), ' '));
  reco_dir_x = std::stod(sdir.at(0));
  reco_dir_y = std::stod(sdir.at(1));
  reco_dir_z = std::stod(sdir.at(2));

  std::string vtx(row[30]);
  std::vector<std::string> svtx(split(vtx.substr(1,vtx.length()-2), ' '));
  vtx0x = std::stod(svtx.at(0));
  vtx0y = std::stod(svtx.at(1));
  vtx0z = std::stod(svtx.at(2));

  vtx = row[31];
  svtx = split(vtx.substr(1,vtx.length()-2), ' ');
  vtx1x = std::stod(svtx.at(0));
  vtx1y = std::stod(svtx.at(1));
  vtx1z = std::stod(svtx.at(2));
  
  parent_primaries = "None";
  parent_particles = "None";
}

Particle::Particle()
  : image_index(-1),
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
    energy_init(-1),
    energy_dep(-1),
    sed(-1),
    reco_energy(-1),
    theta(-1),
    reco_theta(-1),
    nchildren(-1),
    reco_dir_x(-1),
    reco_dir_y(-1),
    reco_dir_z(-1),
    p0_localQ_lowest(false),
    expa(-1),
    expb(-1),
    vtx0x(0.),
    vtx0y(0.),
    vtx0z(0.),
    vtx1x(0.),
    vtx1y(0.),
    vtx1z(0.)
{
  parent_primaries = "None";
  parent_particles = "None";
}
