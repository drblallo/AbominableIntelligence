AIAddLibrary(Warhammer src/Warhammer.cpp src/StatBlock.cpp src/Weapon.cpp)
target_link_libraries(Warhammer PUBLIC AI::Game AI::Utils)
