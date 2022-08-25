#include <vector>
#include "neutrino.h"
#include "interaction.h"
#include "imatch.h"
#include "event.h"

Event::Event()
  : image_index(-1),
    neutrinos(std::vector<Neutrino>()),
    interactions(std::vector<Interaction>()),
    reco_interactions(std::vector<Interaction>()),
    matches_ptt(std::vector<IMatch>()),
    matches_ttp(std::vector<IMatch>()){ }

void Event::add_neutrino(const Neutrino& nu)
{
  neutrinos.push_back(nu);
  image_index = nu.image_index;
}

void Event::add_interaction(const Interaction& in)
{
  interactions.push_back(in);
}

void Event::add_reco_interaction(const Interaction& in)
{
  reco_interactions.push_back(in);
}

void Event::add_match(const IMatch& ma, bool ptt=true)
{
  if(ptt) matches_ptt.push_back(ma);
  else matches_ttp.push_back(ma);
}
