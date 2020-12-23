#include "gtest/gtest.h"
#include <cstddef>
#include <vector>

#include "AI/Game/Concepts.hpp"
#include "AI/tree/MonteCarloNode.hpp"

using namespace AI;
struct GameImpl
{
	using Action = int;
	using ActionContainer = std::vector<Action>;
	[[nodiscard]] size_t getCurrentPlayerID() const { return 0; }

	ActionContainer possibleActions() const { return ActionContainer({ 1 }); }

	int x{ 0 };
};

TEST(MonteCarloTreeNodeTest, simpleTest)
{
	GameImpl impl;
	EXPECT_EQ(impl.x, 0);
}

TEST(MonteCarloTreeNodeTest, treeNodeCanInstantiated)
{
	GameImpl g;
	MonteCarloNode<GameImpl> node(nullptr, g, 2);
	EXPECT_EQ(node.getAction(), 2);
}
