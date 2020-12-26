#pragma once

#include <array>
#include <cassert>
#include <initializer_list>
#include <string_view>
#include <vector>

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
				GameReadOnlyFunction check,
				GameModifyingFunction action,
				std::string_view name)
				: execute(action), canBeExecute(check), name(name)
		{
			assert(check != nullptr);
			assert(action != nullptr);
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

		[[nodiscard]] constexpr std::string_view getName() const { return name; }

		private:
		const GameModifyingFunction execute;
		const GameReadOnlyFunction canBeExecute;
		const std::string_view name;
	};

	template<typename Container>
	class W40kActionDBImpl
	{
		public:
		using container = Container;
		using const_iterator = typename container::const_iterator;

		[[nodiscard]] const_iterator begin() const { return actions.begin(); }
		[[nodiscard]] const_iterator end() const { return actions.end(); }

		[[nodiscard]] constexpr const W40kAction& operator[](size_t index) const
		{
			return actions[index];
		}

		[[nodiscard]] constexpr size_t size() const { return actions.size(); }

		constexpr W40kActionDBImpl(container actions): actions(actions) {}

		private:
		container actions;
	};

};	// namespace AI
