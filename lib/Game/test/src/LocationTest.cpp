#include "gtest/gtest.h"

#include "AI/Game/Location.hpp"

using namespace std;
using namespace AI;

TEST(LocationTest, addPopulationTest)
{
	Location loc("example");
	EXPECT_EQ(loc.getPopulation().size(), 0);
	loc.addPopulation(PopKind::genestealer, 1);
	EXPECT_EQ(loc.getPopulation().size(), 1);
	loc.addPopulation(PopKind::genestealer, 1);
	EXPECT_EQ(loc.getPopulation().size(), 1);
	EXPECT_NEAR(loc.getPopulation()[0].quantity, 2, 0.01);
	loc.addPopulation(PopKind::genestealer2, 1);
	EXPECT_EQ(loc.getPopulation().size(), 2);
	loc.addPopulation(PopKind::genestealer3, 1);
	loc.addPopulation(PopKind::genestealer4, 1);
	loc.addPopulation(PopKind::BroodBrohter, 1);
	loc.addPopulation(PopKind::spaceMarine, 1);
	loc.addPopulation(PopKind::human, 1);
	loc.addPopulation(PopKind::mechanicum, 1);
	EXPECT_EQ(loc.getPopulation().size(), 8);
	loc.addPopulation(PopKind::guards, 1);
	EXPECT_EQ(loc.getPopulation().size(), 8);
}
