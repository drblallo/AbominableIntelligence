#include <cstdint>
#include <fstream>
#include <ios>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string>

#include "AI/Game/Actions.hpp"
#include "AI/Game/InwardSuns.hpp"
#include "AI/Utils/CommandPrompt.hpp"
#include "AI/Utils/NullStream.hpp"
#include "AI/Utils/StreamPrompt.hpp"

using namespace AI;
using namespace std;

static constexpr CommandList list = makeCommandList(showCaracters, "chars")
																				.add(AI::showLocations, "loc")
																				.add(AI::getPop, "pop")
																				.add(AI::showMapElements, "map")
																				.add(AI::showCaracter, "char")
																				.add(AI::nextDay, "day")
																				.add(AI::getStat, "stat")
																				.add(AI::nextDay, "d")
																				.add(AI::skipDays, "skip")
																				.add(AI::nextDay, ":d");

pair<const char*, string> nextString(const char* start, const char* end)
{
	const char* next =
			find_if(start, end, [](char c) { return c == 0 or c == '\n'; });
	string toReturn(start, next);
	return make_pair(next, toReturn);
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size)
{
	if (Size == 0)
		return 0;
	seed_seq seq({ Data[0] });
	auto inwardSun = inwardSuns(seq);
	NullStream OS;
	CommandPrompt prompt(list, inwardSun, OS);

	const char* current = reinterpret_cast<const char*>(Data);
	const char* end = reinterpret_cast<const char*>(Data + Size);

	while (current < end)
	{
		auto [sEnd, string] = nextString(current, end);
		auto _ = prompt(string);
		current = sEnd + 1;
	}

	return 0;
}
