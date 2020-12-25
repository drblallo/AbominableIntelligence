#pragma once

#include <array>
#include <cstddef>

#include "Army.hpp"

namespace AI
{
	enum PlayerEnum
	{
		PLAYER1 = 0,
		PLAYER2 = 1
	};

	class W40kGame
	{
		public:
		using container = std::array<Army, 2>;
		using iterator = typename container::iterator;
		using const_iterator = typename container::const_iterator;

		constexpr W40kGame(Army p1, Army p2): armies({ p1, p2 }) {}

		iterator begin() { return armies.begin(); }
		iterator end() { return armies.end(); }
		[[nodiscard]] const_iterator begin() const { return armies.begin(); }
		[[nodiscard]] const_iterator end() const { return armies.end(); }

		[[nodiscard]] constexpr const Army& getPl1Army() const
		{
			return armies.front();
		}
		[[nodiscard]] constexpr Army& getPl1Army() { return armies.front(); }
		[[nodiscard]] constexpr const Army& getPl2Army() const
		{
			return armies.back();
		}
		[[nodiscard]] constexpr Army& getPl2Army() { return armies.back(); }

		[[nodiscard]] constexpr const Army& operator[](size_t index) const
		{
			return armies.at(index);
		}
		[[nodiscard]] constexpr Army& operator[](size_t index)
		{
			return armies.at(index);
		}

		private:
		container armies;
	};
}	 // namespace AI
