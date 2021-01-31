#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <utility>

#include "AI/Utils/SizedArray.hpp"

namespace AI
{
	using strong_ordering = int;
	enum class PopKind
	{
		human,
		guards,
		spaceMarine,
		genestealer,
		mechanicum
	};

	struct Pop
	{
		using size_type = std::uint32_t;

		PopKind kind{ PopKind::human };
		size_type quantity{ 0 };

		constexpr static Pop human(std::uint32_t quantity)
		{
			return Pop{ PopKind::human, quantity };
		}
		constexpr static Pop guards(std::uint32_t quantity)
		{
			return Pop{ PopKind::guards, quantity };
		}
		constexpr static Pop marine(std::uint32_t quantity)
		{
			return Pop{ PopKind::spaceMarine, quantity };
		}
		constexpr static Pop genestealer(std::uint32_t quantity)
		{
			return Pop{ PopKind::genestealer, quantity };
		}
		constexpr static Pop mechanicus(std::uint32_t quantity)
		{
			return Pop{ PopKind::mechanicum, quantity };
		}

		strong_ordering operator<=>(const Pop& other) const = default;
	};

	inline bool popQuantityCmp(const Pop& l, const Pop& r)
	{
		return l.quantity < r.quantity;
	}

	template<size_t MaxSize>
	using Population = SizedArray<Pop, MaxSize, uint32_t>;

}	 // namespace AI
