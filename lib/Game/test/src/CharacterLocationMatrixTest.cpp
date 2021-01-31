#include "gtest/gtest.h"
#include <cstddef>
#include <iterator>
#include <vector>

#include "AI/Game/CharacterLocationMatrix.hpp"
#include "AI/Game/MapElement.hpp"

using namespace std;
using namespace AI;

TEST(CharacterLocationMatrix, simpleInsert)
{
	CharacterLocationMatrix matrix;
	LocationKey key{ 1, 2 };
	matrix.setCharacterLocation(9, key);
	EXPECT_EQ(matrix[9], key);
	EXPECT_EQ(matrix.characterCount(key), 1);
	EXPECT_EQ(std::distance(matrix.begin(), matrix.end()), 1);
	auto pair = matrix[key];
	EXPECT_EQ(std::distance(pair.first, pair.second), 1);
	EXPECT_EQ(pair.first->second, 9);
}

TEST(CharacterLocationMatrix, simpleDeletion)
{
	CharacterLocationMatrix matrix;
	LocationKey key{ 1, 2 };
	matrix.setCharacterLocation(9, key);
	matrix.removeCharacterFromLocation(9);
	EXPECT_FALSE(matrix[9].isValid());
	EXPECT_EQ(matrix.characterCount(key), 0);
	EXPECT_EQ(std::distance(matrix.begin(), matrix.end()), 0);
	auto pair = matrix[key];
	EXPECT_EQ(std::distance(pair.first, pair.second), 0);
}

TEST(CharacterLocationMatrix, doubleInsert)
{
	CharacterLocationMatrix matrix;
	LocationKey key1{ 1, 3 };
	matrix.setCharacterLocation(9, key1);
	LocationKey key = { 1, 2 };
	matrix.setCharacterLocation(9, key);
	EXPECT_NE(matrix[9], key1);
	EXPECT_EQ(matrix[9], key);
	EXPECT_EQ(matrix.characterCount(key), 1);
	EXPECT_EQ(std::distance(matrix.begin(), matrix.end()), 1);
	auto pair = matrix[key];
	EXPECT_EQ(std::distance(pair.first, pair.second), 1);
	EXPECT_EQ(pair.first->second, 9);
}

TEST(CharacterOwnershipMatrix, simpleInsert)
{
	CharacterOwnershipMatrix matrix;
	LocationKey key{ 1, 2 };
	matrix.setOwnedLocation(9, key);
	EXPECT_EQ(matrix[9], key);
	EXPECT_EQ(std::distance(matrix.begin(), matrix.end()), 1);
	EXPECT_EQ(matrix[key], 9);
}

TEST(CharacterOwnershipMatrix, simpleDeletion)
{
	CharacterOwnershipMatrix matrix;
	LocationKey key{ 1, 2 };
	matrix.setOwnedLocation(9, key);
	matrix.removedOwnedLocation(9);
	EXPECT_EQ(matrix[9], LocationKey::invalid());
	EXPECT_EQ(std::distance(matrix.begin(), matrix.end()), 0);
	EXPECT_FALSE(matrix[key].has_value());
}

TEST(CharacterOwnershipMatrix, doubleInsertion)
{
	CharacterOwnershipMatrix matrix;
	LocationKey key{ 1, 2 };
	matrix.setOwnedLocation(9, key);
	matrix.setOwnedLocation(7, key);
	EXPECT_FALSE(matrix[9].isValid());
	EXPECT_EQ(matrix[7], key);
	EXPECT_EQ(std::distance(matrix.begin(), matrix.end()), 1);
	EXPECT_EQ(matrix[key], 7);
}
