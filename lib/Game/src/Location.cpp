#include "AI/Game/Location.hpp"

#include <iostream>
#include <ostream>

#include "AI/Game/Character.hpp"
#include "AI/Game/Population.hpp"
#include "AI/Utils/Indent.hpp"

using namespace std;
using namespace AI;

static std::string kindToName(PopKind kind)
{
	switch (kind)
	{
		case PopKind::human:
			return "Human";
		case PopKind::guards:
			return "Imperial Guard";
		case PopKind::spaceMarine:
			return "Space Marine";
		case PopKind::genestealer:
			return "Genestealer";
		case PopKind::mechanicum:
			return "Mechanicum";
	}
	assert(false and "unreachable");
	return "ERROR";
}

static void printPop(ostream& OS, int indents, const Pop& pop)
{
	indent(OS, indents);
	OS << kindToName(pop.kind) << ": " << pop.quantity << "\n";
}

void Location::print(ostream& OS, size_t indents) const
{
	indent(OS, indents);
	OS << "[" << getKey().MaplElementID << "," << getKey().LocationID << "] "
		 << getName() << "\n";
	for (const auto& pop : population)
		printPop(OS, indents + 1, pop);
}
void Location::dump() const { print(std::cout); }
