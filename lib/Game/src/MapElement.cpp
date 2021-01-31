#include "AI/Game/MapElement.hpp"

#include <iostream>

#include "AI/Utils/Indent.hpp"

using namespace std;
using namespace AI;

void MapElement::print(std::ostream& OS, size_t indents) const
{
	indent(OS, indents);
	OS << "[" << getID() << "] " << getName() << "\n";
	for (const auto& location : locations)
		location.print(OS, indents + 1);
}

void MapElement::dump() const { print(cout); }
