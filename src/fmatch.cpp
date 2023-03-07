#include "csv_parser.h"
#include "utilities.h"
#include "fmatch.h"

FMatch::FMatch(const CSVRow& row)
: image_index(std::stoi(row[0])),
  volume(std::stoi(row[1])),
  interaction_index(std::stoi(row[2])),
  flash_id(std::stoi(row[3])),
  score(std::stod(row[4])),
  totalpe(std::stod(row[5])),
  totalpe0(std::stod(row[6])),
  totalpe1(std::stod(row[7])),
  tpc_x(std::stod(row[8])),
  tpc_x_err(std::stod(row[9])),
  tpc_y(std::stod(row[10])),
  tpc_y_err(std::stod(row[11])),
  tpc_z(std::stod(row[12])),
  tpc_z_err(std::stod(row[13])),
  flash_time(std::stod(row[14])),
  flash_time_width(std::stod(row[15])),
  flash_abs_time(std::stod(row[16])),
  flash_frame(std::stod(row[17])),
  flash_center_x(std::stod(row[18])),
  flash_width_x(std::stod(row[19])),
  flash_center_y(std::stod(row[20])),
  flash_width_y(std::stod(row[21])),
  flash_center_z(std::stod(row[22])),
  flash_width_z(std::stod(row[23])),
  flash_fast_to_total(std::stod(row[24])),
  flash_in_beam_frame(row[25] == "True"),
  flash_on_beam_time(std::stoi(row[26]))
{
    std::string ext(row[30]);
    std::vector<std::string> v0(split(ext.substr(1,ext.length()-2), ' '));
    tpc_ext_x0 = drift_adjust(std::stod(v0.at(0)), flash_time);
    tpc_ext_x1 = drift_adjust(std::stod(v0.at(1)), flash_time);

    ext = row[31];
    v0 = split(ext.substr(1,ext.length()-2), ' ');
    tpc_ext_y0 = std::stod(v0.at(0));
    tpc_ext_y1 = std::stod(v0.at(1));

    ext = row[32];
    v0 = split(ext.substr(1,ext.length()-2), ' ');
    tpc_ext_z0 = std::stod(v0.at(0));
    tpc_ext_z1 = std::stod(v0.at(1));
}

FMatch::FMatch()
: image_index(-1),
  volume(-1),
  interaction_index(-1),
  flash_id(-1),
  score(-1),
  totalpe(-1),
  totalpe0(-1),
  totalpe1(-1),
  tpc_x(-1),
  tpc_x_err(-1),
  tpc_y(-1),
  tpc_y_err(-1),
  tpc_z(-1),
  tpc_z_err(-1),
  flash_time(-1),
  flash_time_width(-1),
  flash_abs_time(-1),
  flash_frame(-1),
  flash_center_x(-1),
  flash_width_x(-1),
  flash_center_y(-1),
  flash_width_y(-1),
  flash_center_z(-1),
  flash_width_z(-1),
  flash_fast_to_total(-1),
  flash_in_beam_frame(false),
  flash_on_beam_time(-1),
  tpc_ext_x0(-1),
  tpc_ext_x1(-1),
  tpc_ext_y0(-1),
  tpc_ext_y1(-1),
  tpc_ext_z0(-1),
  tpc_ext_z1(-1) { }
