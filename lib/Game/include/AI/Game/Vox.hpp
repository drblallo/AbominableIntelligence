#pragma once

#include <string>
#include <vector>
namespace AI
{
	class Vox
	{
		public:
		std::string popMessage()
		{
			std::string message = messages.back();
			messages.pop_back();
			return message;
		}

		void putMessage(std::string message)
		{
			messages.push_back(std::move(message));
		}

		private:
		std::vector<std::string> messages;
	};
}	 // namespace AI
