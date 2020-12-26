AIAddLibrary(Warhammer src/Warhammer.cpp src/StatBlock.cpp src/Weapon.cpp src/Model.cpp src/Army.cpp src/W40KAction.cpp)
target_link_libraries(Warhammer PUBLIC AI::Game AI::Utils)
