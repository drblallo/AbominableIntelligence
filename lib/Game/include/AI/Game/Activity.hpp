#pragma once

namespace AI
{
	enum class Activity
	{
		None,
		ExtendInfluence,
		ReduceInfluence,
		BidForOwership
	};

	class Map;
	class Character;

	template<Activity activity>
	void resolveActivity(Map& map, Character& character);

}	 // namespace AI
