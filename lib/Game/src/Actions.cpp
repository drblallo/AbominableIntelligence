#include "AI/Game/Actions.hpp"

#include "AI/Game/Character.hpp"
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
