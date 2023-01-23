#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <map>
#include "neutrino.h"
#include "interaction.h"
#include "imatch.h"
#include "pmatch.h"
#include "crthit.h"
#include "fmatch.h"
#include "csv_parser.h"

class Event
{
 public:
  Event();
  void add_neutrino(const Neutrino& nu);
  void add_interaction(const Interaction& in);
  void add_reco_interaction(const Interaction& in);
  void add_match(const IMatch& ma, bool ptt);
  void add_pmatch(const PMatch& ma, bool ptt);
  void add_crthit(const CRTHit& c);
  void add_fmatch(const FMatch& f);
  void generate_pointers();
  int32_t image_index;
  int16_t triggering_volume;
  std::vector<Neutrino> neutrinos;
  std::vector<Interaction> interactions;
  std::vector<Interaction> reco_interactions;
  std::vector<IMatch> matches_ptt;
  std::vector<IMatch> matches_ttp;
  std::vector<PMatch> pmatches_ptt;
  std::vector<PMatch> pmatches_ttp;
  std::vector<CRTHit> crthits;
  std::vector<FMatch> fmatches;
  std::map<uint16_t, size_t> interaction_map;
  std::map<uint16_t, size_t> reco_interaction_map;
  std::map<uint16_t, std::pair<size_t, size_t>> particle_map;
  std::map<uint16_t, std::pair<size_t, size_t>> reco_particle_map;
};
#endif
