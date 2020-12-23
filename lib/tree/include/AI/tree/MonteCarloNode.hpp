#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <ostream>
#include <vector>

#include "AI/Game/Concepts.hpp"

namespace AI
{
	constexpr double ExplorationRate = 0.1;

	template<Game G>
	class MonteCarloNode
	{
		public:
		using content = std::vector<std::unique_ptr<MonteCarloNode>>;
		using iterator = typename content::iterator;
		using const_iterator = typename content::const_iterator;
		using Action = typename G::Action;
		using ActionContainer = typename G::ActionContainer;

		MonteCarloNode(MonteCarloNode *parent, G game, Action action)
				: parent(parent),
					g(std::move(game)),
					action(std::move(action)),
					possibleActions(g.possibleActions()),
					extimatedReward(0)
		{
		}

		iterator begin() { return std::begin(childres); }
		iterator end() { return std::end(childres); }

		const_iterator begin() const { return std::begin(childres); }
		const_iterator end() const { return std::end(childres); }
		[[nodiscard]] size_t childCounts() const { return childres.size(); }

		MonteCarloNode &performAction(Action action)
		{
			auto p(std::make_unique<MonteCarloNode>(action, this, g));
			childres.emplace_back(move(p));
			return *childres.back();
		}

		[[nodiscard]] size_t possibleActionsCount() const
		{
			return possibleActions.size();
		}

		[[nodiscard]] bool isFullyExpanded() const
		{
			return childCounts() == possibleActionsCount();
		}

		[[nodiscard]] size_t visitsCount() const { return visits; }

		[[nodiscard]] double playerOneWinRatio() const
		{
			return (double(extimatedReward) / double(visitsCount()));
		}

		[[nodiscard]] double getExtimatedReward(bool playerOne) const
		{
			return playerOne ? playerOneWinRatio() : 1.0 - playerOneWinRatio();
		}

		double ubc(const MonteCarloNode &child) const
		{
			return (child.getExtimatedReward(isPlayerOneNode())) +
						 (ExplorationRate *
							std::sqrt(2 * std::log(visitsCount()) / child.visitsCount()));
		}

		MonteCarloNode &bestChild() { return **std::max_element(begin(), end()); }

		const MonteCarloNode &bestChild() const
		{
			return **std::max_element(begin(), end());
		}

		void addReward(double reward) { extimatedReward += reward; }

		void visit() { visits++; }
		MonteCarloNode *getParent() { return parent; }
		const MonteCarloNode *getParent() const { return parent; }

		[[nodiscard]] bool isTerminal() const { return g.isTerminal(); }

		[[nodiscard]] bool isPlayerOneNode() const
		{
			return g.currentPlayerID() == 0;
		}

		MonteCarloNode &operator[](size_t index) { return *childres[index]; }
		const MonteCarloNode &operator[](size_t index) const
		{
			return *childres[index];
		}

		G &getGame() { return g; }
		const G &getGame() const { return g; }
		Action getAction() const { return action; }

		bool operator<(const MonteCarloNode &other) const
		{
			assert(parent == other.parent);
			return parent->ubc(*this) < parent->ubc(other);
		}

		private:
		MonteCarloNode *parent;
		content childres;

		G g;
		Action action;
		ActionContainer possibleActions;

		size_t visits{ 0 };
		double extimatedReward;
	};

}	 // namespace AI
