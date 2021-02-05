AIAddLibrary(Game 
	src/Game.cpp 
	src/InwardSuns.cpp 
	src/CharacterLocationMatrix.cpp 
	src/Map.cpp 
	src/Actions.cpp 
	src/Character.cpp 
	src/Location.cpp 
	src/MapElement.cpp 
	src/Activity.cpp 
	src/Event.cpp)
target_link_libraries(Game PUBLIC AI::Utils)
