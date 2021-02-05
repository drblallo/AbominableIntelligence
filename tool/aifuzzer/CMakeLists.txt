if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	AIAddTool(
		aifuzzer
		AI::Game)	
	target_compile_options(aifuzzer PRIVATE -g -O1 -fsanitize=fuzzer)
	target_link_libraries(aifuzzer PRIVATE -fsanitize=fuzzer)
endif()
