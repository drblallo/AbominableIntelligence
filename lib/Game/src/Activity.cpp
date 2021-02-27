#include "AI/Game/Activity.hpp"

#include "AI/Game/Character.hpp"
#include "AI/Game/Map.hpp"
#include "AI/Game/Population.hpp"

using namespace std;
using namespace AI;

static constexpr Statblock::stat_type intelligenceToInfluenceDivisor = 20;

string AI::activityToString(Activity a)
{
	switch (a)
	{
		case Activity::None:
			return "None";
		case Activity::ExtendInfluence:
			return "Extend Influence";
		case Activity::ReduceInfluence:
			return "Reduce Influence";
		case Activity::BidForOwership:
			return "Bid For Owership";
		case Activity::Infect:
			return "Infect";
	}
	assert(false and "Unrechable");
	return "ERROR";
}

template<>
void AI::resolveActivity<Activity::ExtendInfluence>(Map&, Character& character)
{
	float increase =
			character.get<intelligence>() / intelligenceToInfluenceDivisor;
	increase *=
			1 - character.get<requisition>() / maxRequisition(character.getKind());
	increase = increase > 0 ? increase : 0;
	float req = character.get<requisition>() + increase;
	character.set<requisition>(req);
}

template<>
void AI::resolveActivity<Activity::ReduceInfluence>(
		Map& map, Character& character)
{
	float percentageDecrease = character.get<fellowship>() / 10;
	auto chars = map.charactersOfLocation(map.getLocationOf(character));
	for (Character* character2 : chars)
	{
		if (character.getID() == character2->getID())
			continue;
		auto newInfluece =
				(1 - (percentageDecrease / 100)) * character2->get<requisition>();
		character2->set<requisition>(newInfluece < 0 ? 0 : newInfluece);
	}
}

template<>
void AI::resolveActivity<Activity::BidForOwership>(
		Map& map, Character& character)
{
	if (map.getOwnershipOf(character.getID()).isValid())
		return;

	auto diceRes = map.roll();
	auto* comparedTo = map.getOwnerOf(map.getLocationOf(character));
	if (not comparedTo)
		return;

	auto defenderAdvantage =
			comparedTo->get<requisition>() - character.get<requisition>();

	auto defenderTarget = 50 + defenderAdvantage;
	if (diceRes >= defenderTarget)
	{
		character.set<requisition>(character.get<requisition>() / 4);
	}
	else
	{
		comparedTo->set<requisition>(comparedTo->get<requisition>() / 4);
		map.setOwnership(character, map.getLocationOf(character));
	}
}

template<>
void AI::resolveActivity<Activity::Infect>(Map& map, Character& character)
{
	auto& location = map.getLocationOf(character);

	double ovopositorStelaers = location.getPopulation(PopKind::genestealer) + 1;
	location.addPopulation(PopKind::BroodBrohter, ovopositorStelaers);
}
