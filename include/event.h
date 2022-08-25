#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include "neutrino.h"
#include "interaction.h"
#include "imatch.h"
#include "csv_parser.h"

class Event
{
 public:
  Event();
  void add_neutrino(const Neutrino& nu);
  void add_interaction(const Interaction& in);
  void add_reco_interaction(const Interaction& in);
  void add_match(const IMatch& ma, bool ptt);
  int32_t image_index;
  std::vector<Neutrino> neutrinos;
  std::vector<Interaction> interactions;
  std::vector<Interaction> reco_interactions;
  std::vector<IMatch> matches_ptt;
  std::vector<IMatch> matches_ttp;
};
#endif
