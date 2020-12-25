#include "AI/Warhammer/Model.hpp"

#include <iostream>

using namespace std;
using namespace AI;

void Model::dump() const { print(cout); }

void Model::print(ostream& OS, size_t indents) const
{
	stats.print(OS, indents);
	OS << "\n";
	for (const auto& weapon : weapons)
	{
		weapon.print(OS, indents + 1);
		OS << "\n";
	}
}
