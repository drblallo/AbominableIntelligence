#pragma once

#include <cstdlib>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

namespace AI
{
	inline std::vector<std::string> split(const std::string& s, char delimiter)
	{
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(s);
		while (std::getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}

	template<typename T>
	std::string typeToString();

	template<typename ToReturn>
	auto from_string(const std::string& str)
			-> std::tuple<bool, std::string, ToReturn>;

	template<std::size_t N, typename... T, std::size_t... I>
	std::tuple<std::tuple_element_t<N + I, std::tuple<T...>>...> sub(
			std::index_sequence<I...>);

	template<std::size_t N, typename... T>
	using subpack =
			decltype(sub<N, T...>(std::make_index_sequence<sizeof...(T) - N>{}));

	template<typename Tuple, typename Iterator, size_t index = 0>
	std::pair<bool, std::string> argumentsToTuple(Tuple& t, Iterator iterator)
	{
		if constexpr (std::tuple_size_v<Tuple> == index)
		{
			return std::make_pair(true, "");
		}
		else
		{
			using NthType = std::tuple_element_t<index, Tuple>;
			auto [s, mess, value] = from_string<NthType>(*iterator);
			if (not s)
				return make_pair(s, std::move(mess));
			std::get<index>(t) = value;
			return argumentsToTuple<Tuple, Iterator, index + 1>(t, iterator + 1);
		}
	}

	template<typename... FunctionArgs, typename... ExtraArgs>
	std::pair<bool, std::string> parseInvoke(
			void f(FunctionArgs...),
			const std::string& command,
			ExtraArgs&&... extraArgs)
	{
		auto splitted = split(command, ' ');
		if (sizeof...(FunctionArgs) != splitted.size() + sizeof...(ExtraArgs))
			return std::make_pair(false, "Missmatched number of arguments");

		using StringArgsTuple = subpack<sizeof...(extraArgs), FunctionArgs...>;

		StringArgsTuple args;
		auto [s, mess] = argumentsToTuple(args, splitted.begin());

		if (not s)
			return std::make_pair(s, mess);

		std::apply(
				[f, &extraArgs...](auto&&... args) {
					f(std::forward<ExtraArgs>(extraArgs)..., args...);
				},
				args);
		return std::make_pair(true, "");
	}

	template<typename Tuple, size_t index = 0>
	std::string typesToStringImpl()
	{
		if constexpr (std::tuple_size_v<Tuple> == index)
			return "";
		else
			return typeToString<std::tuple_element_t<index, Tuple>>() + " " +
						 typesToStringImpl<Tuple, index + 1>();
	}

	template<typename... Args, size_t index = 0>
	std::string typesToString()
	{
		return typesToStringImpl<std::tuple<Args...>>();
	}

	template<typename... Args>
	std::string fArgsToString(void(Args...))
	{
		return typesToString<Args...>();
	}

}	 // namespace AI
