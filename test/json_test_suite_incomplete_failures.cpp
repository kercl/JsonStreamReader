#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JSON_TEST_SUITE_INCOMPLETE_FAILURES
#include <boost/test/unit_test.hpp>

#include "TestParser.h"

JSON_PARSER_EXPECT_FAILURE(n_incomplete_false, R"json(  [fals]  )json")
JSON_PARSER_EXPECT_FAILURE(n_incomplete_null,  R"json(  [nul]  )json")
JSON_PARSER_EXPECT_FAILURE(n_incomplete_true,  R"json(  [tru]  )json")