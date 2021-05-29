#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE N_ARRAY
#include <boost/test/unit_test.hpp>

#include "parser.h"

TEST_CASE(n_array_1_true_without_comma,         R"json( [1 true]  )json")
TEST_CASE(n_array_colon_instead_of_comma,       R"json( ["": 1]   )json")
TEST_CASE(n_array_comma_after_close,            R"json( [""],     )json")
TEST_CASE(n_array_comma_and_number,             R"json( [,1]      )json")
TEST_CASE(n_array_double_comma,                 R"json( [1,,2]    )json")
TEST_CASE(n_array_double_extra_comma,           R"json( ["x",,]   )json")
TEST_CASE(n_array_extra_close,                  R"json( ["x"]]    )json")
TEST_CASE(n_array_extra_comma,                  R"json( ["",]     )json")
TEST_CASE(n_array_inner_array_no_comma,         R"json( [3[4]]    )json")
TEST_CASE(n_array_items_separated_by_semicolon, R"json( [1:2]     )json")
TEST_CASE(n_array_just_comma,                   R"json( [,]       )json")
TEST_CASE(n_array_just_minus,                   R"json( [-]       )json")
TEST_CASE(n_array_missing_value,                R"json( [   , ""] )json")
TEST_CASE(n_array_number_and_comma,             R"json( [1,]      )json")
TEST_CASE(n_array_number_and_several_commas,    R"json( [1,,]     )json")
TEST_CASE(n_array_spaces_vertical_tab_formfeed, R"json( ["a"\f]   )json")
TEST_CASE(n_array_star_inside,                  R"json( [*]       )json")