#include "AI/Game/CharacterLocationMatrix.hpp"

#include <algorithm>

using namespace AI;
using namespace std;

void CharacterLocationMatrix::removeCharacterFromLocation(CharacterID id)
{
	auto& location = get(id);
	if (not location.isValid())
		return;

	auto range = charsOfLocation.equal_range(location);
	auto i = std::find_if(range.first, range.second, [id](const auto& pair) {
		return pair.second == id;
	});
	assert(i != range.second);
	charsOfLocation.erase(i);

	location = LocationKey::invalid();
}

void CharacterOwnershipMatrix::removedOwnedLocation(CharacterID id)
{
	auto& location = get(id);
	if (not location.isValid())
		return;

	assert(ownerCharacter.find(location) != ownerCharacter.end());

	ownerCharacter.erase(location);
	location = LocationKey::invalid();
}
