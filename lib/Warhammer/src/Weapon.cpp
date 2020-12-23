#include "AI/Warhammer/Weapon.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <ostream>

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

void AI::print(ostream& OS, WeaponStat stat, uint8_t value) noexcept
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
			print(OS, static_cast<WeaponType>(value));
			return;
		case WeaponStat::LAST:
			OS << "ERROR";
			return;
	}
}
