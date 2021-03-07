#include "AI/Game/Location.hpp"

#include <iostream>
#include <ostream>

#include "AI/Game/Character.hpp"
#include "AI/Game/Population.hpp"
#include "AI/Utils/Indent.hpp"

using namespace std;
using namespace AI;

std::string AI::kindToName(PopKind kind)
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
			return "Pure genestealer";
		case PopKind::genestealer2:
			return "Second generation";
		case PopKind::genestealer3:
			return "Third generation";
		case PopKind::genestealer4:
			return "Fourth generation";
		case PopKind::BroodBrohter:
			return "Brood brothers";
		case PopKind::mechanicum:
			return "Mechanicum";
		case PopKind::END:
			return "ERROR";
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

void Location::addPopulation(PopKind kind, Pop::size_type quantity)
{
	if (quantity == 0)
		return;
	auto* pos = find_if(
			begin(), end(), [kind](const Pop& pop) { return pop.kind == kind; });
	if (pos != end())
	{
		pos->quantity = max(0.0, quantity + pos->quantity);
		return;
	}

	if (population.size() == MaxPop)
		return;

	population.push_back(Pop{ kind, quantity });
}
