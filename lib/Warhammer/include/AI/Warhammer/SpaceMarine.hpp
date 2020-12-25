#pragma once

#include "AI/Warhammer/Model.hpp"
#include "AI/Warhammer/StatBlock.hpp"
#include "AI/Warhammer/Weapon.hpp"

namespace AI
{
	constexpr Weapon bolter = WeaponBuilder()
																.set<WeaponStat::AP>(1)
																.set<WeaponStat::R>(24)
																.set<WeaponStat::ROF>(1)
																.set<WeaponStat::S>(4)
																.set<WeaponStat::TYPE>(RAPID_FIRE)
																.getWeapon();
	constexpr Model baseMarine = ModelBuilder()
																	 .set<ModelStat::A>(1)
																	 .set<ModelStat::BS>(3)
																	 .set<ModelStat::LD>(8)
																	 .set<ModelStat::M>(6)
																	 .set<ModelStat::S>(4)
																	 .set<ModelStat::SV>(3)
																	 .set<ModelStat::T>(4)
																	 .set<ModelStat::W>(1)
																	 .set<ModelStat::WS>(3)
																	 .addWeapon(bolter)
																	 .getModel();

}	 // namespace AI
