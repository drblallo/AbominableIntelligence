#include "gtest/gtest.h"
#include <cstddef>
#include <vector>

#include "AI/Utils/LocatedObject.hpp"
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

TEST(UtilsTest, aggregateVectorInitialization)
{
	constexpr auto v = U8Vector2{ 5, 2 } * 2;
	static_assert(v.getX() == 10);
	static_assert(v.getY() == 4);
}

TEST(UtilsTest, aggregateVectorInitialization2)
{
	constexpr auto v = U8Vector2({ 5, 2 }) * 2;
	static_assert(v.getX() == 10);
	static_assert(v.getY() == 4);
}

TEST(UtilsTest, LocatedObjectTest)
{
	constexpr LocatedObject<int, FVector2> v(FVector2(10, 4), 5);
	static_assert(v.getX() == 10);
	static_assert(v.getY() == 4);
	static_assert(*v == 5);
}

TEST(UtilsTest, LocatedObjectTestAggregate)
{
	constexpr LocatedObject<int, FVector2> v({ 10, 4 }, 5);
	static_assert(v.getX() == 10);
	static_assert(v.getY() == 4);
	static_assert(*v == 5);
}
