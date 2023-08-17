add_library(frac-form
	include/Parser.h
	Parser.cpp
)
target_include_directories(frac-form PUBLIC include)
target_link_libraries(frac-form PUBLIC Boost::boost)
