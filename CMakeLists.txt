cmake_minimum_required(VERSION 3.10)
project(min_cut)

# The version number.
set (min_cut_VERSION_MAJOR 1)
set (min_cut_VERSION_MINOR 0)

# variables used for in experiments,
# they define the number of trials to run in each experiment
set(TRIALS "8" CACHE STRING "")
set(V_MIN_POW_2 "15" CACHE STRING "")
set(V_MAX_POW_2 "21" CACHE STRING "")

set(CMAKE_CXX_STANDARD 14)

# add the deterministic-solution library?
option (USE_DETERMINISTIC "Use the deterministic implementation" ON)
if (USE_DETERMINISTIC)
    include_directories ("${PROJECT_SOURCE_DIR}/deterministic-solution")
    add_subdirectory (deterministic-solution)
    # set (EXTRA_LIBS ${EXTRA_LIBS} binary-heap-pq)
endif (USE_DETERMINISTIC)

# add the probabilistic-solution library?
option (USE_PROBABILISTIC "Use the probabilistic implementation" ON)
if (USE_PROBABILISTIC)
    include_directories ("${PROJECT_SOURCE_DIR}/probabilistic-solution")
    add_subdirectory (probabilistic-solution)
    # set (EXTRA_LIBS ${EXTRA_LIBS} leftist-heap-pq)
endif (USE_PROBABILISTIC)

# add the hybrid-solution library?
option (USE_HYBRID "Use the hybrid implementation" ON)
if (USE_HYBRID)
    include_directories ("${PROJECT_SOURCE_DIR}/hybrid-solution")
    add_subdirectory (hybrid-solution)
    # set (EXTRA_LIBS ${EXTRA_LIBS} skew-heap-pq)
endif (USE_HYBRID)

# add experiments library?
option(USE_EXPERIMENTS "Use experiments implementation" ON)
if (USE_EXPERIMENTS)
    include_directories ("${PROJECT_SOURCE_DIR}/experiments")
    add_subdirectory (experiments)
    # set (EXTRA_LIBS ${EXTRA_LIBS} experiments)
endif (USE_EXPERIMENTS)

message("OPTIONS INFO:")
message("USE_DETERMINISTIC  " ${USE_DETERMINISTIC})
message("USE_PROBABILISTIC  " ${USE_PROBABILISTIC})
message("USE_HYBRID         " ${USE_HYBRID})
message("USE_EXPERIMENTS    " ${USE_EXPERIMENTS})
message("TRIALS             " ${TRIALS})
message("V_MIN_POW_2        " ${V_MIN_POW_2})
message("V_MAX_POW_2        " ${V_MAX_POW_2})

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
)

# add the binary tree to the search path for include files
# so that we will find config.h
include_directories("${PROJECT_BINARY_DIR}")

add_executable(priority_queues main.cpp config.h.in experiments/experiments.h deterministic-solution/min-cut-deterministic.h hybrid-solution/min-cut-hybrid.h probabilistic-solution/min-cut-probabilistic.h graph.h)
# target_link_libraries (priority_queues $(EXTRA_LIBS))

# add the install targets
install (TARGETS priority_queues DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)