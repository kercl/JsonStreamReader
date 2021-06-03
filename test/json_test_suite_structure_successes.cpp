#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JSON_TEST_SUITE_STRUCTURE_SUCCESSES
#include <boost/test/unit_test.hpp>

#include "TestParser.h"

JSON_PARSER_EXPECT_SUCCESS(y_structure_lonely_false,         R"json(  false  )json")
JSON_PARSER_EXPECT_SUCCESS(y_structure_lonely_int,           R"json(  42  )json")
JSON_PARSER_EXPECT_SUCCESS(y_structure_lonely_negative_real, R"json(  -0.1  )json")
JSON_PARSER_EXPECT_SUCCESS(y_structure_lonely_null,          R"json(  null  )json")
JSON_PARSER_EXPECT_SUCCESS(y_structure_lonely_string,        R"json(  "asd"  )json")
JSON_PARSER_EXPECT_SUCCESS(y_structure_lonely_true,          R"json(  true  )json")
JSON_PARSER_EXPECT_SUCCESS(y_structure_string_empty,         R"json(  ""  )json")
JSON_PARSER_EXPECT_SUCCESS(y_structure_trailing_newline,     "  [\"a\"]\n  ")
JSON_PARSER_EXPECT_SUCCESS(y_structure_true_in_array,        R"json(  [true]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_structure_whitespace_array,     R"json(   []   )json")
