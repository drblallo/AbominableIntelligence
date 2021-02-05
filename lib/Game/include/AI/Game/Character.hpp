#pragma once

#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "AI/Game/Activity.hpp"
#include "AI/Game/MapElement.hpp"

namespace AI
{
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
		RoguePsyker,
		OrkWarboss,
		EldarAutharc,
		EldarPirate,
		GenestealerPatriarch
	};

	constexpr inline float maxRequisition(CharacterKind kind)
	{
		switch (kind)
		{
			case CharacterKind::FeudalLord:
			case CharacterKind::GangLord:
			case CharacterKind::RoguePsyker:
				return 30;
			case CharacterKind::PDFColonnell:
			case CharacterKind::OrkWarboss:
				return 50;
			case CharacterKind::Bishop:
			case CharacterKind::PlanetaryGovernor:
			case CharacterKind::LocalNobility:
			case CharacterKind::EldarAutharc:
			case CharacterKind::EldarPirate:
			case CharacterKind::SisterOfBattleBadess:
			case CharacterKind::PDFGeneral:
			case CharacterKind::GenestealerPatriarch:
				return 60;
			case CharacterKind::SpaceMarineCaptain:
			case CharacterKind::ImperialGuardCommander:
				return 80;
			case CharacterKind::Inquisitor:
			case CharacterKind::SpaceMarineChapterMaster:
				return 100;
		}
	}

	enum Stat
	{
		strenght,
		toughness,
		agility,
		perception,
		intelligence,
		willpower,
		fellowship,
		requisition
	};

	class Statblock
	{
		public:
		static constexpr size_t StatCount = requisition + 1;
		using stat_type = float;
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
		[[nodiscard]] stat_type get() const
		{
			return stats[s];
		}

		template<Stat s>
		void set(stat_type newValue)
		{
			stats[s] = newValue;
		}
		[[nodiscard]] stat_type operator[](size_t index) const
		{
			return stats[index];
		}

		[[nodiscard]] iterator begin() { return stats.begin(); }

		[[nodiscard]] iterator end() { return stats.end(); }

		[[nodiscard]] const_iterator begin() const { return stats.begin(); }

		[[nodiscard]] const_iterator end() const { return stats.end(); }

		void print(std::ostream& OS, size_t indents = 0) const;
		void dump() const;

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

		[[nodiscard]] Statblock::stat_type operator[](size_t index) const
		{
			return stats[index];
		}

		void print(std::ostream& OS, size_t indents = 0) const;
		void dump() const;

		[[nodiscard]] Activity getActivity() const { return activity; }
		void setActivity(Activity newActivity) { activity = newActivity; }

		private:
		const CharacterID id;
		std::string name;
		CharacterKind kind;
		Statblock stats;
		Activity activity;
	};
}	 // namespace AI
