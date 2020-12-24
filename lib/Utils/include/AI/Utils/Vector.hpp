#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <ostream>
namespace AI
{
	template<typename Type, size_t Dim>
	class Vector
	{
		public:
		using Container = std::array<Type, Dim>;
		using iterator = typename Container::iterator;
		using const_iterator = typename Container::const_iterator;

		template<typename... Args>
		explicit constexpr Vector(Args... args)
				: values({ static_cast<Type>(args)... })
		{
		}

		constexpr Vector(): values() {}

		explicit constexpr Vector(const std::array<Type, Dim> val): values(val) {}

		constexpr int operator<=>(const Vector& other) const
		{
			for (size_t i = 0; i < Dim; i++)
			{
				if (values[i] < other.values[i])
					return -1;
				if (values[i] > other.values[i])
					return 1;
			}

			return 0;
		}

		constexpr bool operator==(const Vector& other) const
		{
			for (size_t i = 0; i < Dim; i++)
			{
				if (values[i] != other.values[i])
					return false;
			}
			return true;
		}

		constexpr Vector operator+(const Vector other) const
		{
			auto vals = values;
			for (size_t i = 0; i < Dim; i++)
				vals[i] += other.values[i];

			return Vector(vals);
		}

		constexpr Vector operator-(const Vector other) const
		{
			auto vals = values;
			for (size_t i = 0; i < Dim; i++)
				vals[i] -= other.values[i];

			return Vector(vals);
		}

		template<typename T>
		constexpr Vector operator*(T val) const
		{
			auto vals = values;
			for (size_t i = 0; i < Dim; i++)
				vals[i] *= val;

			return Vector(vals);
		}

		template<typename T>
		constexpr Vector operator/(T val) const
		{
			auto vals = values;
			for (size_t i = 0; i < Dim; i++)
				vals[i] /= val;

			return Vector(vals);
		}

		constexpr Vector& operator+=(const Vector& other)
		{
			*this = *this + other;
			return *this;
		}

		constexpr Vector& operator-=(const Vector& other)
		{
			*this = *this - other;
			return *this;
		}

		template<typename Value>
		constexpr Vector& operator*=(Value val)
		{
			*this = *this * val;
			return *this;
		}

		template<typename Value>
		constexpr Vector& operator/=(Value val)
		{
			*this = *this / val;
			return *this;
		}

		template<size_t Field>
		[[nodiscard]] constexpr const Type& get() const
		{
			static_assert(Field < Dim);
			return values[Field];
		}

		template<size_t Field>
		[[nodiscard]] constexpr Type& get()
		{
			static_assert(Field < Dim);
			return values[Field];
		}

		[[nodiscard]] constexpr const Type& getX() const { return get<0>(); }
		[[nodiscard]] constexpr Type& getX() { return get<0>(); }

		[[nodiscard]] constexpr const Type& getY() const { return get<1>(); }
		[[nodiscard]] constexpr Type& getY() { return get<1>(); }

		[[nodiscard]] constexpr const Type& getZ() const { return get<2>(); }
		[[nodiscard]] constexpr Type& getZ() { return get<2>(); }

		void print(std::ostream& OS, size_t indent = 0) const
		{
			for (size_t i = 0; i < indent; i++)
				OS << "\t";

			for (size_t i = 0; i < Dim - 1; i++)
				OS << values[i] << ", ";

			OS << values.back();
		}

		iterator begin() { return values.begin(); }
		iterator end() { return values.end(); }
		const_iterator begin() const { return values.begin(); }
		const_iterator end() const { return values.end(); }

		constexpr const Type& operator[](size_t index) const
		{
			return values.at(index);
		}

		private:
		Container values;
	};

	using FVector2 = Vector<float, 2>;
	using FVector3 = Vector<float, 3>;
	using U8Vector3 = Vector<std::uint8_t, 3>;
	using U8Vector2 = Vector<std::uint8_t, 2>;

}	 // namespace AI
