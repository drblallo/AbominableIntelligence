#pragma once

#include <string>

#include "AI/Game/Location.hpp"
#include "AI/Game/Map.hpp"
#include "AI/Game/MapElement.hpp"

namespace AI
{
	constexpr Pop::size_type UnderHiveDefaultPopulation = 8000000000;
	Location underHive(
			std::string name = "Under Hive",
			Pop::size_type humanPop = UnderHiveDefaultPopulation);

	constexpr Pop::size_type HighHiveDefaultPopulation = 1000000000;
	Location hiveSpire(
			std::string name = "High spires",
			Pop::size_type humanPop = HighHiveDefaultPopulation);

	constexpr Pop::size_type PDFBarracksDefaultGuardPopulation = 1000000;
	constexpr Pop::size_type PDFBarracksDefaultPopulation = 4;
	Location pdfBarracks(
			std::string name = "Planetary Defence Force Barraks",
			Pop::size_type guardPopulation = PDFBarracksDefaultGuardPopulation,
			Pop::size_type humanPop = PDFBarracksDefaultPopulation);

	constexpr Pop::size_type CathedralAveragePopulation = 100000;
	Location cathedral(
			std::string name = "Cathedral",
			Pop::size_type humanPop = CathedralAveragePopulation);

	MapElement kaneis();

	// TODO
	MapElement ulisse();

	Map inwardSuns(std::seed_seq& seed);

};	// namespace AI
