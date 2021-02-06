#pragma once

#include <iostream>

#include "AI/Game/Character.hpp"
#include "AI/Game/Map.hpp"

namespace AI
{
	void showCaracters(const Map& map, std::ostream& OS);
	void showLocations(const Map& map, std::ostream& OS);
	void getStat(
			const Map& map, std::ostream& OS, CharacterID id, int statNumber);
	void showMapElements(const Map& map, std::ostream& OS);
	void showCaracter(const Map& map, std::ostream& OS, CharacterID id);
	void exit(const Map& map, std::ostream& OS);
	void nextDay(Map& map, std::ostream& OS);
	void skipDays(Map& map, std::ostream& OS, int ticks);
	void getPop(
			const Map& map,
			std::ostream& OS,
			size_t element,
			size_t location,
			size_t popIndex);
	void getPops(
			const Map& map, std::ostream& OS, size_t element, size_t location);
}	 // namespace AI
