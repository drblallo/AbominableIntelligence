#pragma once

#include "AI/Game/Activity.hpp"
#include "AI/Game/Location.hpp"
#include "AI/Game/Map.hpp"

namespace AI
{
	template<typename... Args>
	class Event
	{
		public:
		using ConditionType = bool(const Args&... args);
		using ApplyType = void(Args&... args);
		constexpr Event(
				const char* name, ConditionType* condition, ApplyType* action)
				: condition(condition), action(action), name(name)
		{
		}

		[[nodiscard]] bool check(const Args&... args) const
		{
			return check(args...);
		}
		void apply(Args&... args) const { return action(args...); }

		[[nodiscard]] const char* getName() const { return name; }

		private:
		ConditionType* condition;
		ApplyType* action;
		const char* name;
	};

	using GlobalEvent = Event<Map>;
	using LocationEvent = Event<Map, Location>;
	using CharacterEvent = Event<Map, Character>;
}	 // namespace AI
