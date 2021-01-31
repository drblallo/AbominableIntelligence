#include "gtest/gtest.h"
#include <cstddef>
#include <sstream>
#include <vector>

#include "AI/Utils/CommandPrompt.hpp"
#include "AI/Utils/FunctionParser.hpp"
#include "AI/Utils/StreamPrompt.hpp"

using namespace AI;
using namespace std;

static void fTest(string& out, string in) { out = in; }

TEST(UtilsTest, FunctionParserTest)
{
	string out;
	string in = "val";
	auto [success, msg] = parseInvoke(fTest, in, out);
	EXPECT_EQ(success, true);
	EXPECT_EQ(out, in);
}

static void magicalF(int& out, int f, int f2, int f3) { out = f + f2 + f3; }

TEST(UtilsTest, FunctionParserTestInt)
{
	int out;
	auto [success, msg] = parseInvoke(magicalF, "1 2 3", out);
	EXPECT_EQ(success, true);
	EXPECT_EQ(out, 6);
}

static void magicalF2(int& out, long f4, int f, int f2, int f3)
{
	out = f + f2 + f3 + f4;
}

TEST(UtilsTest, FunctionParserTestIntFailure)
{
	int out;
	auto [success, msg] = parseInvoke(magicalF2, "1 hey 3", out, 3);
	EXPECT_EQ(success, false);
}

static void magicalF3(int, int, int, int) {}

TEST(UtilsTest, FunctionParserTestIntNoExtraArgs)
{
	auto [success, msg] = parseInvoke(magicalF3, "1 3 3 4");
	EXPECT_EQ(success, true);
}

TEST(UtilsTest, FunctionParserTestIntNotEnoughArgs)
{
	auto [success, msg] = parseInvoke(magicalF3, "3 3 4");
	EXPECT_EQ(success, false);
}

TEST(UtilsTest, FunctionParserTestIntTooManyArgs)
{
	auto [success, msg] = parseInvoke(magicalF3, "1 1 3 3 4");
	EXPECT_EQ(success, false);
}

TEST(UtilsTest, FunctionArgsToString)
{
	EXPECT_EQ(fArgsToString(magicalF3), "int int int int ");
}

static void noArgs(int& i) { i = 10; }

TEST(UtilsTest, FunctionParserNoArgs)
{
	int i = 0;
	constexpr Command command(noArgs, "c2");
	constexpr CommandList list(command);
	auto [success, msg] = list.findAndInvoke("c2", "", i);
	EXPECT_EQ(success, true);
	EXPECT_EQ(i, 10);
}

TEST(UtilsTest, CommandInvocation)
{
	int out;
	constexpr Command command(magicalF, "sampleName");
	auto [success, msg] = command("1 2 3", out);
	EXPECT_EQ(success, true);
	EXPECT_EQ(out, 6);
}

TEST(UtilsTest, CommandListInvocation)
{
	int out;
	constexpr Command command(magicalF, "c1");
	constexpr Command command2(magicalF2, "c2");
	constexpr CommandList list(command, command2);
	auto [success, msg] = list.findAndInvoke("c1", "1 2 3", out);
	EXPECT_EQ(success, true);
	EXPECT_EQ(out, 6);
}

TEST(UtilsTest, CommandListInvocationUnkownCommand)
{
	int out;
	constexpr Command command(magicalF, "c1");
	constexpr Command command2(magicalF2, "c2");
	constexpr CommandList list(command, command2);
	auto [success, msg] = list.findAndInvoke("c12", "1 2 3", out);
	EXPECT_EQ(success, false);
}

TEST(UtilsTest, CommandListInvocationMissmatchedArguments)
{
	int out;
	constexpr Command command(magicalF, "c1");
	constexpr Command command2(magicalF2, "c2");
	constexpr CommandList list(command, command2);
	auto [success, msg] = list.findAndInvoke("c2", "1 2 3", out);
	EXPECT_EQ(success, false);
}

TEST(UtilsTest, CommandListBuilder)
{
	int out;
	constexpr Command command(magicalF, "c1");
	constexpr Command command2(magicalF2, "c2");
	constexpr CommandList list = CommandList(command).add(command2);
	auto [success, msg] = list.findAndInvoke("c2", "1 2 3", out);
	EXPECT_EQ(success, false);
}

TEST(UtilsTest, CommandListBuilder2)
{
	int out;
	constexpr Command command(magicalF, "c1");
	constexpr CommandList list = CommandList(command).add(magicalF2, "c2");
	auto [success, msg] = list.findAndInvoke("c2", "1 2 3", out);
	EXPECT_EQ(success, false);
}

TEST(UtilsTest, CommandPrompt)
{
	int out;
	constexpr Command command(magicalF, "c1");
	constexpr Command command2(magicalF2, "c2");
	constexpr CommandList list(command, command2);
	CommandPrompt prompt(list, out);
	auto [success, msg] = prompt.findAndInvoke("c2", "1 2 3 4");
	EXPECT_EQ(success, true);
	EXPECT_EQ(out, 10);
}

TEST(UtilsTest, CommandPromptBeutifull)
{
	int out;
	constexpr auto list = makeCommandList(magicalF, "c1").add(magicalF2, "c2");
	CommandPrompt prompt(list, out);
	auto [success, msg] = prompt.findAndInvoke("c2", "1 2 3 4");
	EXPECT_EQ(success, true);
	EXPECT_EQ(out, 10);
}

TEST(UtilsTest, CommandPromptBeutifull2)
{
	int out;
	constexpr auto list = makeCommandList(magicalF, "c1").add(magicalF2, "c2");
	CommandPrompt prompt(list, out);
	auto [success, msg] = prompt("c2 1 2 3 4");
	if (!success)
		std::cout << msg << "\n";
	EXPECT_EQ(success, true);
	EXPECT_EQ(out, 10);
}

TEST(UtilsTest, CommandPromptBeutifullNoArgs)
{
	int out;
	constexpr auto list = makeCommandList(noArgs, "c1").add(magicalF2, "c2");
	CommandPrompt prompt(list, out);
	auto [success, msg] = prompt("c1");
	if (!success)
		std::cout << msg << "\n";
	EXPECT_EQ(success, true);
	EXPECT_EQ(out, 10);
}

TEST(UtilsTest, executeCommandStream)
{
	int out = 0;
	constexpr auto list = makeCommandList(magicalF, "c1").add(magicalF2, "c2");
	istringstream iss("c2 1 2 3 4\nc1 1 2 3\n");

	ostringstream oss;
	executeCommandStream(iss, oss, list, out);
	string res = oss.str();
	EXPECT_EQ(out, 6);
	EXPECT_EQ(res, ">\n>\n>");
}
