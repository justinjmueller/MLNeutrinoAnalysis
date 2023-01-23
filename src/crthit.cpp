#include "csv_parser.h"
#include "crthit.h"

CRTHit::CRTHit(const CSVRow& row)
  : image_index(std::stoi(row[0])),
    peshit(std::stod(row[1])),
    ts0_s(std::stod(row[2])),
    ts0_corr(std::stod(row[3])),
    ts0_ns(std::stod(row[4])),
    ts0_ns_corr(std::stod(row[5])),
    ts1_ns(std::stod(row[6])),
    plane(std::stoi(row[7])),
    tagger(row[8]),
    xpos(std::stod(row[9])),
    xerr(std::stod(row[10])),
    ypos(std::stod(row[11])),
    yerr(std::stod(row[12])),
    zpos(std::stod(row[13])),
    zerr(std::stod(row[14])) { }

CRTHit::CRTHit()
  : image_index(-1),
    peshit(-1),
    ts0_s(-1),
    ts0_corr(-1),
    ts0_ns(-1),
    ts0_ns_corr(-1),
    ts1_ns(-1),
    plane(-1),
    tagger("None"),
    xpos(-1),
    xerr(-1),
    ypos(-1),
    yerr(-1),
    zpos(-1),
    zerr(-1) { }