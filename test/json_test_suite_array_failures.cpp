#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JSON_TEST_SUITE_ARRAY_FAILURES
#include <boost/test/unit_test.hpp>

#include "TestParser.h"

JSON_PARSER_EXPECT_FAILURE(n_array_1_true_without_comma,          R"json(  [1 true]     )json")
JSON_PARSER_EXPECT_FAILURE(n_array_a_invalid_utf8,                R"json(  [a�]         )json")
JSON_PARSER_EXPECT_FAILURE(n_array_colon_instead_of_comma,        R"json(  ["": 1]      )json")
JSON_PARSER_EXPECT_FAILURE(n_array_comma_after_close,             R"json(  [""],        )json")
JSON_PARSER_EXPECT_FAILURE(n_array_comma_and_number,              R"json(  [,1]         )json")
JSON_PARSER_EXPECT_FAILURE(n_array_double_comma,                  R"json(  [1,,2]       )json")
JSON_PARSER_EXPECT_FAILURE(n_array_double_extra_comma,            R"json(  ["x",,]      )json")
JSON_PARSER_EXPECT_FAILURE(n_array_extra_close,                   R"json(  ["x"]]       )json")
JSON_PARSER_EXPECT_FAILURE(n_array_extra_comma,                   R"json(  ["",]        )json")
JSON_PARSER_EXPECT_FAILURE(n_array_incomplete_invalid_value,      R"json(  [x           )json")
JSON_PARSER_EXPECT_FAILURE(n_array_incomplete,                    R"json(  ["x"         )json")
JSON_PARSER_EXPECT_FAILURE(n_array_inner_array_no_comma,          R"json(  [3[4]]       )json")
JSON_PARSER_EXPECT_FAILURE(n_array_invalid_utf8,                  R"json(  [�]          )json")
JSON_PARSER_EXPECT_FAILURE(n_array_items_separated_by_semicolon,  R"json(  [1:2]        )json")
JSON_PARSER_EXPECT_FAILURE(n_array_just_comma,                    R"json(  [,]          )json")
JSON_PARSER_EXPECT_FAILURE(n_array_just_minus,                    R"json(  [-]          )json")
JSON_PARSER_EXPECT_FAILURE(n_array_missing_value,                 R"json(  [   , ""]    )json")
JSON_PARSER_EXPECT_FAILURE(n_array_newlines_unclosed,             R"json(  ["a", 4 ,1,  )json")
JSON_PARSER_EXPECT_FAILURE(n_array_number_and_comma,              R"json(  [1,]         )json")
JSON_PARSER_EXPECT_FAILURE(n_array_number_and_several_commas,     R"json(  [1,,]        )json")
JSON_PARSER_EXPECT_FAILURE(n_array_spaces_vertical_tab_formfeed,  R"json(  ["a"\f]      )json")
JSON_PARSER_EXPECT_FAILURE(n_array_star_inside,                   R"json(  [*]          )json")
JSON_PARSER_EXPECT_FAILURE(n_array_unclosed,                      R"json(  [""          )json")
JSON_PARSER_EXPECT_FAILURE(n_array_unclosed_trailing_comma,       R"json(  [1,          )json")
JSON_PARSER_EXPECT_FAILURE(n_array_unclosed_with_new_lines,       R"json(  [1, 1 ,1     )json")
JSON_PARSER_EXPECT_FAILURE(n_array_unclosed_with_object_inside,   R"json(  [{}          )json")
