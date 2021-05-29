#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE N_INCOMPLETE
#include <boost/test/unit_test.hpp>

#include "parser.h"

TEST_CASE(n_incomplete_false,  R"json( [fals] )json")
TEST_CASE(n_incomplete_null,   R"json( [nul]  )json")
TEST_CASE(n_incomplete_true,   R"json( [tru]  )json")