#pragma once

#include <iostream>

#include "AI/Game/Map.hpp"

namespace AI
{
	void showCaracters(const Map& map, std::ostream& OS);
	void showLocations(const Map& map, std::ostream& OS);
	void showMapElements(const Map& map, std::ostream& OS);
	void showCaracter(const Map& map, std::ostream& OS, CharacterID id);
	void exit(const Map& map, std::ostream& OS);
}	 // namespace AI
