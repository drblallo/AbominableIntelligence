#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <ostream>

namespace AI
{
	template<typename StatEnumT, typename StatT = std::uint8_t>
	class AbstractStatBlock
	{
		public:
		constexpr static size_t StatCount = static_cast<size_t>(StatEnumT::LAST);

		using StatEnumType = StatEnumT;
		using StatType = StatT;
		using Container = std::array<StatType, StatCount>;
		using iterator = typename Container::iterator;
		using const_iterator = typename Container::const_iterator;

		template<typename... Stat>
		explicit constexpr AbstractStatBlock(Stat... s) noexcept
				: stats({ static_cast<StatType>(s)... })
		{
		}

		template<StatEnumType stat>
		[[nodiscard]] constexpr const StatType& get() const noexcept
		{
			return stats[static_cast<size_t>(stat)];
		}

		template<StatEnumType stat>
		[[nodiscard]] constexpr StatType& get() noexcept
		{
			return stats[static_cast<size_t>(stat)];
		}

		[[nodiscard]] constexpr const StatType& get(
				StatEnumType stat) const noexcept
		{
			return stats[static_cast<size_t>(stat)];
		}

		[[nodiscard]] constexpr StatType& get(StatEnumType stat) noexcept
		{
			return stats[static_cast<size_t>(stat)];
		}

		constexpr int operator<=>(const AbstractStatBlock& other) const noexcept
		{
			for (size_t i = 0; i < StatCount; i++)
			{
				if (stats.at(i) < other.stats.at(i))
					return -1;
				if (stats.at(i) > other.stats.at(i))
					return 1;
			}
			return 0;
		}

		constexpr bool operator==(const AbstractStatBlock& other) const noexcept
		{
			for (size_t i = 0; i < StatCount; i++)
				if (stats.at(i) != other.stats.at(i))
					return false;

			return true;
		}

		[[nodiscard]] iterator begin() noexcept { return stats.begin(); }
		[[nodiscard]] iterator end() noexcept { return stats.end(); }
		[[nodiscard]] const_iterator begin() const noexcept
		{
			return stats.begin();
		}
		[[nodiscard]] const_iterator end() const noexcept { return stats.end(); }

		void print(std::ostream& OS, size_t indent = 0) const
		{
			for (size_t i = 0; i < indent; i++)
				OS << "\t";

			for (size_t i = 0; i < StatCount; i++)
			{
				print(OS, static_cast<StatEnumType>(i), stats[i]);
				OS << " ";
			}

			OS << "\n";
		}

		private:
		Container stats;
	};

	template<typename StatBlockType>
	class AbstractStatBlockBuilder
	{
		public:
		using StatEnumType = typename StatBlockType::StatEnumType;
		using StatType = typename StatBlockType::StatType;

		constexpr AbstractStatBlockBuilder() noexcept: blk(){};

		template<StatEnumType stat>
		[[nodiscard]] constexpr AbstractStatBlockBuilder set(
				StatType newValue) const noexcept
		{
			auto toReturn = *this;
			toReturn.blk.get(stat) = newValue;
			return toReturn;
		}

		[[nodiscard]] constexpr const StatBlockType& getBlock() const noexcept
		{
			return blk;
		}
		[[nodiscard]] constexpr StatBlockType& getBlock() noexcept { return blk; }

		private:
		StatBlockType blk;
	};
}	 // namespace AI
