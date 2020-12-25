#pragma once
#include <array>
#include <initializer_list>
#include <limits>
#include <type_traits>

namespace AI
{
	template<typename T, size_t Size, typename size_type = size_t>
	class SizedArray
	{
		public:
		using Container = std::array<T, Size>;

		using value_type = typename Container::value_type;
		using difference_type = typename Container::difference_type;
		using reference = typename Container::reference;
		using const_reference = typename Container::const_reference;
		using pointer = typename Container::pointer;
		using const_pointer = typename Container::const_pointer;
		using iterator = typename Container::iterator;
		using const_iterator = typename Container::const_iterator;
		using reverse_iterator = typename Container::reverse_iterator;
		using const_reverse_iterator = typename Container::const_reverse_iterator;

		template<typename... Content>
		constexpr SizedArray(T first, Content... args)
				: values({ first, static_cast<T>(args)... }), siz(sizeof...(args) + 1)
		{
			static_assert(Size < std::numeric_limits<size_type>::max());
			static_assert(std::is_trivial_v<T>);
		}

		constexpr SizedArray(std::initializer_list<T> lst)
				: values(), siz(lst.size())
		{
			auto it = lst.begin();
			for (size_t i = 0; i < siz; i++)
				values[i] = *(it++);
		}

		constexpr SizedArray(): values(), siz(0) {}

		constexpr reference at(size_t pos) { return values.at(pos); }
		constexpr const_reference at(size_t pos) const { return values.at(pos); }

		constexpr reference operator[](size_t pos) { return values[pos]; }
		constexpr const_reference operator[](size_t pos) const
		{
			return values[pos];
		}

		constexpr reference front() { return values.front(); }
		constexpr const_reference front() const { return values.front(); }

		constexpr reference back() { return values.at(siz - 1); }
		constexpr const_reference back() const { return values.at(siz - 1); }

		value_type* data() noexcept { return values.data(); }
		const value_type* data() const noexcept { return values.data(); }

		constexpr size_type size() const { return siz; }
		constexpr size_type max_size() const { return values.max_size(); }

		[[nodiscard]] constexpr bool empty() const { return size() == 0; }

		iterator begin() { return values.begin(); }
		iterator end() { return values.begin() + siz; }
		const_iterator begin() const { return values.begin(); }
		const_iterator end() const { return values.begin() + siz; }

		const_iterator cbegin() const { return begin(); }
		const_iterator cend() const { return end(); }

		reverse_iterator rbegin() { return values.rend() - siz; }
		reverse_iterator rend() { return values.rend(); }
		const_reverse_iterator rbegin() const { return values.rend() - siz; }
		const_reverse_iterator rend() const { return values.rend(); }

		const_reverse_iterator crbegin() const { return rbegin(); }
		const_reverse_iterator crend() const { return rend(); }

		void swap(SizedArray& other)
		{
			values.swap(other.values);
			std::swap(siz, other.siz);
		}

		[[nodiscard]] constexpr int operator<=>(const SizedArray& other) const
		{
			for (size_t i = 0; i < std::min(siz, other.siz); i++)
			{
				if (at(i) < other.at(i))
					return -1;
				if (at(i) > other.at(i))
					return 1;
			}

			if (siz < other.siz)
				return -1;
			if (siz > other.siz)
				return 1;

			return 0;
		}

		[[nodiscard]] constexpr bool operator==(const SizedArray& other) const
		{
			if (siz != other.siz)
				return false;

			for (size_t i = 0; i < siz; i++)
				if (values[i] != other[i])
					return false;
			return true;
		}

		constexpr void push_back(const value_type& val)
		{
			values.at(siz) = val;
			siz++;
		}

		constexpr void push_back(value_type&& val)
		{
			values.at(siz) = std::move(val);
			siz++;
		}

		constexpr void clear() { siz = 0; }

		private:
		std::array<T, Size> values;
		size_type siz;
	};
}	 // namespace AI
