#include "gtest/gtest.h"
#include <cstddef>
#include <vector>

#include "AI/Utils/Vector.hpp"

using namespace std;
using namespace AI;

TEST(UtilsTest, simpleTest)
{
	constexpr auto v = FVector2(1.0, 4.0);
	static_assert(v.getX() == 1.0);
	static_assert(v.getY() == 4.0);
}

TEST(UtilsTest, multiplicationTest)
{
	constexpr auto v = U8Vector2(5, 2) * 2;
	static_assert(v.getX() == 10);
	static_assert(v.getY() == 4);
}
