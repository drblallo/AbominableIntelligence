#pragma once

#include <cstdint>
#include <limits>
#include <random>

namespace AI
{
	template<typename type, size_t FacesCount>
	class Dice
	{
		public:
		constexpr static size_t faces_count = FacesCount;
		using value_type = type;

		explicit Dice(std::seed_seq& seed)
				: generator(seed), distribution(1, FacesCount)
		{
			static_assert(std::numeric_limits<type>::max() > FacesCount);
		}

		Dice(): generator(), distribution(1, FacesCount)	// NOLINT
		{
			static_assert(std::numeric_limits<type>::max() > FacesCount);
			std::random_device dev;
			generator.seed(dev());
		}

		value_type roll() { return distribution(generator); }

		template<typename Iterator>
		void fill(Iterator begin, Iterator end)
		{
			for (Iterator it = begin; it != end; it++)
				*it = roll();
		}

		template<typename Container>
		void fill(Container c)
		{
			for (auto& val : c)
				val = roll();
		}

		private:
		std::mt19937 generator;
		std::uniform_int_distribution<type> distribution;
	};
}	 // namespace AI
