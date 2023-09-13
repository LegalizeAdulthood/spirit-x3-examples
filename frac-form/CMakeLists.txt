add_library(frac-form
	include/AST.h
	include/ASTAdapted.h
	include/ASTPrinter.h
	include/Complex.h
	include/FormulaParser.h
	include/Parser.h
	FormulaParser.cpp
	Parser.cpp
)
target_include_directories(frac-form PUBLIC include)
target_link_libraries(frac-form PUBLIC Boost::boost)
target_compile_features(frac-form PUBLIC cxx_std_17)
