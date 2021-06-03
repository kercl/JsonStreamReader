#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JSON_TEST_SUITE_OBJECT_SUCCESSES
#include <boost/test/unit_test.hpp>

#include "TestParser.h"

JSON_PARSER_EXPECT_SUCCESS(y_object_basic,                    R"json(  {"asd":"sdf"}  )json")
JSON_PARSER_EXPECT_SUCCESS(y_object_duplicated_key_and_value, R"json(  {"a":"b","a":"b"}  )json")
JSON_PARSER_EXPECT_SUCCESS(y_object_duplicated_key,           R"json(  {"a":"b","a":"c"}  )json")
JSON_PARSER_EXPECT_SUCCESS(y_object_empty,                    R"json(  {}  )json")
JSON_PARSER_EXPECT_SUCCESS(y_object_empty_key,                R"json(  {"":0}  )json")
JSON_PARSER_EXPECT_SUCCESS(y_object_escaped_null_in_key,      R"json(  {"foo\u0000bar": 42}  )json")
JSON_PARSER_EXPECT_SUCCESS(y_object_extreme_numbers,          R"json(  { "min": -1.0e+28, "max": 1.0e+28 }  )json")
JSON_PARSER_EXPECT_SUCCESS(y_object_long_strings,             R"json(  {"x":[{"id": "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"}], "id": "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"}  )json")
JSON_PARSER_EXPECT_SUCCESS(y_object_simple,                   R"json(  {"a":[]}  )json")
JSON_PARSER_EXPECT_SUCCESS(y_object_string_unicode,           R"json(  {"title":"\u041f\u043e\u043b\u0442\u043e\u0440\u0430 \u0417\u0435\u043c\u043b\u0435\u043a\u043e\u043f\u0430" }  )json")
JSON_PARSER_EXPECT_SUCCESS(y_object_with_newlines,            "  {\n \"a\": \"b\"\n}  ")
