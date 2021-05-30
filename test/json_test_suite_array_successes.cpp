#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JSON_TEST_SUITE_ARRAY_SUCCESSES
#include <boost/test/unit_test.hpp>

#include "parser.h"

JSON_PARSER_EXPECT_SUCCESS(y_array_arraysWithSpaces,    R"json(  [[]   ]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_array_empty,               R"json(  []  )json")
JSON_PARSER_EXPECT_SUCCESS(y_array_empty_string,        R"json(  [""]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_array_ending_with_newline, R"json(  ["a"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_array_false,               R"json(  [false]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_array_heterogeneous,       R"json(  [null, 1, "1", {}]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_array_null,                R"json(  [null]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_array_with_1_and_newline,  R"json(  [1 ]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_array_with_leading_space,  R"json(   [1]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_array_with_several_null,   R"json(  [1,null,null,null,2]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_array_with_trailing_space, R"json(  [2]   )json")