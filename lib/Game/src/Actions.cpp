#include "AI/Game/Actions.hpp"

#include "AI/Game/Character.hpp"
#include "AI/Game/Population.hpp"
#include "AI/Utils/Indent.hpp"

using namespace AI;
using namespace std;

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

static void updateDay(Map& map)
{
	map.nextDay();
	// for (auto& character : map.getCharactersRange())
	// character do something
}

static double growRate(PopKind kind)
{
	switch (kind)
	{
		case PopKind::spaceMarine:
			return 0;
		case PopKind::mechanicum:
			return 0.0000002;
		case PopKind::human:
			return 0.000002;
		case PopKind::guards:
			return 0.000001;
		case PopKind::genestealer:
			return 0;
	}

	return 0;
}

static void updatePopulation(const Map&, const Location&, Pop& pop)
{
	pop.quantity += pop.quantity * growRate(pop.kind);
}

static void updateLocation(const Map& map, Location& location)
{
	for (auto& pop : location)
		updatePopulation(map, location, pop);
}

static void updateMonth(Map& map)
{
	for (auto& mapElement : map)
		for (auto& location : mapElement.first)
			updateLocation(map, location);
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

	std::cout << "Day: " << map.getCurrentDay() << "\n";
}
