#include "gtest/gtest.h"
#include <cstddef>
#include <vector>

#include "AI/Utils/SizedArray.hpp"

using namespace AI;
using namespace std;

TEST(UtilsTest, SizedArrayTest)
{
	SizedArray<int, 5> array(1, 2, 3);
	EXPECT_EQ(std::distance(array.begin(), array.end()), 3);
}

TEST(UtilsTest, SizedArrayPushBack)
{
	SizedArray<int, 5> array(1, 2, 3);
	array.push_back(4);
	EXPECT_EQ(std::distance(array.begin(), array.end()), 4);
	for (int a = 0; a < 4; a++)
		EXPECT_EQ(array[a], a + 1);
}
