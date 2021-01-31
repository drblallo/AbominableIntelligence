#include "gtest/gtest.h"
#include <cstddef>
#include <iterator>
#include <vector>

#include "AI/Game/Character.hpp"
#include "AI/Game/InwardSuns.hpp"
#include "AI/Game/Map.hpp"
#include "AI/Game/MapElement.hpp"

using namespace std;
using namespace AI;

TEST(mapTest, defaultMap)
{
	Map map;
	EXPECT_EQ(map.charactersSize(), 0);
}

TEST(mapTest, withCharacters)
{
	Map map = inwardSuns();
	LocationKey loc{ 0, 0 };
	EXPECT_TRUE(map[loc].getKey().isValid());
	EXPECT_EQ(map[loc].getKey(), loc);
	CharacterID characterID =
			map.emplaceCharacter(loc, "Feu Foy", CharacterKind::FeudalLord);
	auto& character = map.getCharacter(characterID);
	EXPECT_EQ(character.getName(), "Feu Foy");
	EXPECT_TRUE(character.isA(CharacterKind::FeudalLord));
	EXPECT_TRUE(map.getLocationKeyOf(character).isValid());
	EXPECT_EQ(map.getLocationKeyOf(character), loc);
}
