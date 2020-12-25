#include "AI/Warhammer/Weapon.hpp"

#include <bits/stdint-uintn.h>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <ostream>

#include "AI/Warhammer/AbstractStatBlock.hpp"

using namespace AI;
using namespace std;

void AI::print(ostream& OS, WeaponType stat) noexcept
{
	switch (stat)
	{
		case WeaponType::ASSAULT:
			OS << "Assault";
		case WeaponType::RAPID_FIRE:
			OS << "Rapid Fire";
		case WeaponType::GRANADE:
			OS << "Granade";
		case WeaponType::HEAVY:
			OS << "Heavy";
		case WeaponType::PISTOL:
			OS << "Pistol";
		case WeaponType::MELEE:
			OS << "Melee";
	}
}

template<>
void AbstractStatBlock<WeaponStat, uint8_t>::print(
		ostream& OS, WeaponStat stat, uint8_t value)
{
	switch (stat)
	{
		case WeaponStat::R:
			OS << "R:" << value;
			return;
		case WeaponStat::S:
			OS << "S:" << value;
			return;
		case WeaponStat::ROF:
			OS << "ROF:" << value;
			return;
		case WeaponStat::AP:
			OS << "AP:" << value;
			return;
		case WeaponStat::TYPE:
			OS << "TYPE:";
			AI::print(OS, static_cast<WeaponType>(value));
			return;
		case WeaponStat::LAST:
			OS << "ERROR";
			return;
	}
}

void AI::print(ostream& OS, WeaponSpecialRule rule) noexcept
{
	switch (rule)
	{
		case WeaponSpecialRule::NONE:
			OS << "NONE";
			return;
	}
}

void Weapon::print(ostream& OS, size_t indents) const
{
	stats.print(OS, indents);
	OS << " ";
	for (WeaponSpecialRule rule : specialRule)
	{
		AI::print(OS, rule);
		OS << " ";
	}
}

void Weapon::dump() const { print(cout); }
