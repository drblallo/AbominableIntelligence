##############################
###        DOXYGEN         ###
##############################
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(doc 
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

##############################
###        COVERAGE        ###
##############################
if (${CMAKE_CXX_FLAGS} MATCHES ".*--coverage.*")
	find_program(LCOV NAMES lcov)
	find_program(GENHTML NAMES genhtml)

	add_custom_command(
		OUTPUT base.info
		COMMAND ${LCOV} --capture --initial --directory . --output-file ./base.info
		VERBATIM
		)

	add_custom_command(
		OUTPUT filtered.info 
		COMMAND ${LCOV} --remove ./total.info "/usr/*" "*/googletest/*" "*/build/*" --output-file ./filtered.info
		DEPENDS total.info
		VERBATIM)

	add_custom_command(
		OUTPUT total.info 
		COMMAND ${LCOV} --add-tracefile ./base.info --add-tracefile ./test.info --output-file ./total.info
		DEPENDS test.info base.info
		VERBATIM)

	add_custom_command(
		OUTPUT test.info 
		COMMAND ${LCOV} --capture --directory . --output-file ./test.info
		VERBATIM)

	add_custom_target(genCoverage
		COMMAND ${GENHTML} ./filtered.info --output-directory ./Coverage
		DEPENDS filtered.info
		BYPRODUCTS ./Coverage
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generate coverage"
		VERBATIM )

	add_custom_target(coverage COMMAND rm test.info filtered.info base.info total.info)
	add_dependencies(coverage genCoverage)
	add_dependencies(coverage test)
elseif(TRUE)
	add_custom_target(coverage
		COMMAND echo could not calculate coverage run cmake with -DCMAKE_CXX_FLAGS=--coverage
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Coverage not aviable"
		VERBATIM)
endif()
