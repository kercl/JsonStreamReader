#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JSON_TEST_SUITE_OBJECT_FAILURES
#include <boost/test/unit_test.hpp>

#include "TestParser.h"

JSON_PARSER_EXPECT_FAILURE(n_object_bad_value,                                        R"json(  ["x", truth]  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_bracket_key,                                      R"json(  {[: "x"}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_comma_instead_of_colon,                           R"json(  {"x", null}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_double_colon,                                     R"json(  {"x"::"b"}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_emoji,                                            R"json(  {🇨🇭}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_garbage_at_end,                                   R"json(  {"a":"a" 123}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_key_with_single_quotes,                           R"json(  {key: 'value'}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_lone_continuation_byte_in_key_and_trailing_comma, R"json(  {"�":"0",}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_missing_colon,                                    R"json(  {"a" b}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_missing_key,                                      R"json(  {:"b"}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_missing_semicolon,                                R"json(  {"a" "b"}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_missing_value,                                    R"json(  {"a":  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_no_colon,                                         R"json(  {"a"  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_non_string_key_but_huge_number_instead,           R"json(  {9999E9999:1}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_non_string_key,                                   R"json(  {1:1}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_repeated_null_null,                               R"json(  {null:null,null:null}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_several_trailing_commas,                          R"json(  {"id":0,,,,,}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_single_quote,                                     R"json(  {'a':0}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_trailing_comma,                                   R"json(  {"id":0,}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_trailing_comment,                                 R"json(  {"a":"b"}/**/  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_trailing_comment_open,                            R"json(  {"a":"b"}/**//  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_trailing_comment_slash_open_incomplete,           R"json(  {"a":"b"}/  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_trailing_comment_slash_open,                      R"json(  {"a":"b"}//  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_two_commas_in_a_row,                              R"json(  {"a":"b",,"c":"d"}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_unquoted_key,                                     R"json(  {a: "b"}  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_unterminated_value,                               R"json(  {"a":"a  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_with_single_string,                               R"json(  { "foo" : "bar", "a" }  )json")
JSON_PARSER_EXPECT_FAILURE(n_object_with_trailing_garbage,                            R"json(  {"a":"b"}#  )json")
