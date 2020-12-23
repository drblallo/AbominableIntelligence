##############################
###       InstallMacro     ###
##############################
macro(AIInstall target)

include(GNUInstallDirs)
INSTALL(TARGETS ${target} EXPORT ${target}Targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include)

INSTALL(EXPORT ${target}Targets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
	FILE ${target}Targets.cmake
	NAMESPACE AI::)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${target}ConfigVersion.cmake
	VERSION ${example_VERSION}
	COMPATIBILITY SameMajorVersion)

INSTALL(FILES ${target}Config.cmake ${CMAKE_CURRENT_BINARY_DIR}/${target}ConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

endmacro(AIInstall)

##############################
###    addLibraryMacro     ###
##############################
macro(AIAddLibrary target)
	add_library(${target} ${ARGN})

add_library(AI::${target} ALIAS ${target})

target_include_directories(${target}
	PRIVATE 
		src 
	PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
		)

target_compile_features(${target} PUBLIC cxx_std_20)
AIInstall(${target})

add_subdirectory(test)
	

endmacro(AIAddLibrary)


##############################
### AIAddTestMacro   ###
##############################
macro(AIAddTest target)
	include(GoogleTest)

	add_executable(${target}Test ${ARGN})
	add_executable(AI::${target}Test ALIAS ${target}Test) 
	target_link_libraries(${target}Test PRIVATE gtest gtest_main ${target})
	target_include_directories(${target}Test PUBLIC include PRIVATE src)
	target_compile_features(${target}Test PUBLIC cxx_std_20)

	gtest_add_tests(TARGET     ${target}Test
					TEST_SUFFIX .noArgs
					TEST_LIST   noArgsTests
	)

endmacro(AIAddTest)

##############################
###  AIAddToolMacro  ###
##############################
macro(AIAddTool target)

	add_executable(${target} src/Main.cpp)
	add_executable(AI::${target} ALIAS ${target})

	target_link_libraries(${target} PUBLIC ${ARGN})
	target_compile_features(${target} PUBLIC cxx_std_20)

	include(GNUInstallDirs)
	INSTALL(TARGETS ${target} RUNTIME DESTINATION bin)

	add_subdirectory(test)
	

endMacro(AIAddTool)
