set(target tests)

file(GLOB core_sources "*.cpp")

INCLUDE_DIRECTORIES(
	${Boost_INCLUDE_DIRS}
	${JSONCPP_INCLUDE_DIR}
	${PROJECT_SOURCE_DIR}/include
	catch/single_include
)

set(libraries
	${Boost_LIBRARIES}
	ginkgo
)

add_executable(${target} ${core_sources})
target_link_libraries(${target} ${libraries})

add_custom_target(run-tests
	COMMAND ${CMAKE_BINARY_DIR}/bin/tests
	DEPENDS ${target}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
