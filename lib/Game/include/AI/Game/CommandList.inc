#pragma once

namespace AI
{

static constexpr CommandList commandListNoQuit = makeCommandList(showCaracters, "chars")
																				.add(AI::infect, "inf")
																				.add(AI::showLocations, "loc")
																				.add(AI::getPop, "pop")
																				.add(AI::getPops, "pops")
																				.add(AI::showMapElements, "map")
																				.add(AI::showCaracter, "char")
																				.add(AI::nextDay, "day")
																				.add(AI::getStat, "stat")
																				.add(AI::nextDay, "d")
																				.add(AI::skipDays, "skip")
																				.add(AI::nextDay, ":d");


static constexpr CommandList commandList = commandListNoQuit.add(AI::exit, "exit") 
																				.add(AI::exit, "q")
																				.add(AI::exit, "quit")
																				.add(AI::exit, ":q");
}
