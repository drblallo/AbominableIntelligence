#pragma once

#include <string>

namespace AI
{
	enum class Activity
	{
		None,
		ExtendInfluence,
		ReduceInfluence,
		BidForOwership,
		Infect
	};

	class Map;
	class Character;

	template<Activity activity>
	void resolveActivity(Map& map, Character& character);
	std::string activityToString(Activity a);

}	 // namespace AI
