#include "AI/Warhammer/StatBlock.hpp"

#include <bits/stdint-uintn.h>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <ostream>

#include "AI/Warhammer/AbstractStatBlock.hpp"

using namespace std;
using namespace AI;

template<>
void AbstractStatBlock<ModelStat, uint8_t>::print(
		ostream& OS, ModelStat stat, uint8_t value)
{
	switch (stat)
	{
		case ModelStat::M:
			OS << "M";
		case ModelStat::WS:
			OS << "WS";
		case ModelStat::BS:
			OS << "BS";
		case ModelStat::S:
			OS << "S";
		case ModelStat::T:
			OS << "T";
		case ModelStat::W:
			OS << "W";
		case ModelStat::A:
			OS << "A";
		case ModelStat::LD:
			OS << "LD";
		case ModelStat::SV:
			OS << "SV";
		case ModelStat::LAST:
			OS << "Error";
	}
	OS << ": " << value;
}
