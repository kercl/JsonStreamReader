#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JSON_TEST_SUITE_NUMBER_SUCCESSES
#include <boost/test/unit_test.hpp>

#include "TestParser.h"

JSON_PARSER_EXPECT_SUCCESS(y_number_0e_plus_1,              R"json(  [0e+1]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_0e1,                    R"json(  [0e1]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_after_space,            R"json(  [ 4]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_double_close_to_zero,   R"json(  [-0.000000000000000000000000000000000000000000000000000000000000000000000000000001]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_int_with_exp,           R"json(  [20e1]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_minus_zero,             R"json(  [-0]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_negative_int,           R"json(  [-123]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_negative_one,           R"json(  [-1]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_negative_zero,          R"json(  [-0]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_real_capital_e,         R"json(  [1E22]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_real_capital_e_neg_exp, R"json(  [1E-2]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_real_capital_e_pos_exp, R"json(  [1E+2]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_real_exponent,          R"json(  [123e45]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_real_fraction_exponent, R"json(  [123.456e78]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_real_neg_exp,           R"json(  [1e-2]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_real_pos_exponent,      R"json(  [1e+2]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_simple_int,             R"json(  [123]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_number_simple_real,            R"json(  [123.456789]  )json")
