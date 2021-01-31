#pragma once

#include <istream>
#include <iterator>
#include <ostream>
#include <string>

#include "AI/Utils/CommandPrompt.hpp"

namespace AI
{
	template<bool showPrompt, typename CommandList, typename... FixedArgs>
	void executeCommandStreamImpl(
			std::istream& in,
			std::ostream& out,
			const CommandList& lst,
			FixedArgs&... args)
	{
		if (showPrompt)
			out << ">";
		CommandPrompt prompt(lst, args...);
		std::string command;
		while (std::getline(in, command))
		{
			auto [b, mess] = prompt(command);
			if (not b)
				out << mess << std::endl;
			if (showPrompt)
				out << ">";
		}
	}
	template<typename CommandList, typename... FixedArgs>
	void executeCommandStreamNoPrompt(
			std::istream& in,
			std::ostream& out,
			const CommandList& lst,
			FixedArgs&... args)
	{
		return executeCommandStreamImpl<false, CommandList, FixedArgs...>(
				in, out, lst, args...);
	}

	template<typename CommandList, typename... FixedArgs>
	void executeCommandStream(
			std::istream& in,
			std::ostream& out,
			const CommandList& lst,
			FixedArgs&... args)
	{
		return executeCommandStreamImpl<true, CommandList, FixedArgs...>(
				in, out, lst, args...);
	}

	template<typename CommandList, typename... FixedArgs>
	void executeCommandStream(
			bool showPrompt,
			std::istream& in,
			std::ostream& out,
			const CommandList& lst,
			FixedArgs&... args)
	{
		if (showPrompt)
			executeCommandStreamImpl<true, CommandList, FixedArgs...>(
					in, out, lst, args...);
		else
			executeCommandStreamImpl<false, CommandList, FixedArgs...>(
					in, out, lst, args...);
	}
}	 // namespace AI
