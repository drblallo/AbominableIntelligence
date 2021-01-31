#pragma once

#include <cstddef>
#include <functional>
#include <limits>
#include <utility>
#include <vector>

#include "AI/Game/Character.hpp"
#include "AI/Game/CharacterLocationMatrix.hpp"
#include "AI/Game/Hierarchy.hpp"
#include "AI/Game/Location.hpp"
#include "AI/Game/MapElement.hpp"
#include "AI/Utils/Graph.hpp"
#include "AI/Utils/Range.hpp"
#include "AI/Utils/Vector.hpp"

namespace AI
{
	class Map
	{
		public:
		using CharacterContainer = std::vector<Character>;
		using character_iterator = CharacterContainer::iterator;
		using const_character_iterator = CharacterContainer::const_iterator;

		using iterator = Graph<MapElement>::iterator;
		using const_iterator = Graph<MapElement>::const_iterator;

		iterator begin() { return mapGraph.begin(); }
		const_iterator begin() const { return mapGraph.begin(); }
		iterator end() { return mapGraph.end(); }
		const_iterator end() const { return mapGraph.end(); }

		const Location& operator[](LocationKey key) const
		{
			return mapGraph[key.MaplElementID][key.LocationID];
		}
		Location& operator[](LocationKey key)
		{
			return mapGraph[key.MaplElementID][key.LocationID];
		}
		[[nodiscard]] const Location& getLocation(
				size_t mapElementID, size_t locationID) const
		{
			return mapGraph[mapElementID][locationID];
		}
		[[nodiscard]] Location& getLocation(size_t mapElementID, size_t locationID)
		{
			return mapGraph[mapElementID][locationID];
		}

		[[nodiscard]] MapElement& getMapElement(size_t mapElementID)
		{
			return mapGraph[mapElementID];
		}

		[[nodiscard]] const MapElement& getMapElement(size_t mapElementID) const
		{
			return mapGraph[mapElementID];
		}

		[[nodiscard]] const Character& getCharacter(size_t charID) const
		{
			return characters[charID];
		}
		[[nodiscard]] Character& getCharacter(size_t charID)
		{
			return characters[charID];
		}

		template<typename... Args>
		CharacterID emplaceCharacter(LocationKey location, Args&&... args)
		{
			characters.emplace_back(characters.size(), std::forward<Args>(args)...);
			setCharacterLocation(characters.back().getID(), location);
			return characters.back().getID();
		}

		template<CharacterKind kind, typename... Args>
		CharacterID emplace(Args&&... args)
		{
			return emplaceCharacter(std::forward<Args>(args)..., kind);
		}

		template<typename... Args>
		CharacterID emplaceCharacter(const Location& location, Args&&... args)
		{
			return emplaceCharacter(location.getKey(), std::forward<Args>(args)...);
		}

		template<typename... Args>
		CharacterID emplaceOwnerCharacter(const Location& location, Args&&... args)
		{
			CharacterID id =
					emplaceCharacter(location.getKey(), std::forward<Args>(args)...);
			setOwnership(id, location);
			return id;
		}
		template<CharacterKind kind, typename... Args>
		CharacterID emplaceOwner(Args&&... args)
		{
			return emplaceOwnerCharacter(std::forward<Args>(args)..., kind);
		}

		size_t addMapElement(MapElement mapElement)
		{
			mapElement.setID(mapGraph.size());
			const auto& n = mapGraph.addNode(std::move(mapElement));
			return n.getID();
		}

		template<typename... Args>
		size_t& emplaceMapElement(Args&&... args)
		{
			const auto& n =
					mapGraph.emplaceNode(mapGraph.size(), std::forward<Args>(args)...);
			return n.getID();
		}

		void setCharacterLocation(CharacterID characterID, LocationKey locationKey)
		{
			assert(locationKey.isValid());
			locationMatrix.setCharacterLocation(characterID, locationKey);
		}

		void setCharacterLocation(const Character& c, const Location& location)
		{
			setCharacterLocation(c.getID(), location.getKey());
		}

		const Location& getLocationOf(const Character& c) const
		{
			return getLocationOf(c.getID());
		}
		Location& getLocationOf(const Character& c)
		{
			return getLocationOf(c.getID());
		}
		const Location& getLocationOf(CharacterID c) const
		{
			return operator[](locationMatrix[c]);
		}
		Location& getLocationOf(CharacterID c)
		{
			return operator[](locationMatrix[c]);
		}

		LocationKey getLocationKeyOf(const Character& c) const
		{
			return getLocationKeyOf(c.getID());
		}
		LocationKey getLocationKeyOf(CharacterID c) const
		{
			return locationMatrix[c];
		}

		std::vector<Character*> charactersOfLocation(LocationKey key);

		std::vector<const Character*> charactersOfLocation(LocationKey key) const;

		size_t charactersSize() const { return characters.size(); }
		const_character_iterator charbegin() const { return characters.begin(); }
		const_character_iterator charend() const { return characters.end(); }
		character_iterator charbegin() { return characters.begin(); }
		character_iterator charend() { return characters.end(); }

		void setOwnership(CharacterID id, LocationKey key)
		{
			ownershipMatrix.setOwnedLocation(id, key);
		}

		void setOwnership(const Character& character, const Location& location)
		{
			setOwnership(character.getID(), location.getKey());
		}

		void setOwnership(const Character& character, LocationKey key)
		{
			setOwnership(character.getID(), key);
		}
		void setOwnership(CharacterID character, const Location& location)
		{
			setOwnership(character, location.getKey());
		}

		void removeOwnership(CharacterID id)
		{
			ownershipMatrix.removedOwnedLocation(id);
		}

		void removeOwnership(const Character& character)
		{
			removeOwnership(character.getID());
		}

		LocationKey getOwnershipOf(CharacterID id) const
		{
			return ownershipMatrix[id];
		}

		void setSuperrior(CharacterID inferior, CharacterID superior)
		{
			characterHierarchy.setSuperrior(inferior, superior);
		}
		void setSuperrior(const Character& inferior, const Character& superior)
		{
			characterHierarchy.setSuperrior(inferior.getID(), superior.getID());
		}
		void removeSuperior(CharacterID inferior)
		{
			characterHierarchy.removeSuperior(inferior);
		}
		void removeSuperior(const Character& inferior)
		{
			characterHierarchy.removeSuperior(inferior.getID());
		}

		Character* getSuperiorOf(const Character& c)
		{
			auto sup = characterHierarchy.superiorOf(c.getID());
			if (not sup.has_value())
				return nullptr;

			return &getCharacter(*sup);
		}
		const Character* getSuperiorOf(const Character& c) const
		{
			auto sup = characterHierarchy.superiorOf(c.getID());
			if (not sup.has_value())
				return nullptr;

			return &getCharacter(*sup);
		}

		[[nodiscard]] Range<character_iterator> getCharactersRange()
		{
			return Range(charbegin(), charend());
		}
		[[nodiscard]] Range<const_character_iterator> getCharactersRange() const
		{
			return Range(charbegin(), charend());
		}

		std::vector<Character*> getInferiorsOf(const Character& c);
		std::vector<const Character*> getInferiorsOf(const Character& c) const;

		private:
		Graph<MapElement> mapGraph;
		CharacterContainer characters;
		CharacterLocationMatrix locationMatrix;
		CharacterOwnershipMatrix ownershipMatrix;
		Hierarchy<CharacterID> characterHierarchy;
	};
}	 // namespace AI
