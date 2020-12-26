#pragma once
#include <bits/c++config.h>
#include <type_traits>

namespace AI
{
	template<class From, class To>
	concept convertible_to = std::is_convertible_v<From, To>&& requires(
			std::add_rvalue_reference_t<From> (&f)())
	{
		static_cast<To>(f());
	};

	template<typename T>
	concept Game = requires(T x)
	{
		typename T::Action;
		typename T::ActionContainer;
		{
			x.possibleActions()
		}
		->convertible_to<typename T::ActionContainer>;

		typename T::ActionContainer::iterator;
		typename T::ActionContainer::const_iterator;
		{
			(typename T::ActionContainer()).size()
		}
		->convertible_to<std::size_t>;
		{
			x.getCurrentPlayerID()
		}
		->convertible_to<std::size_t>;
	};

	template<typename G, typename A>
	concept Action = requires(G game, A action)
	{
		requires Game<G>;
		requires std::is_same_v<A, G::Action>;
		{ action(game) };
		{
			action.canBeExecute(game)
		}
		->convertible_to<bool>;
	};

}	 // namespace AI
