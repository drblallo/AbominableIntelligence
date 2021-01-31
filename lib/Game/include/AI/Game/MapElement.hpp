#pragma once

#include <initializer_list>
#include <utility>
#include <vector>

#include "AI/Game/Location.hpp"

namespace AI
{
	class MapElement
	{
		public:
		using Container = std::vector<Location>;
		using iterator = Container::iterator;
		using const_iterator = Container::const_iterator;
		using reference = Container::reference;
		using pointer = Container::pointer;
		using const_reference = Container::const_reference;
		using const_pointer = Container::const_pointer;

		MapElement(
				size_t index,
				std::string name,
				std::initializer_list<Location> locs = {})
				: index(index), name(std::move(name)), locations(locs)
		{
			updateChildrensID();
		}
		MapElement(std::string name, std::initializer_list<Location> locs = {})
				: index(0), name(std::move(name)), locations(locs)
		{
			updateChildrensID();
		}

		template<typename... Args>
		Location& addLocation(Args&&... args)
		{
			locations.emplace_back(std::forward<Args>(args)...);
			locations.back().setKey(LocationKey{ getID(), locations.size() });
			return locations.back();
		}

		iterator begin() { return locations.begin(); }
		iterator end() { return locations.end(); }
		[[nodiscard]] const_iterator begin() const { return locations.begin(); }
		[[nodiscard]] const_iterator end() const { return locations.end(); }

		[[nodiscard]] bool empty() const { return locations.empty(); }
		[[nodiscard]] size_t size() const { return locations.size(); }

		[[nodiscard]] const std::string& getName() const { return name; }
		void setName(std::string newName) { name = std::move(newName); }

		const Location& operator[](size_t index) const { return locations[index]; }
		Location& operator[](size_t index) { return locations[index]; }

		[[nodiscard]] size_t getID() const { return index; }
		void setID(size_t newIndex)
		{
			index = newIndex;
			updateChildrensID();
		}
		void print(std::ostream& OS, size_t indents = 0) const;
		void dump() const;

		private:
		void updateChildrensID()
		{
			for (size_t i = 0; i < locations.size(); i++)
				locations[i].setKey(LocationKey{ index, i });
		}
		size_t index;
		std::string name;
		Container locations;
	};

}	 // namespace AI
