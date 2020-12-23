#pragma once

#include <vector>

#include "AI/Game/Concepts.hpp"
#include "AI/tree/MonteCarloNode.hpp"

namespace AI
{
	template<Game G>
	class MonteCarloTree
	{
		public:
		using Action = typename G::Action;
		explicit MonteCarloTree(G g): root(nullptr, std::move(g), nullptr)
		{
			assert(not root.isTerminal());
		}

		void backup(MonteCarloNode<G> &node, bool playerOne)
		{
			auto *currNode = &node;
			while (currNode != nullptr)
			{
				currNode->visit();
				currNode->addReward(playerOne);

				auto currentParent = currNode->getParent();
				currNode = currentParent;
			}
		}

		MonteCarloNode<G> &treePolicy()
		{
			MonteCarloTree<G> *node(&root);
			while (not node->isTerminal())
			{
				if (not node->isFullyExpanded())
					return node->expand();

				node = &(node->bestChild());
			}
			return *node;
		}

		Action getBestAction() const
		{
			if (root.isPlayerOneNode())
				return (*std::max_element(
										root.begin(),
										root.end(),
										[](const auto &l, const auto &r) {
											return l->marineWinRatio() < r->marineWinRatio();
										}))
						->getAction();
			return (*std::min_element(
									root.begin(),
									root.end(),
									[](const auto &l, const auto &r) {
										return l->marineWinRatio() < r->marineWinRatio();
									}))
					->getAction();
		}

		MonteCarloNode<G> &defaultPolicy(MonteCarloNode<G> *node)
		{
			while (not node->isTerminal())
				node = &node->expand();
			return *node;
		}

		private:
		MonteCarloNode<G> root;
	};

	template<Game G>
	typename G::Action mcts(const G &g, size_t maxIter = 1000)
	{
		MonteCarloTree t(g);
		for (size_t i = 0; i < maxIter; i++)
		{
			auto &node = t.treePolicy();
			// auto &terminalNode = t.defaultPolicy(&node, net);
			t.backup(node, marineScoring(node.getGame()));
		}
		return t.getBestAction();
	}

}	 // namespace AI
