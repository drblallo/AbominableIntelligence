#pragma once

#include <cassert>

#include "AI/Warhammer/W40KGame.hpp"

namespace AI
{
	using GameModifyingFunction = void (*)(W40kGame& game);
	using GameReadOnlyFunction = bool (*)(const W40kGame& game);

	template<GameReadOnlyFunction... F>
	constexpr bool fOr(const W40kGame& game)
	{
		return (F(game) or ...);
	}

	template<GameReadOnlyFunction... F>
	constexpr bool fAnd(const W40kGame& game)
	{
		return (F(game) and ...);
	}

	template<GameModifyingFunction... F>
	constexpr void fAll(W40kGame& game)
	{
		(F(game), ...);
	}

	class W40kAction
	{
		public:
		constexpr W40kAction(
				GameReadOnlyFunction check, GameModifyingFunction action)
				: execute(action), canBeExecute(check)
		{
		}

		constexpr void operator()(W40kGame& game) const
		{
			assert(canBeExecute(game));
			execute(game);
		}

		[[nodiscard]] constexpr bool canBeExecuted(const W40kGame& game) const
		{
			return canBeExecute(game);
		}

		private:
		GameModifyingFunction execute;
		GameReadOnlyFunction canBeExecute;
	};

};	// namespace AI
