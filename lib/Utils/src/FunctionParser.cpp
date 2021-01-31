#include "AI/Utils/FunctionParser.hpp"

using namespace AI;
using namespace std;

template<>
tuple<bool, string, string> AI::from_string<string>(const string& str)
{
	return make_tuple(true, "", str);
}

template<>
tuple<bool, string, int> AI::from_string<int>(const string& str)
{
	char* next;
	int res = static_cast<int>(strtol(str.c_str(), &next, 10));
	if (*next)
		return std::make_tuple(false, "\"" + str + "\" is not a number", res);
	else
		return std::make_tuple(true, "", res);
}
template<>
tuple<bool, string, unsigned long> AI::from_string<unsigned long>(
		const string& str)
{
	char* next;
	long res = strtol(str.c_str(), &next, 10);
	if (*next)
		return std::make_tuple(false, "\"" + str + "\" is not a number", res);
	else
		return std::make_tuple(true, "", res);
}

template<>
tuple<bool, string, long> AI::from_string<long>(const string& str)
{
	char* next;
	long res = strtol(str.c_str(), &next, 10);
	if (*next)
		return std::make_tuple(false, "\"" + str + "\" is not a number", res);
	else
		return std::make_tuple(true, "", res);
}

template<>
std::string AI::typeToString<string>()
{
	return "string";
}

template<>
std::string AI::typeToString<int>()
{
	return "int";
}

template<>
std::string AI::typeToString<long>()
{
	return "long";
}

template<>
std::string AI::typeToString<unsigned long>()
{
	return "unsigned long";
}
