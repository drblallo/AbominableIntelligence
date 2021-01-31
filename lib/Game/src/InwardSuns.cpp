#include "AI/Game/InwardSuns.hpp"

#include "AI/Game/Character.hpp"
#include "AI/Game/Location.hpp"
#include "AI/Game/MapElement.hpp"
#include "AI/Game/Population.hpp"

using namespace AI;
using namespace std;

Location AI::underHive(string name, Pop::size_type humanPop)
{
	return Location(move(name), Pop::human(humanPop));
}

Location AI::hiveSpire(string name, Pop::size_type humanPop)
{
	return Location(move(name), Pop::human(humanPop));
}

Location AI::pdfBarracks(
		string name, Pop::size_type guardPopulation, Pop::size_type humanPop)
{
	return Location(
			move(name), Pop::guards(guardPopulation), Pop::human(humanPop));
}

Location AI::cathedral(string name, Pop::size_type humanPop)
{
	return Location(move(name), Pop::human(humanPop));
}

MapElement AI::kaneis()
{
	return MapElement(
			"Kaneis",
			{ underHive(),
				hiveSpire("Stargrazing Spires"),
				pdfBarracks(),
				cathedral("Saint Neminem Cathedral") });
}

Map AI::inwardSuns()
{
	using C = CharacterKind;
	Map map;
	const auto& location = map.getMapElement(map.addMapElement(kaneis()));
	map.emplaceOwner<C::GangLord>(location[0], "Ollander");
	map.emplace<C::GangLord>(location[0], "Red Stick");

	CharacterID Vincent =
			map.emplaceOwner<C::PlanetaryGovernor>(location[1], "Vincent Roh VII");
	CharacterID Alexander =
			map.emplace<C::LocalNobility>(location[1], "Alexander Roh");
	map.setSuperrior(Alexander, Vincent);

	CharacterID Sebastian =
			map.emplaceOwner<C::PDFGeneral>(location[2], "Sebastian Nimoy");
	CharacterID Federick =
			map.emplace<C::PDFColonnell>(location[2], "Federick Asseir");
	map.setSuperrior(Federick, Sebastian);
	map.setSuperrior(Sebastian, Vincent);

	CharacterID Henne = map.emplaceOwner<C::Bishop>(location[3], "Henne Vogel");
	CharacterID Lolerey =
			map.emplace<C::SisterOfBattleBadess>(location[3], "Lolerey Pathos");
	map.setSuperrior(Lolerey, Henne);

	return map;
}
