#pragma once

#include <stddef.h>

struct GameInfo;

GameInfo* gameCreate(int seed);
void gameDestroy(GameInfo* info);
void gameShowLocations(GameInfo* info);
void gameGetStat(GameInfo* info, size_t id, int statNumber);
void gameShowMapElements(GameInfo* info);
void gameShowCaracter(GameInfo* info, size_t id);
void gameExit(GameInfo* info);
void gameNextDay(GameInfo* info);
void gameSkipDays(GameInfo* info, int ticks);
void gameInfect(GameInfo* info, size_t characterID);
void gameGetPop(
		GameInfo* info, size_t element, size_t location, size_t popIndex);
void gameGetPops(GameInfo* info, size_t element, size_t location);
