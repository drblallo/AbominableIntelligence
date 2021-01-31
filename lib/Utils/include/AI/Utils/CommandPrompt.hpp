#pragma once
#include <cctype>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

#include "AI/Utils/FunctionParser.hpp"

namespace AI
{
	template<typename... Args>
	class Command
	{
		public:
		using FunctionType = void (*)(Args...);

		constexpr Command(FunctionType f, const char* name): fun(f), name(name) {}
		template<typename... ExtraArgs>
		[[nodiscard]] std::pair<bool, std::string> operator()(
				const std::string& command, ExtraArgs&&... args) const
		{
			return parseInvoke(fun, command, std::forward<ExtraArgs>(args)...);
		}

		[[nodiscard]] const char* getName() const { return name; }

		private:
		FunctionType fun;
		const char* const name;
	};

	template<int index>
	int commandListFind(const std::string&)
	{
		return -1;
	}

	template<int index = 0, typename Head, typename... Rest>
	int commandListFind(
			const std::string& commandName,
			const Head& firstCommand,
			const Rest&... otherCommands)
	{
		if (commandName == firstCommand.getName())
			return index;

		return commandListFind<index + 1>(commandName, otherCommands...);
	}

	template<int index = 0, typename Tuple, typename... ExtraArgs>
	std::pair<bool, std::string> invokeNth(
			int toBeInvoked,
			const Tuple& t,
			const std::string& stringArgs,
			ExtraArgs&&... args)
	{
		if constexpr (std::tuple_size_v<Tuple> <= index)
		{
			return std::make_pair(false, "Out of bound invocation");
		}
		else
		{
			if (toBeInvoked == index)
				return std::get<index>(t)(stringArgs, std::forward<ExtraArgs>(args)...);
			return invokeNth<index + 1, Tuple, ExtraArgs...>(
					toBeInvoked, t, stringArgs, std::forward<ExtraArgs>(args)...);
		}
	}

	template<typename... Command>
	class CommandList
	{
		public:
		constexpr CommandList(Command... command)
				: commands(std::make_tuple(command...))
		{
		}

		template<typename... Args>
		constexpr CommandList<Command..., AI::Command<Args...>> add(
				AI::Command<Args...> newCmd) const
		{
			const auto& fun = [&newCmd](const Command&... oldCmd) {
				return CommandList<Command..., AI::Command<Args...>>(oldCmd..., newCmd);
			};
			return std::apply(fun, commands);
		}

		template<typename... Args>
		constexpr CommandList<Command..., AI::Command<Args...>> add(
				void (*f)(Args...), const char* name) const
		{
			return add(AI::Command(f, name));
		}

		template<typename... ExtraArgs>
		[[nodiscard]] std::pair<bool, std::string> findAndInvoke(
				const std::string& commandName,
				const std::string& stringArgs,
				ExtraArgs&&... args) const
		{
			int index = std::apply(
					[&commandName](const auto&... commands) {
						return commandListFind(commandName, commands...);
					},
					commands);
			if (index == -1)
				return std::make_pair(false, "no known command named " + commandName);

			return invokeNth(
					index, commands, stringArgs, std::forward<ExtraArgs>(args)...);
		}

		private:
		std::tuple<Command...> commands;
	};

	template<typename... Args>
	constexpr CommandList<Command<Args...>> makeCommandList(
			void (*f)(Args...), const char* name)
	{
		return CommandList(Command(f, name));
	}

	template<typename CommandList, typename... FixedParameters>
	class CommandPrompt
	{
		public:
		constexpr CommandPrompt(
				CommandList& commands, FixedParameters&... parameters)
				: commandList(commands),
					parameters(std::make_tuple<FixedParameters*...>(&parameters...))
		{
		}

		[[nodiscard]] std::pair<bool, std::string> findAndInvoke(
				const std::string& commandName, const std::string& stringArgs) const
		{
			const auto toBeInvoked =
					[this, &commandName, &stringArgs](FixedParameters*... extraArgs) {
						return commandList.findAndInvoke(
								commandName, stringArgs, *extraArgs...);
					};

			return std::apply(toBeInvoked, parameters);
		}

		[[nodiscard]] std::pair<bool, std::string> operator()(
				const std::string& command) const
		{
			auto commandNameEnd = std::find(command.begin(), command.end(), ' ');
			auto nextWordStart = commandNameEnd;
			while (std::isspace(*nextWordStart))
				nextWordStart++;

			return findAndInvoke(
					std::string(command.begin(), commandNameEnd),
					std::string(nextWordStart, command.end()));
		}

		private:
		CommandList& commandList;
		std::tuple<FixedParameters*...> parameters;
	};
}	 // namespace AI
