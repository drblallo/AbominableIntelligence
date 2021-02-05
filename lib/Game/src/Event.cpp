#include "AI/Game/Event.hpp"

#include "AI/Game/Character.hpp"

using namespace std;
using namespace AI;

#define CEVENT(name, check, action)                                            \
	CharacterEvent name(                                                         \
			"name", [](const Map& map, const Character& character) check, [          \
			](Map & map, Character & character) action);

#define LEVENT(name, check, action)                                            \
	LocationEvent name(                                                          \
			"name", [](const Map& map, const Location& location) check, [            \
			](Map & map, Location & location) action);

#define GEVENT(name, check, action)                                            \
	GlobalEvent name("name", [](const Map& map) check, [](Map & map) action);

static GEVENT(
		Sanguinala,
		{ return map.getCurrentDay() % 1000 == 982 and map.getDice().roll() < 3; },
		{
			map.getVox().putMessage(
					"Serfs and lieges alike unite in a pray of deliverance, as the day "
					"of Sanguinius binds mankind together.");
		});

static LEVENT(
		MindUnbound,
		{ return map.getDice().roll() < 2 and map.getDice().roll() < 2; },
		{
			map.emplace<CharacterKind::RoguePsyker>(
					location,
					"Tim The Sorcerer",
					Statblock{ 25, 25, 30, 25, 35, 20, 10 });

			map.getVox().putMessage(
					"Attention Citizens: Inhuman behaviours reported at " +
					location.getName() +
					". Do not approach. Trust in the Emperor's will.");
		});
