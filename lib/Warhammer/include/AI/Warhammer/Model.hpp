#pragma once

#include <AI/Warhammer/StatBlock.hpp>
#include <bits/stdint-uintn.h>
#include <string_view>

#include "AI/Utils/SizedArray.hpp"
#include "AI/Utils/Vector.hpp"
#include "AI/Warhammer/Weapon.hpp"

namespace AI
{
	class Model
	{
		public:
		using StatType = typename StatBlock::StatType;
		using StatEnum = typename StatBlock::StatEnumType;

		using stat_iterator = typename StatBlock::iterator;
		using stat_const_iterator = typename StatBlock::const_iterator;

		constexpr static size_t max_weapons_count = 3;
		using weapon_size_type = uint8_t;

		using WeaponContainer =
				SizedArray<Weapon, max_weapons_count, weapon_size_type>;

		using iterator = WeaponContainer::iterator;
		using const_iterator = WeaponContainer::const_iterator;

		constexpr Model(StatBlock stats, U8Vector2 location = {}) noexcept
				: stats(stats), location(location), weapons()
		{
		}

		[[nodiscard]] constexpr const U8Vector2& getLocation() const
		{
			return location;
		}
		[[nodiscard]] constexpr U8Vector2& getLocation() { return location; }

		template<StatEnum stat>
		[[nodiscard]] constexpr const StatType& get() const noexcept
		{
			return stats.get<stat>();
		}

		template<StatEnum stat>
		[[nodiscard]] constexpr StatType& get() noexcept
		{
			return stats.get<stat>();
		}

		[[nodiscard]] constexpr const StatType& get(StatEnum stat) const noexcept
		{
			return stats.get(stat);
		}

		[[nodiscard]] constexpr StatType& get(StatEnum stat) noexcept
		{
			return stats.get(stat);
		}

		[[nodiscard]] constexpr const StatType& operator[](
				StatEnum stat) const noexcept
		{
			return stats.get(stat);
		}

		[[nodiscard]] constexpr StatType& operator[](StatEnum stat) noexcept
		{
			return stats.get(stat);
		}

		[[nodiscard]] constexpr int operator<=>(const Model&) const noexcept =
				default;

		[[nodiscard]] stat_iterator statBegin() { return stats.begin(); }
		[[nodiscard]] stat_iterator statEnd() { return stats.end(); }
		[[nodiscard]] stat_const_iterator statBegin() const
		{
			return stats.begin();
		}

		[[nodiscard]] stat_const_iterator statEnd() const { return stats.end(); }

		[[nodiscard]] const StatBlock& getStat() const { return stats; }
		[[nodiscard]] StatBlock& getStat() { return stats; }

		[[nodiscard]] iterator begin() { return weapons.begin(); }
		[[nodiscard]] iterator end() { return weapons.end(); }

		[[nodiscard]] const_iterator begin() const { return weapons.begin(); }
		[[nodiscard]] const_iterator end() const { return weapons.end(); }

		private:
		StatBlock stats;
		U8Vector2 location;
		WeaponContainer weapons;
	};
}	 // namespace AI
