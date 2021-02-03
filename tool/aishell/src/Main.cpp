#include <fstream>
#include <ios>
#include <iostream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string>

#include "AI/Game/Actions.hpp"
#include "AI/Game/InwardSuns.hpp"
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
																				.add(AI::nextDay, ":d")
																				.add(AI::exit, "exit")
																				.add(AI::exit, "q")
																				.add(AI::exit, "quit")
																				.add(AI::exit, ":q");

struct Args
{
	bool showPrompt{ true };
	string command{ "" };
	string inputFile{ "" };
};

static Args parseArgs(int argc, char* argv[])
{
	Args toReturn{};
	for (int i = 1; i < argc; i++)
	{
		string arg(argv[i]);
		if (arg == "--no-prompt")
		{
			toReturn.showPrompt = false;
			continue;
		}
		if (arg == "-c")
		{
			toReturn.command = argv[++i];
			toReturn.showPrompt = false;
			continue;
		}
		if (arg == "-i")
		{
			toReturn.inputFile = argv[++i];
			toReturn.showPrompt = false;
			continue;
		}
	}

	return toReturn;
}

int main(int argc, char* argv[])
{
	Args args(parseArgs(argc, argv));
	seed_seq seed({ 0 });
	Map map = inwardSuns(seed);

	if (not args.command.empty())
	{
		istringstream s(args.command);
		executeCommandStream(args.showPrompt, s, cout, list, map, cout);
		return 0;
	}

	if (not args.inputFile.empty())
	{
		fstream s;
		s.open(args.inputFile, ios_base::in);
		executeCommandStream(args.showPrompt, s, cout, list, map, cout);
		return 0;
	}

	executeCommandStream(args.showPrompt, cin, cout, list, map, cout);
	return 0;
}
