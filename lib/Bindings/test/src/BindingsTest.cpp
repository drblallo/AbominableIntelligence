#include "gtest/gtest.h"
extern "C"
{
#include "AI/Bindings/Bindings.h"
}

TEST(BindingsTest, simpleTest)
{
	auto* game = gameCreate(0);
	gameSkipDays(game, 1);
	gameDestroy(game);
}
