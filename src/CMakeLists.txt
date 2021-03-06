set(target ginkgo)

file(GLOB core_sources "ginkgo/*.cpp")
file(GLOB core_headers "../include/ginkgo/*.h")

file(GLOB gtk-widgets_sources "ginkgo/gtk-widgets/*.cpp")
file(GLOB gtk-widgets_headers "../include/ginkgo/gtk-widgets/*.h")

file(GLOB feedback-loop_sources "ginkgo/feedback-loop/*.cpp")
file(GLOB feedback-loop_headers "../include/ginkgo/feedback-loop/*.h")

file(GLOB feedback-loop-production_sources "ginkgo/feedback-loop/production/*.cpp")
file(GLOB feedback-loop-production_headers "../include/ginkgo/feedback-loop/production/*.h")

file(GLOB feedback-loop-consumption_sources "ginkgo/feedback-loop/consumption/*.cpp")
file(GLOB feedback-loop-consumption_headers "../include/ginkgo/feedback-loop/consumption/*.h")

file(GLOB feedback-loop-analysis_sources "ginkgo/feedback-loop/analysis/*.cpp")
file(GLOB feedback-loop-analysis_headers "../include/ginkgo/feedback-loop/analysis/*.h")

file(GLOB feedback-loop-evaluation_sources "ginkgo/feedback-loop/evaluation/*.cpp")
file(GLOB feedback-loop-evaluation_headers "../include/ginkgo/feedback-loop/evaluation/*.h")

file(GLOB solving_sources "ginkgo/solving/*.cpp")
file(GLOB solving_headers "../include/ginkgo/solving/*.h")

file(GLOB utils_sources "ginkgo/utils/*.cpp")
file(GLOB utils_headers "../include/ginkgo/utils/*.h")

include_directories(
	${Boost_INCLUDE_DIRS}
	${Clingo_INCLUDE_DIRS}
	${JSONCPP_INCLUDE_DIR}
)

if(BUILD_GTK3_GUI_TOOLS)
	include_directories(
		${GTKmm_INCLUDE_DIRS}
	)
endif(BUILD_GTK3_GUI_TOOLS)

include_directories(
	${PROJECT_SOURCE_DIR}/include
)

set(sources
	${core_sources}
	${core_headers}

	${direct-feedback-benchmark_sources}
	${direct-feedback-benchmark_headers}

	${instance-generation_sources}
	${instance-generation_headers}

	${feedback-loop_sources}
	${feedback-loop_headers}

	${feedback-loop-production_sources}
	${feedback-loop-production_headers}

	${feedback-loop-consumption_sources}
	${feedback-loop-consumption_headers}

	${feedback-loop-analysis_sources}
	${feedback-loop-analysis_headers}

	${solving_sources}
	${solving_headers}

	${utils_sources}
	${utils_headers}
)

if(BUILD_GTK3_GUI_TOOLS)
	set(sources
		${sources}

		${direct-feedback-evaluation_sources}
		${direct-feedback-evaluation_headers}

		${gtk-widgets_sources}
		${gtk-widgets_headers}

		${feedback-loop-evaluation_sources}
		${feedback-loop-evaluation_headers}
	)
endif(BUILD_GTK3_GUI_TOOLS)

set(libraries
	${Boost_LIBRARIES}
	${Clingo_LIBRARIES}
	${JSONCPP_LIBRARY}
	pthread
)

if(BUILD_GTK3_GUI_TOOLS)
	set(libraries
		${libraries}

		${GTKmm_LIBRARIES}
	)
endif(BUILD_GTK3_GUI_TOOLS)

add_library(${target} ${sources})
target_link_libraries(${target} ${libraries})
