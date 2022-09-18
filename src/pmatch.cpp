#include "csv_parser.h"
#include "pmatch.h"

PMatch::PMatch(const CSVRow& row)
  : image_index(std::stoi(row[0])),
    from_index(std::stoi(row[1])),
    from_pid(std::stoi(row[2])),
    from_primary(row[3] == "True"),
    to_index(std::stoi(row[4])),
    to_pid(std::stoi(row[5])),
    to_primary(row[6] == "True") {}

PMatch::PMatch()
  : image_index(-1),
    from_index(-1),
    from_pid(-1),
    from_primary(false),
    to_index(-1),
    to_pid(-1),
    to_primary(false) {}
