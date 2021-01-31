#pragma once

#include <ostream>
namespace AI
{
	inline void indent(std::ostream& OS, size_t indents)
	{
		for (size_t i = 0; i < indents; i++)
			OS << '\t';
	}
}	 // namespace AI
