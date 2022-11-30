#include "csv_parser.h"
#include "pmatch.h"

PMatch::PMatch(const CSVRow& row)
  : image_index(std::stoi(row[0])),
    volume(std::stoi(row[1])),
    from_index(std::stoi(row[2])),
    from_pid(std::stoi(row[3])),
    from_primary(row[4] == "True"),
    to_index(std::stoi(row[5])),
    to_pid(std::stoi(row[6])),
    to_primary(row[7] == "True") {}

PMatch::PMatch()
  : image_index(-1),
    volume(-1),
    from_index(-1),
    from_pid(-1),
    from_primary(false),
    to_index(-1),
    to_pid(-1),
    to_primary(false) {}
