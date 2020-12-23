#pragma once

#include <AI/Warhammer/StatBlock.hpp>
#include <string_view>

namespace AI
{
	class Model
	{
		public:
		constexpr Model(StatBlock stats) noexcept: stats(stats) {}

		private:
		StatBlock stats;
	};
}	 // namespace AI
