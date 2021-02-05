#pragma once

#include <iostream>
#include <streambuf>

namespace AI
{
	class NullStream: public std::ostream
	{
		class NullBuffer: public std::streambuf
		{
			public:
			int overflow(int c) { return c; }

		} m_nb;

		public:
		NullStream(): std::ostream(&m_nb) {}
	};
}	 // namespace AI
