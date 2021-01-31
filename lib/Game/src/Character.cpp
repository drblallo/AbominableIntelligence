#include "AI/Game/Character.hpp"

#include <iostream>
#include <string_view>

#include "AI/Utils/Indent.hpp"

using namespace AI;
using namespace std;

std::string_view AI::kindToName(CharacterKind k)
{
	switch (k)
	{
		case CharacterKind::FeudalLord:
			return "Feudal Lord";
		case CharacterKind::PlanetaryGovernor:
			return "Planetary Governor";
		case CharacterKind::LocalNobility:
			return "Local Nobility";
		case CharacterKind::Inquisitor:
			return "Inquisitor";
		case CharacterKind::SpaceMarineCaptain:
			return "Space Marine Captain";
		case CharacterKind::SpaceMarineChapterMaster:
			return "Space Marine Chapter Master";
		case CharacterKind::ImperialGuardCommander:
			return "Imperial Guard Commander";
		case CharacterKind::PDFGeneral:
			return "PDF General";
		case CharacterKind::PDFColonnell:
			return "PDF Colonnell";
		case CharacterKind::Bishop:
			return "Bishop";
		case CharacterKind::SisterOfBattleBadess:
			return "Sister Of Battle Badess";
		case CharacterKind::GangLord:
			return "Gang Lord";
		case CharacterKind::OrkWarboss:
			return "Ork Warboss";
		case CharacterKind::EldarAutharc:
			return "Eldar Autharc";
		case CharacterKind::EldarPirate:
			return "Eldar Pirate";
		case CharacterKind::GenestealerPatriarch:
			return "Genestealer Patriarc";
	}
	assert(false and "Unrechable");
	return "Unreachable";
}

void Character::print(std::ostream& OS, size_t indents) const
{
	indent(OS, indents);
	OS << '[' << getID() << "] " << kindToName(getKind()) << ": " << getName()
		 << "\n";
}

void Character::dump() const { print(std::cout); }
