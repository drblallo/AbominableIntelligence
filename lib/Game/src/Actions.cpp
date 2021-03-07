#include "AI/Game/Actions.hpp"

#include <ostream>

#include "AI/Game/Activity.hpp"
#include "AI/Game/Character.hpp"
#include "AI/Game/Location.hpp"
#include "AI/Game/Map.hpp"
#include "AI/Game/Population.hpp"
#include "AI/Utils/Indent.hpp"

using namespace AI;
using namespace std;

void AI::getStat(
		const Map& map, std::ostream& OS, CharacterID id, int statNumber)
{
	if (statNumber > Stat::requisition or statNumber < 0)
	{
		OS << "No stat with index " << statNumber << "\n";
		return;
	}
	if (map.getCharacterCount() <= id)
	{
		OS << "No character with ID " << id << "\n";
		return;
	}
	const auto& character = map.getCharacter(id);
	OS << character.getName() << "-"
		 << statToString(static_cast<Stat>(statNumber)) << ", "
		 << character.getStats()[statNumber] << "\n";
}

static void getPopImpl(
		const Map&, std::ostream& OS, const Location& loc, size_t popIndex)
{
	const auto& pop = loc.getPopulation()[popIndex];
	OS << loc.getName() << "-" << kindToName(pop.kind) << ", " << pop.quantity
		 << "\n";
}

static const Location* safeGetLocation(
		const Map& map, ostream& OS, size_t element, size_t location)
{
	if (map.mapElementsCount() <= element)
	{
		OS << "No map element with index " << element << "\n";
		return nullptr;
	}
	const auto& mapElement = map.getMapElement(element);
	if (mapElement.size() <= location)
	{
		OS << "No location with index " << element << "\n";
		return nullptr;
	}
	return &mapElement[location];
}

static Character* safeGetCharacter(
		Map& map, ostream& OS, CharacterID characterID)
{
	if (map.charactersSize() <= characterID)
	{
		OS << "No character with ID " << characterID << "\n";
		return nullptr;
	}
	return &map.getCharacter(characterID);
}

static Character* safeGetPatriarch(
		Map& map, ostream& OS, CharacterID characterID)
{
	auto* res = safeGetCharacter(map, OS, characterID);
	if (res and not res->isA(CharacterKind::GenestealerPatriarch))
	{
		OS << "Character with ID " << characterID << " is not a patriarch\n";
		return nullptr;
	}
	return res;
}

void AI::getPop(
		const Map& map,
		std::ostream& OS,
		size_t element,
		size_t location,
		size_t popIndex)
{
	const auto* loc = safeGetLocation(map, OS, element, location);
	if (loc == nullptr)
		return;
	if (loc->getPopulation().size() <= popIndex)
	{
		OS << "No population with index " << popIndex << " in " << loc->getName()
			 << " only " << loc->getPopulation().size() << "\n";
		return;
	}
	getPopImpl(map, OS, *loc, popIndex);
}
void AI::getPops(
		const Map& map, std::ostream& OS, size_t element, size_t location)
{
	const auto* loc = safeGetLocation(map, OS, element, location);
	if (loc == nullptr)
		return;

	for (size_t i = 0; i < loc->getPopulation().size(); i++)
		getPopImpl(map, OS, *loc, i);
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
		case Activity::Infect:
			resolveActivity<Activity::Infect>(map, character);
			return;
		case Activity::Attack:
			resolveActivity<Activity::Attack>(map, character);
			return;
	}
}

static void updateChar(Map& map, Character& character)
{
	if (character.isA(CharacterKind::GenestealerPatriarch))
		return;

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
			return location.getPopulation(PopKind::genestealer4) * 0.0001;
		case PopKind::genestealer2:
			return location.getPopulation(PopKind::BroodBrohter) * 0.001;
		case PopKind::genestealer3:
			return location.getPopulation(PopKind::genestealer2) * 0.0005;
		case PopKind::genestealer4:
			return location.getPopulation(PopKind::genestealer3) * 0.0003;
		case PopKind::BroodBrohter:
			return 0;
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

static void patriarchMaybeSetAction(
		Map& map, std::ostream& OS, CharacterID characterID, Activity activity)
{
	if (auto* patriarch = safeGetPatriarch(map, OS, characterID); patriarch)
		patriarch->setActivity(activity);
}

void AI::infect(Map& map, std::ostream& OS, CharacterID characterID)
{
	patriarchMaybeSetAction(map, OS, characterID, Activity::Infect);
}

void AI::attack(Map& map, std::ostream& OS, CharacterID characterID)
{
	patriarchMaybeSetAction(map, OS, characterID, Activity::Attack);
}
