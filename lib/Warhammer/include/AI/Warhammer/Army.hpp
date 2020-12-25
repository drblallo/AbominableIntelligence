#pragma once

#include <cstddef>
#include <initializer_list>
#include <ostream>

#include "AI/Utils/LocatedObject.hpp"
#include "AI/Utils/SizedArray.hpp"
#include "AI/Utils/Vector.hpp"
#include "AI/Warhammer/Model.hpp"
namespace AI
{
	class Army
	{
		public:
		constexpr static size_t max_model_in_army = 100;
		using LocatedModel = LocatedObject<Model, U8Vector2>;
		using container = SizedArray<LocatedModel, max_model_in_army, uint8_t>;
		using iterator = container::iterator;
		using const_iterator = container::const_iterator;

		constexpr Army(std::initializer_list<LocatedModel> lst): cont(lst) {}
		constexpr Army() = default;

		iterator begin() { return cont.begin(); }
		iterator end() { return cont.end(); }
		[[nodiscard]] const_iterator begin() const { return cont.begin(); }
		[[nodiscard]] const_iterator end() const { return cont.end(); }

		[[nodiscard]] constexpr size_t size() const { return cont.size(); }

		[[nodiscard]] constexpr const LocatedModel& operator[](size_t content) const
		{
			return cont[content];
		}
		constexpr LocatedModel& operator[](size_t content) { return cont[content]; }

		void print(std::ostream& OS, size_t indent = 0) const;
		void dump() const;

		private:
		container cont;
	};
}	 // namespace AI
