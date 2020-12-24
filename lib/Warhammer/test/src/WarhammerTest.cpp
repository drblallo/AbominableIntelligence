#include "gtest/gtest.h"
#include <cstddef>
#include <vector>

#include "AI/Game/Concepts.hpp"
#include "AI/Warhammer/Model.hpp"
#include "AI/Warhammer/StatBlock.hpp"
#include "AI/Warhammer/Weapon.hpp"

using namespace std;
using namespace AI;

TEST(WarhammerTest, simpleTest) { EXPECT_TRUE(true); }

TEST(ModelTest, StatBlockIsConstexpr)
{
	constexpr StatBlock body(6, 1, 1, 1, 1, 1, 1, 1, 1);
	static_assert(body.get<ModelStat::WS>() == 1);
}

TEST(ModelTest, StatBlockIsConstexprAndCanBeBuilt)
{
	constexpr auto body =
			StatBlockBuilder().set<ModelStat::M>(6).set<ModelStat::BS>(4).getBlock();
	static_assert(body.get<ModelStat::M>() == 6);
}

TEST(ModelTest, StatBlockConstxperComparable)
{
	constexpr auto body =
			StatBlockBuilder().set<ModelStat::M>(6).set<ModelStat::BS>(4).getBlock();
	constexpr auto body2 =
			StatBlockBuilder().set<ModelStat::M>(6).set<ModelStat::BS>(7).getBlock();
	static_assert(body < body2);
	static_assert(body != body2);
}

TEST(ModelTest, WeaponStatBlockBuilder)
{
	constexpr auto weaponStats = WeaponBlockBuilder()
																	 .set<WeaponStat::S>(1)
																	 .set<WeaponStat::TYPE>(ASSAULT)
																	 .getBlock();
	static_assert(weaponStats.get<WeaponStat::S>() == 1);
	static_assert(weaponStats.get<WeaponStat::TYPE>() == ASSAULT);
}

TEST(ModelTest, WeaponStatOperatorQuad)
{
	constexpr auto weaponStats = WeaponBlockBuilder()
																	 .set<WeaponStat::S>(1)
																	 .set<WeaponStat::TYPE>(ASSAULT)
																	 .getBlock();
	static_assert(weaponStats[WeaponStat::S] == 1);
	static_assert(weaponStats[WeaponStat::TYPE] == ASSAULT);
}
