#include "AI/Game/Character.hpp"

#include <iostream>
#include <ostream>
#include <string_view>

#include "AI/Utils/Indent.hpp"

using namespace AI;
using namespace std;

string_view AI::kindToName(CharacterKind k)
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
		case CharacterKind::RoguePsyker:
			return "Rogue Psyker";
	}
	assert(false and "Unrechable");
	return "Unreachable";
}

void Statblock::dump() const { print(cout); }

string AI::statToString(Stat s)
{
	switch (s)
	{
		case strenght:
			return "S";
		case toughness:
			return "T";
		case agility:
			return "A";
		case perception:
			return "P";
		case intelligence:
			return "I";
		case willpower:
			return "W";
		case fellowship:
			return "F";
		case requisition:
			return "R";
	}
	assert(false and "Unrechable");
	return "ERROR";
}

void Statblock::print(ostream& OS, size_t indents) const
{
	indent(OS, indents);
	OS << '[';
	for (size_t i = 0; i < StatCount - 1; i++)
		OS << stats[i] << statToString(static_cast<Stat>(i)) << " ";
	OS << stats[StatCount - 1] << statToString(static_cast<Stat>(StatCount - 1))
		 << "]\n";
}

void Character::print(std::ostream& OS, size_t indents) const
{
	indent(OS, indents);
	OS << '[' << getID() << "] " << kindToName(getKind()) << ": " << getName()
		 << " (" << activityToString(activity) << ")\n";
	stats.print(OS, 0);
}

void Character::dump() const { print(cout); }
