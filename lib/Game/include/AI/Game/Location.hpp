#pragma once

#include <cstdint>
#include <functional>
#include <vector>

#include "AI/Game/Population.hpp"

namespace AI
{
	struct LocationKey
	{
		size_t MaplElementID;
		size_t LocationID;
		static LocationKey invalid()
		{
			return { std::numeric_limits<size_t>::max(),
							 std::numeric_limits<size_t>::max() };
		}
		int operator<=>(const LocationKey& other) const
		{
			if (MaplElementID < other.MaplElementID)
				return -1;
			else if (MaplElementID > other.MaplElementID)
				return 1;
			else if (LocationID < other.LocationID)
				return -1;
			else if (LocationID > other.LocationID)
				return 1;
			return 0;
		}
		bool operator==(const LocationKey& other) const = default;
		[[nodiscard]] bool isValid() const { return *this != invalid(); }
	};
	class Location
	{
		public:
		constexpr static std::size_t MaxPop = 8;
		using Container = Population<MaxPop>;
		using iterator = typename Container::iterator;
		using const_iterator = typename Container::const_iterator;

		Location(std::string name, Container population)
				: key(LocationKey::invalid()),
					name(std::move(name)),
					population(population)
		{
		}

		template<typename... Pops>
		Location(std::string name, Pops&&... pops)
				: key(LocationKey::invalid()),
					name(std::move(name)),
					population({ std::forward<Pops>(pops)... })
		{
		}

		void addPopulation(PopKind kind, Pop::size_type quantity);
		[[nodiscard]] const std::string& getName() const { return name; }
		Container& getPopulation() { return population; }
		[[nodiscard]] const Container& getPopulation() const { return population; }

		iterator begin() { return population.begin(); }
		[[nodiscard]] const_iterator begin() const { return population.begin(); }
		iterator end() { return population.end(); }
		[[nodiscard]] const_iterator end() const { return population.end(); }
		[[nodiscard]] LocationKey getKey() const { return key; }
		void setKey(LocationKey newKey) { key = newKey; }

		void print(std::ostream& OS, size_t indents = 0) const;
		void dump() const;

		[[nodiscard]] Pop::size_type getPopulation(PopKind kind) const
		{
			for (const auto& p : population)
				if (p.kind == kind)
					return p.quantity;
			return 0;
		}

		private:
		LocationKey key;
		std::string name;
		Container population;
	};
}	 // namespace AI
