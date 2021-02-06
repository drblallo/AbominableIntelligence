#include "AI/Game/Actions.hpp"

#include "AI/Game/Activity.hpp"
#include "AI/Game/Character.hpp"
#include "AI/Game/Location.hpp"
#include "AI/Game/Population.hpp"
#include "AI/Utils/Indent.hpp"

using namespace AI;
using namespace std;

void AI::getStat(
		const Map& map, std::ostream& OS, CharacterID id, int statNumber)
{
	OS << id << ", " << map.getCharacter(id).getStats()[statNumber] << "\n";
}

static void getPopImpl(
		const Map&, std::ostream& OS, const Location& loc, size_t popIndex)
{
	const auto& pop = loc.getPopulation()[popIndex];
	OS << loc.getName() << "-" << kindToName(pop.kind) << ", " << pop.quantity
		 << "\n";
}

void AI::getPop(
		const Map& map,
		std::ostream& OS,
		size_t element,
		size_t location,
		size_t popIndex)
{
	const auto& loc = map.getLocation(element, location);
	if (loc.getPopulation().size() <= popIndex)
	{
		OS << "No population with index " << popIndex << " in " << loc.getName()
			 << " only " << loc.getPopulation().size() << "\n";
		return;
	}
	getPopImpl(map, OS, loc, popIndex);
}
void AI::getPops(
		const Map& map, std::ostream& OS, size_t element, size_t location)
{
	const auto& loc = map.getLocation(element, location);
	for (size_t i = 0; i < loc.getPopulation().size(); i++)
		getPopImpl(map, OS, loc, i);
}

void AI::showCaracters(const Map& map, std::ostream& OS)
{
	for (const auto& character : map.getCharactersRange())
		character.print(OS);
}

void AI::showMapElements(const Map& map, std::ostream& OS)
{
	for (const auto& mapElement : map)
		mapElement.first.print(OS);
}

void AI::showLocations(const Map& map, std::ostream& OS)
{
	for (const auto& element : map)
	{
		OS << element.first.getName() << "\n";
		for (const auto& location : element.first)
		{
			OS << "\t" << location.getName() << "\n";
		}
	}
}
void AI::exit(const Map&, std::ostream&) { std::exit(0); }
void AI::showCaracter(const Map& map, std::ostream& OS, CharacterID id)
{
	if (map.charactersSize() <= id)
	{
		OS << "No character with ID: " << id << "\n";
		return;
	}

	const Character& character = map.getCharacter(id);
	character.print(OS);

	const auto& location = map.getLocationOf(id);
	OS << "Location: " << location.getName() << '['
		 << location.getKey().MaplElementID << "," << location.getKey().LocationID
		 << "]\n";

	auto ownershipKey = map.getOwnershipOf(id);
	if (ownershipKey.isValid())
	{
		auto ownership = map[ownershipKey];
		OS << "Owning: " << ownership.getName() << '['
			 << ownership.getKey().MaplElementID << ","
			 << ownership.getKey().LocationID << "]\n";
	}

	if (const auto* sup = map.getSuperiorOf(character); sup)
	{
		OS << "Superior: \n";
		sup->print(OS, 1);
	}
	auto infer = map.getInferiorsOf(character);
	if (not infer.empty())
		OS << "Inferiors: \n";
	for (const auto* inferior : infer)
		inferior->print(OS, 1);
}

static constexpr size_t largeTickInterval = 10;

static void updateActivity(Map& map, Character& character)
{
	switch (character.getActivity())
	{
		case Activity::None:
			return;
		case Activity::BidForOwership:
			resolveActivity<Activity::BidForOwership>(map, character);
			return;
		case Activity::ExtendInfluence:
			resolveActivity<Activity::ExtendInfluence>(map, character);
			return;
		case Activity::ReduceInfluence:
			resolveActivity<Activity::ReduceInfluence>(map, character);
			return;
	}
}

static void updateChar(Map& map, Character& character)
{
	if (character.get<requisition>() >
					maxRequisition(character.getKind()) - 0.1 or
			character.get<fellowship>() >= character.get<intelligence>())
		character.setActivity(Activity::ReduceInfluence);
	else if (auto* owner = map.getOwnerOf(map.getLocationOf(character));
					 owner != nullptr and
					 owner->get<requisition>() < character.get<requisition>() - 5)
		character.setActivity(Activity::BidForOwership);
	else
		character.setActivity(Activity::ExtendInfluence);
}

static void updateDay(Map& map) { map.nextDay(); }

static double growRate(const Map&, const Location& location, PopKind pop)
{
	auto quantity = location.getPopulation(pop);
	switch (pop)
	{
		case PopKind::spaceMarine:
			return 0;
		case PopKind::mechanicum:
			return quantity * 0.0000002;
		case PopKind::human:
			return quantity * 0.000002;
		case PopKind::guards:
			return quantity * 0.000001;
		case PopKind::genestealer:
			return location.getPopulation(PopKind::genestealer4) * 0.001;
		case PopKind::genestealer2:
			return location.getPopulation(PopKind::BroodBrohter) * 0.01;
		case PopKind::genestealer3:
			return location.getPopulation(PopKind::genestealer2) * 0.005;
		case PopKind::genestealer4:
			return location.getPopulation(PopKind::genestealer3) * 0.003;
		case PopKind::BroodBrohter:
			return location.getPopulation(PopKind::genestealer) * 0.1;
		case AI::PopKind::END:
			return 0;
	}

	return 0;
}

static void updatePopulation(const Map& map, Location& loc)
{
	for (size_t i = 0; i < static_cast<size_t>(PopKind::END); i++)
	{
		PopKind kind = static_cast<PopKind>(i);
		loc.addPopulation(kind, growRate(map, loc, kind));
	}
}

static void updateLocation(const Map& map, Location& location)
{
	updatePopulation(map, location);
}

static void updateMonth(Map& map)
{
	for (auto& mapElement : map)
		for (auto& location : mapElement.first)
			updateLocation(map, location);

	for (auto& character : map.getCharactersRange())
		updateChar(map, character);

	for (auto& character : map.getCharactersRange())
		updateActivity(map, character);
}

void AI::nextDay(Map& map, std::ostream&)
{
	updateDay(map);
	if (map.getCurrentDay() % largeTickInterval == 0)
		updateMonth(map);
}

void AI::skipDays(Map& map, std::ostream& OS, int ticks)
{
	for (int i = 0; i < ticks; i++)
		nextDay(map, OS);
}
