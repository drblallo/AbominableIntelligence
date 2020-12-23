#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <ostream>

#include "AI/Warhammer/AbstractStatBlock.hpp"

namespace AI
{
	enum class WeaponStat
	{
		R = 0,	// Range
		S = 1,	// Strength
		AP = 2,
		ROF = 3,
		TYPE = 4,
		LAST = 5,
	};

	enum WeaponType
	{
		RAPID_FIRE = 0,
		HEAVY = 1,
		ASSAULT = 2,
		MELEE = 3,
		PISTOL = 4,
		GRANADE = 5
	};

	void print(std::ostream& OS, WeaponType stat) noexcept;
	void print(std::ostream& OS, WeaponStat stat, std::uint8_t value) noexcept;

	using WeaponStatBlock = AbstractStatBlock<WeaponStat>;
	using WeaponBlockBuilder = AbstractStatBlockBuilder<WeaponStatBlock>;

};	// namespace AI
