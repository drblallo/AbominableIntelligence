#pragma once

#include <optional>
#include <vector>

namespace AI
{
	template<typename NumberType>
	class Hierarchy
	{
		public:
		void setSuperrior(NumberType element, NumberType newSuperrior)
		{
			assert(not isAncestor(newSuperrior, element));
			removeSuperior(element);
			getSuperior(element) = newSuperrior;
			getInferiors(newSuperrior).emplace_back(element);
		}

		void removeSuperior(NumberType element)
		{
			auto& superior = getSuperior(element);
			if (not superior.has_value())
				return;

			std::erase(getInferiors(*superior), element);
			superior = std::nullopt;
		}

		const std::vector<NumberType>& inferiorsOf(NumberType element) const
		{
			if (inferiors.size() <= element)
				inferiors.resize(element + 1, {});
			return inferiors[element];
		}

		std::optional<NumberType> superiorOf(NumberType element) const
		{
			if (superriors.size() <= element)
				superriors.resize(element + 1, std::nullopt);
			return superriors[element];
		}

		bool isAncestor(NumberType first, NumberType ancestor)
		{
			std::optional<NumberType> currentAcestor;
			while ((currentAcestor = getSuperior(first)).has_value())
			{
				if (*currentAcestor == ancestor)
					return true;
			}
			return false;
		}

		bool areRelated(NumberType l, NumberType r)
		{
			return isAncestor(l, r) or isAncestor(r, l);
		}

		private:
		std::vector<NumberType>& getInferiors(NumberType element)
		{
			if (inferiors.size() <= element)
				inferiors.resize(element + 1, {});
			return inferiors[element];
		}
		std::optional<NumberType>& getSuperior(NumberType element)
		{
			if (superriors.size() <= element)
				superriors.resize(element + 1, std::nullopt);
			return superriors[element];
		}

		mutable std::vector<std::optional<NumberType>> superriors;
		mutable std::vector<std::vector<NumberType>> inferiors;
	};
}	 // namespace AI
