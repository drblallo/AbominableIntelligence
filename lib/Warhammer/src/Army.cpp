#include "AI/Warhammer/Army.hpp"

#include <iostream>
#include <ostream>

using namespace AI;
using namespace std;

void Army::dump() const { print(cout); }

void Army::print(ostream& OS, size_t indent) const
{
	for (const auto& model : cont)
	{
		model.getPos().print(OS, indent);
		model->print(OS);
		OS << "\n";
	}
}
