#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "AI/Warhammer/AbstractStatBlock.hpp"

namespace AI
{
	enum class ModelStat
	{
		M = 0,
		WS = 1,
		BS = 2,
		S = 3,
		T = 4,
		W = 5,
		A = 6,
		LD = 7,
		SV = 8,
		LAST = 9
	};

	using StatBlock = AbstractStatBlock<ModelStat>;
	using StatBlockBuilder = AbstractStatBlockBuilder<StatBlock>;

}	 // namespace AI
