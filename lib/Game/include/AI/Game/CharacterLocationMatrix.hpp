#pragma once

#include <iterator>
#include <map>
#include <optional>
#include <vector>

#include "AI/Game/Character.hpp"
#include "AI/Game/MapElement.hpp"

namespace AI
{
	class CharacterLocationMatrix
	{
		public:
		using Container = std::multimap<LocationKey, CharacterID>;
		using iterator = Container::iterator;
		using const_iterator = Container::const_iterator;

		const LocationKey& operator[](CharacterID id) const
		{
			if (locationOfChar.size() < id + 1)
				locationOfChar.resize(id + 1, LocationKey::invalid());

			return locationOfChar[id];
		}

		std::pair<const_iterator, const_iterator> operator[](LocationKey key) const
		{
			return charsOfLocation.equal_range(key);
		}

		const_iterator begin() const { return charsOfLocation.begin(); }
		const_iterator end() const { return charsOfLocation.end(); }

		void setCharacterLocation(CharacterID id, LocationKey key)
		{
			removeCharacterFromLocation(id);
			get(id) = key;
			charsOfLocation.insert(std::make_pair(key, id));
		}

		void removeCharacterFromLocation(CharacterID id);

		[[nodiscard]] size_t characterCount(LocationKey key) const
		{
			auto pair = charsOfLocation.equal_range(key);
			return std::distance(pair.first, pair.second);
		}

		private:
		LocationKey& get(CharacterID id)
		{
			if (locationOfChar.size() < id + 1)
				locationOfChar.resize(id + 1, LocationKey::invalid());

			return locationOfChar[id];
		}

		mutable std::vector<LocationKey> locationOfChar;
		Container charsOfLocation;
	};

	class CharacterOwnershipMatrix
	{
		public:
		using Container = std::map<LocationKey, CharacterID>;
		using iterator = Container::iterator;
		using const_iterator = Container::const_iterator;

		const LocationKey& operator[](CharacterID id) const
		{
			if (locationOwned.size() < id + 1)
				locationOwned.resize(id + 1, LocationKey::invalid());

			return locationOwned[id];
		}

		std::optional<CharacterID> operator[](LocationKey key) const
		{
			auto maybeRes = ownerCharacter.find(key);
			if (maybeRes == ownerCharacter.end())
				return std::nullopt;
			return maybeRes->second;
		}

		const_iterator begin() const { return ownerCharacter.begin(); }
		const_iterator end() const { return ownerCharacter.end(); }

		void setOwnedLocation(CharacterID id, LocationKey key)
		{
			removedOwnedLocation(id);
			auto maybeOldController = operator[](key);
			if (maybeOldController.has_value())
				removedOwnedLocation(*maybeOldController);

			get(id) = key;
			ownerCharacter[key] = id;
		}

		void removedOwnedLocation(CharacterID id);

		private:
		LocationKey& get(CharacterID id)
		{
			if (locationOwned.size() < id + 1)
				locationOwned.resize(id + 1, LocationKey::invalid());

			return locationOwned[id];
		}

		mutable std::vector<LocationKey> locationOwned;
		Container ownerCharacter;
	};
}	 // namespace AI
