#pragma once

namespace AI
{
	template<typename iterator>
	class Range
	{
		public:
		Range(iterator b, iterator e): b(b), e(e) {}
		[[nodiscard]] iterator begin() const { return b; }
		[[nodiscard]] iterator end() const { return e; }

		private:
		iterator b;
		iterator e;
	};

}	 // namespace AI
