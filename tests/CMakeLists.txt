find_package(GTest)
include(GoogleTest)

add_executable(test-frac-form
	TestParser.cpp
)
target_link_libraries(test-frac-form PUBLIC frac-form GTest::gtest_main)
gtest_discover_tests(test-frac-form)
