#include <iostream>
#include <random>

#include "AI/Game/Actions.hpp"
#include "AI/Game/InwardSuns.hpp"
#include "AI/Game/Map.hpp"

using namespace AI;
using namespace std;

extern "C"
{
#include "AI/Bindings/Bindings.h"

	struct GameInfo
	{
		public:
		GameInfo(int seed): seed({ seed }), map(inwardSuns(this->seed)) {}

		const Map& getMap() const { return map; }
		Map& getMap() { return map; }

		private:
		seed_seq seed;
		Map map;
	};

	GameInfo* gameCreate(int seed) { return new GameInfo(seed); }
	void gameDestroy(GameInfo* info) { delete info; }
	void gameShowLocations(GameInfo* info)
	{
		showLocations(info->getMap(), cout);
	}

	void gameGetStat(GameInfo* info, size_t id, int statNumber)
	{
		getStat(info->getMap(), cout, id, statNumber);
	}

	void gameShowMapElements(GameInfo* info)
	{
		showMapElements(info->getMap(), cout);
	}
	void gameShowCaracter(GameInfo* info, size_t id)
	{
		showCaracter(info->getMap(), cout, id);
	}

	void gameExit(GameInfo* info) { exit(info->getMap(), cout); }
	void gameNextDay(GameInfo* info) { nextDay(info->getMap(), cout); }
	void gameSkipDays(GameInfo* info, int ticks)
	{
		skipDays(info->getMap(), cout, ticks);
	}
	void gameInfect(GameInfo* info, size_t characterID)
	{
		infect(info->getMap(), cout, characterID);
	}
	void gameGetPop(
			GameInfo* info, size_t element, size_t location, size_t popIndex)
	{
		getPop(info->getMap(), cout, element, location, popIndex);
	}

	void gameGetPops(GameInfo* info, size_t element, size_t location)
	{
		getPops(info->getMap(), cout, element, location);
	}
};
