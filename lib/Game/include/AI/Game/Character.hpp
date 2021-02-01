#pragma once

#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "AI/Game/MapElement.hpp"

namespace AI
{
	enum class Activity
	{
		None,
		ExtendInfluence,
		ReduceInfluence,
		BidForOwership
	};

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

	enum Stat
	{
		strenght,
		toughness,
		agility,
		perception,
		intelligence,
		willpower,
		fellowship
	};

	class Statblock
	{
		public:
		static constexpr size_t StatCount = 6;
		using stat_type = std::uint16_t;
		using Container = std::array<stat_type, StatCount>;
		using iterator = Container::iterator;
		using const_iterator = Container::const_iterator;
		constexpr Statblock(std::initializer_list<int> list)
		{
			auto* iter = stats.begin();
			for (auto stat : list)
				*iter++ = stat;
		}

		template<Stat s>
		stat_type get() const
		{
			return stats[s];
		}

		template<Stat s>
		stat_type set(stat_type newValue)
		{
			stats[s] = newValue;
		}

		[[nodiscard]] iterator begin() { return stats.begin(); }

		[[nodiscard]] iterator end() { return stats.end(); }

		[[nodiscard]] const_iterator begin() const { return stats.begin(); }

		[[nodiscard]] const_iterator end() const { return stats.end(); }

		private:
		Container stats{};
	};

	std::string_view kindToName(CharacterKind name);

	using CharacterID = size_t;
	class Character
	{
		public:
		Character(
				CharacterID id, std::string name, Statblock stats, CharacterKind kind)
				: id(id),
					name(std::move(name)),
					kind(kind),
					stats(stats),
					activity(Activity::None)
		{
		}
		[[nodiscard]] const std::string& getName() const { return name; }
		[[nodiscard]] CharacterKind getKind() const { return kind; }
		[[nodiscard]] size_t getID() const { return id; }
		[[nodiscard]] bool isA(CharacterKind charKind) const
		{
			return charKind == kind;
		}

		[[nodiscard]] const Statblock& getStats() const { return stats; }
		Statblock& getStats() { return stats; }

		template<Stat s>
		[[nodiscard]] Statblock::stat_type get() const
		{
			return stats.get<s>();
		}

		template<Stat s>
		void set(Statblock::stat_type newValue)
		{
			stats.set<s>(newValue);
		}

		void print(std::ostream& OS, size_t indents = 0) const;
		void dump() const;

		[[nodiscard]] Activity getActivity() const { return activity; }

		private:
		const CharacterID id;
		std::string name;
		CharacterKind kind;
		Statblock stats;
		Activity activity;
	};
}	 // namespace AI
