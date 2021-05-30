#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JSON_TEST_SUITE_STRUCTURE_FAILURES
#include <boost/test/unit_test.hpp>

#include "parser.h"

JSON_PARSER_EXPECT_FAILURE(n_structure_angle_bracket_dot,                             R"json(  <.>  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_angle_bracket_null,                            R"json(  [<null>]  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_array_trailing_garbage,                        R"json(  [1]x  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_array_with_extra_array_close,                  R"json(  [1]]  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_array_with_unclosed_string,                    R"json(  ["asd]  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_ascii_unicode_identifier,                      R"json(  aå  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_capitalized_True,                              R"json(  [True]  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_close_unopened_array,                          R"json(  1]  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_comma_instead_of_closing_brace,                R"json(  {"x": true,  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_double_array,                                  R"json(  [][]  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_end_array,                                     R"json(  ]  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_incomplete_UTF8_BOM,                           R"json(  �{}  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_lone_invalid_utf_8,                            R"json(  �  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_lone_open_bracket,                             R"json(  [  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_no_data,                                       R"json(    )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_number_with_trailing_garbage,                  R"json(  2@  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_object_followed_by_closing_object,             R"json(  {}}  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_object_unclosed_no_value,                      R"json(  {"":  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_object_with_comment,                           R"json(  {"a":/*comment*/"b"}  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_object_with_trailing_garbage,                  R"json(  {"a": true} "x"  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_open_array_apostrophe,                         R"json(  ['  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_open_array_comma,                              R"json(  [,  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_open_array_open_object,                        R"json(  [{  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_open_array_open_string,                        R"json(  ["a  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_open_array_string,                             R"json(  ["a"  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_open_object_close_array,                       R"json(  {]  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_open_object_comma,                             R"json(  {,  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_open_object,                                   R"json(  {  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_open_object_open_array,                        R"json(  {[  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_open_object_open_string,                       R"json(  {"a  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_open_object_string_with_apostrophes,           R"json(  {'a'  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_open_open,                                     R"json(  ["\{["\{["\{["\{  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_single_eacute,                                 R"json(  �  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_single_star,                                   R"json(  *  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_trailing_hash,                                 R"json(  {"a":"b"}#{}  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_Uplus2060_word_joined,                         R"json(  [⁠]  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_uescaped_LF_before_string,                     R"json(  [\u000A""]  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_unclosed_array,                                R"json(  [1  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_unclosed_array_partial_null,                   R"json(  [ false, nul  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_unclosed_array_unfinished_false,               R"json(  [ true, fals  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_unclosed_array_unfinished_true,                R"json(  [ false, tru  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_unclosed_object,                               R"json(  {"asd":"asd"  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_unicode_identifier,                            R"json(  å  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_UTF8_BOM_no_data,                              R"json(  [  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_whitespace_formfeed,                           R"json(   ]  )json")
JSON_PARSER_EXPECT_FAILURE(n_structure_whitespace_Uplus2060_word_joiner,              R"json(  [⁠]  )json")