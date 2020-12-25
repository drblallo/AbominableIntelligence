#pragma once

#include <AI/Warhammer/StatBlock.hpp>
#include <string_view>

#include "AI/Utils/SizedArray.hpp"
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

		using weapon_container =
				SizedArray<Weapon, max_weapons_count, weapon_size_type>;

		using iterator = weapon_container::iterator;
		using const_iterator = weapon_container::const_iterator;

		constexpr Model(StatBlock stats, weapon_container weapons = {}) noexcept
				: stats(stats), weapons(weapons)
		{
		}

		constexpr Model() noexcept: stats(), weapons() {}

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

		constexpr void addWeapon(Weapon w) { weapons.push_back(w); }

		void print(std::ostream& OS, size_t indents = 0) const;
		void dump() const;

		private:
		StatBlock stats;
		weapon_container weapons;
	};

	class ModelBuilder
	{
		public:
		constexpr ModelBuilder(): model(){};
		constexpr ModelBuilder(Model m): model(m){};

		template<ModelStat stat>
		constexpr ModelBuilder& set(StatBlock::StatType val)
		{
			model.get<stat>() = val;
			return *this;
		}

		constexpr ModelBuilder& addWeapon(Weapon w)
		{
			model.addWeapon(w);
			return *this;
		}

		[[nodiscard]] constexpr const Model& getModel() const { return model; }
		[[nodiscard]] constexpr Model& getModel() { return model; }

		private:
		Model model;
	};
}	 // namespace AI
