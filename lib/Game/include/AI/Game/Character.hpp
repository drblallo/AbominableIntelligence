#pragma once

#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "AI/Game/MapElement.hpp"

namespace AI
{
	enum class CharacterKind
	{
		FeudalLord,
		PlanetaryGovernor,
		LocalNobility,
		Inquisitor,
		SpaceMarineCaptain,
		SpaceMarineChapterMaster,
		ImperialGuardCommander,
		PDFGeneral,
		PDFColonnell,
		Bishop,
		SisterOfBattleBadess,
		GangLord,
		OrkWarboss,
		EldarAutharc,
		EldarPirate,
		GenestealerPatriarch
	};

	std::string_view kindToName(CharacterKind name);

	using CharacterID = size_t;
	class Character
	{
		public:
		Character(CharacterID id, std::string name, CharacterKind kind)
				: id(id), name(std::move(name)), kind(kind)
		{
		}
		[[nodiscard]] const std::string& getName() const { return name; }
		[[nodiscard]] CharacterKind getKind() const { return kind; }
		[[nodiscard]] size_t getID() const { return id; }
		[[nodiscard]] bool isA(CharacterKind charKind) const
		{
			return charKind == kind;
		}

		void print(std::ostream& OS, size_t indents = 0) const;
		void dump() const;

		private:
		const CharacterID id;
		std::string name;
		CharacterKind kind;
	};
}	 // namespace AI
