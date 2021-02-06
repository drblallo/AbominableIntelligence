#include "AI/Game/Event.hpp"

#include "AI/Game/Character.hpp"

using namespace std;
using namespace AI;

#define CEVENT(name, check, action)                                            \
	CharacterEvent name(                                                         \
			"name",                                                                  \
			[]([[maybe_unused]] const Map& map,                                      \
				 [[maybe_unused]] const Character& character) check,                   \
			[]([[maybe_unused]] Map & map, [[maybe_unused]] Character & character)   \
					action);

#define LEVENT(name, check, action)                                            \
	LocationEvent name(                                                          \
			"name",                                                                  \
			[]([[maybe_unused]] const Map& map,                                      \
				 [[maybe_unused]] const Location& location) check,                     \
			[]([[maybe_unused]] Map & map, [[maybe_unused]] Location & location)     \
					action);

#define GEVENT(name, check, action)                                            \
	GlobalEvent name("name", []([[maybe_unused]] const Map& map) check, [        \
	]([[maybe_unused]] Map & map) action);

GEVENT(
		Sanguinala,
		{ return map.getCurrentDay() % 1000 == 982 and map.roll() < 3; },
		{
			map.getVox().putMessage(
					"Serfs and lieges alike unite in a pray of deliverance, as the day "
					"of Sanguinius binds mankind together.");
		});

LEVENT(
		MindUnbound,
		{ return map.roll() < 2 and map.roll() < 2; },
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
