#include "AI/Game/Map.hpp"

#include <algorithm>

#include "AI/Game/Character.hpp"

using namespace AI;
using namespace std;

vector<Character*> Map::charactersOfLocation(LocationKey key)
{
	auto pair = locationMatrix[key];
	vector<Character*> out(distance(pair.first, pair.second));
	transform(pair.first, pair.second, out.begin(), [this](const auto& pair) {
		return &characters[pair.second];
	});
	return out;
};

vector<const Character*> Map::charactersOfLocation(LocationKey key) const
{
	auto pair = locationMatrix[key];
	vector<const Character*> out(distance(pair.first, pair.second));
	transform(pair.first, pair.second, out.begin(), [this](const auto& pair) {
		return &characters[pair.second];
	});
	return out;
};

vector<Character*> Map::getInferiorsOf(const Character& c)
{
	const auto& inf = characterHierarchy.inferiorsOf(c.getID());
	vector<Character*> toReturn(inf.size());
	transform(inf.begin(), inf.end(), toReturn.begin(), [this](CharacterID id) {
		return &getCharacter(id);
	});

	return toReturn;
}

vector<const Character*> Map::getInferiorsOf(const Character& c) const
{
	const auto& inf = characterHierarchy.inferiorsOf(c.getID());
	vector<const Character*> toReturn(inf.size());
	transform(inf.begin(), inf.end(), toReturn.begin(), [this](CharacterID id) {
		return &getCharacter(id);
	});

	return toReturn;
}
