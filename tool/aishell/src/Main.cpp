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

#include "AI/Game/CommandList.inc"

struct Args
{
	bool showPrompt{ true };
	string command;
	string inputFile;
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
		executeCommandStream(args.showPrompt, s, cout, commandList, map, cout);
		return 0;
	}

	if (not args.inputFile.empty())
	{
		fstream s;
		s.open(args.inputFile, ios_base::in);
		executeCommandStream(args.showPrompt, s, cout, commandList, map, cout);
		return 0;
	}

	executeCommandStream(args.showPrompt, cin, cout, commandList, map, cout);
	return 0;
}
