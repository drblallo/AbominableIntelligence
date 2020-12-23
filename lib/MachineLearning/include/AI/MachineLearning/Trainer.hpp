#pragma once
#include "AI/tree/MonteCarloNode.hpp"

namespace AI
{
	template<typename Net, typename Game>
	double train(Net &net, const MonteCarloNode<Game> &Node)
	{
		auto result = net.forward(Node.getGame().toTensor());
		auto acc = result.template accessor<float, 2>();

		std::array<std::pair<std::int32_t, float>, Game::ActionsCount> scores;
		for (size_t i = 0; i < ActionsCount; i++)
		{
			auto score = i < childres.size() ? childres[0]->marineWinRatio()
																			 : acc[0][possibleActions[i].first];
			scores[i] = std::make_pair(possibleActions[i].first, score);
		}

		sort(scores.begin(), scores.end(), [](const auto &l, const auto &r) {
			return l.first < r.first;
		});

		auto target = torch::from_blob(
				(void *) &(scores[0].second),
				{ 1, ActionsCount },
				{ 0, 2 },
				torch::TensorOptions(torch::kFloat32));

		torch::Tensor error = at::mse_loss(result, target);

		error.backward();
		return error;
	}

}	 // namespace AI
