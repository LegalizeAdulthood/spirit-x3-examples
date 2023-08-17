cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(frac-form)

include(CTest)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(Boost_NO_WARN_NEW_VERSIONS 1)
find_package(Boost REQUIRED)

add_subdirectory(frac-form)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
