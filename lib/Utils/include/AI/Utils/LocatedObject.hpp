#pragma once

#include <initializer_list>
#include <type_traits>
namespace AI
{
	template<typename T, typename VectorType>
	class LocatedObject
	{
		public:
		using position_type = VectorType;
		using value_type = T;
		using scalar_position_type = typename VectorType::value_type;

		constexpr explicit LocatedObject(): actual(), position() {}

		template<typename... Args>
		constexpr explicit LocatedObject(position_type pos, Args... args)
				: actual(args...), position(pos)
		{
			static_assert(
					std::is_trivial_v<T>,
					"Located Object is optimized for trivial types, if you want to use "
					"it on a non trivial type, roll your own");
		}

		template<typename... Args>
		constexpr explicit LocatedObject(
				std::initializer_list<scalar_position_type> pos, Args... args)
				: actual(args...), position(pos)
		{
			static_assert(
					std::is_trivial_v<T>,
					"Located Object is optimized for trivial types, if you want to use "
					"it on a non trivial type, roll your own");
		}

		template<typename First, typename... Args>
		constexpr explicit LocatedObject(First arg, Args... args)
				: actual(arg, args...), position()
		{
			static_assert(not std::is_same_v<First, position_type>);
			static_assert(
					std::is_trivial_v<T>,
					"Located Object is optimized for trivial types, if you want to use "
					"it on a non trivial type, roll your own");
		}

		constexpr int operator<=>(const LocatedObject& other) const noexcept =
				default;

		[[nodiscard]] constexpr const scalar_position_type& getX() const
		{
			return position.getX();
		}
		[[nodiscard]] constexpr scalar_position_type& getX()
		{
			return position.getX();
		}

		[[nodiscard]] constexpr const scalar_position_type& getY() const
		{
			return position.getY();
		}
		[[nodiscard]] constexpr scalar_position_type& getY()
		{
			return position.getY();
		}

		[[nodiscard]] constexpr const scalar_position_type& getZ() const
		{
			return position.getZ();
		}
		[[nodiscard]] constexpr scalar_position_type& getZ()
		{
			return position.getZ();
		}

		constexpr value_type* operator->() { return &actual; }
		[[nodiscard]] constexpr const value_type* operator->() const
		{
			return &actual;
		}

		constexpr value_type& operator*() { return actual; }
		[[nodiscard]] constexpr const value_type& operator*() const
		{
			return actual;
		}

		[[nodiscard]] constexpr const position_type& getPos() const
		{
			return position;
		}

		[[nodiscard]] constexpr position_type& getPos() { return position; }

		private:
		value_type actual;
		position_type position;
	};
}	 // namespace AI
