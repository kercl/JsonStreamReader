#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE N_INCOMPLETE
#include <boost/test/unit_test.hpp>

#include "parser.h"

TEST_CASE(n_number_plus,               R"json( [+1]    )json")
TEST_CASE(n_number_plus_Inf,           R"json( [+Inf]  )json")
TEST_CASE(n_number_minus_01,           R"json( [-01]   )json")
TEST_CASE(n_number_minus_1_dot_0_ddot, R"json( [-1.0.] )json")
TEST_CASE(n_number_minus_2_dot,        R"json( [-2.]   )json")
TEST_CASE(n_number_plus_NaN,           R"json( [-NaN]  )json")
TEST_CASE(n_number_dot_minus_1,        R"json( [.-1]   )json")
TEST_CASE(n_number_dot_2e_3,           R"json( [.2e-3] )json")
TEST_CASE(n_number_0_1_2,              R"json( [0.1.2] )json")
TEST_CASE(n_number_0_dot_3e_plus,      R"json( [0.3e+] )json")
TEST_CASE(n_number_0_e1,               R"json( [0.e1]  )json")
TEST_CASE(n_number_0_capital_E_plus,   R"json( [0E+]   )json")
TEST_CASE(n_number_0_capital_E,        R"json( [0E]    )json")
TEST_CASE(n_number_0e_plus,            R"json( [0e+]   )json")
TEST_CASE(n_number_0e,                 R"json( [0e]    )json")
