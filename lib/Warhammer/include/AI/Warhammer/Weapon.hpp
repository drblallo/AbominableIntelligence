#pragma once

#include <array>
#include <bits/stdint-uintn.h>
#include <cstddef>
#include <cstdint>
#include <ostream>

#include "AI/Utils/SizedArray.hpp"
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

	enum class WeaponSpecialRule
	{
		NONE = 0
	};

	void print(std::ostream& OS, WeaponType stat) noexcept;
	void print(std::ostream& OS, WeaponSpecialRule rule) noexcept;

	using WeaponStatBlock = AbstractStatBlock<WeaponStat>;
	using WeaponBlockBuilder = AbstractStatBlockBuilder<WeaponStatBlock>;

	class Weapon
	{
		public:
		constexpr static size_t max_rules_count = 3;
		using rules_size_type = uint8_t;

		using StatType = typename WeaponStatBlock::StatType;
		using StatEnum = typename WeaponBlockBuilder::StatEnumType;
		using Container =
				SizedArray<WeaponSpecialRule, max_rules_count, rules_size_type>;

		using iterator = Container::iterator;
		using const_iterator = Container::const_iterator;

		using stat_iterator = WeaponStatBlock::iterator;
		using stat_const_iterator = WeaponStatBlock::const_iterator;

		template<typename... Rules>
		constexpr explicit Weapon(WeaponStatBlock stats, Rules... rules)
				: stats(stats), specialRule(rules...)
		{
		}

		constexpr Weapon(): stats(), specialRule() {}

		[[nodiscard]] constexpr int operator<=>(const Weapon&) const noexcept =
				default;

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

		[[nodiscard]] iterator begin() { return specialRule.begin(); }
		[[nodiscard]] iterator end() { return specialRule.end(); }
		[[nodiscard]] const_iterator begin() const { return specialRule.begin(); }
		[[nodiscard]] const_iterator end() const { return specialRule.end(); }

		[[nodiscard]] stat_iterator statBegin() { return stats.begin(); }
		[[nodiscard]] stat_iterator statEnd() { return stats.end(); }
		[[nodiscard]] stat_const_iterator statBegin() const
		{
			return stats.begin();
		}

		[[nodiscard]] stat_const_iterator statEnd() const { return stats.end(); }

		[[nodiscard]] const WeaponStatBlock& getStat() const { return stats; }
		[[nodiscard]] WeaponStatBlock& getStat() { return stats; }

		[[nodiscard]] constexpr bool hasRule(WeaponSpecialRule rule) const
		{
			for (const auto& r : specialRule)
				if (r == rule)
					return true;
			return false;
		}

		constexpr void addRule(WeaponSpecialRule rule)
		{
			specialRule.push_back(rule);
		}

		void print(std::ostream& OS, size_t indents = 0) const;

		void dump() const;

		private:
		WeaponStatBlock stats;
		Container specialRule;
	};

	class WeaponBuilder
	{
		public:
		constexpr WeaponBuilder(): w() {}

		template<WeaponStat stat>
		constexpr WeaponBuilder& set(WeaponStatBlock::StatType val)
		{
			w.get<stat>() = val;
			return *this;
		}

		constexpr WeaponBuilder& addRule() { return *this; }

		template<typename First, typename... Rest>
		constexpr WeaponBuilder& addRule(First w, Rest... r)
		{
			w.addRule(w);
			return addRule(r...);
		}

		[[nodiscard]] constexpr Weapon getWeapon() const { return w; }

		private:
		Weapon w;
	};

};	// namespace AI
