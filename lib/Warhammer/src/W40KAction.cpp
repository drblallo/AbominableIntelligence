#include "AI/Warhammer/W40KAction.hpp"

#include <array>
#include <tuple>

#include "AI/Warhammer/W40KGame.hpp"

using namespace std;
using namespace AI;

#define SIZE_OF_ARGS(...)                                                      \
	std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value
#define PRE(name) static bool name(const W40kGame& game)
#define POST(name) static void name(W40kGame& game)

using ACT = W40kAction;

#define IACT(pre, post, name)                                                  \
	W40kAction(                                                                  \
			[](const W40kGame& game) -> bool pre, [](W40kGame & game) post, (name))

#define makeActions(...)                                                       \
	constexpr static size_t ActionsCount = SIZE_OF_ARGS(__VA_ARGS__);            \
	using Container = std::array<W40kAction, ActionsCount>;                      \
	constexpr W40kActionDBImpl<Container> ActionsDB                              \
	{                                                                            \
		{                                                                          \
			__VA_ARGS__                                                              \
		}                                                                          \
	}

template<PlayerEnum PlayerID>
PRE(emptyArmy)
{
	return game[PlayerID].size() == 0;
}

POST(dumpGame) { game[0].dump(); }

makeActions(ACT(emptyArmy<PLAYER1>, dumpGame, "testAction"));
