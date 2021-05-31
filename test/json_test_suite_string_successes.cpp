#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JSON_TEST_SUITE_STRING_SUCCESSES
#include <boost/test/unit_test.hpp>

#include "parser.h"

JSON_PARSER_EXPECT_SUCCESS(y_string_1_2_3_bytes_UTF_8_sequences,             R"json(  ["\u0060\u012a\u12AB"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_accepted_surrogate_pair,                 R"json(  ["\uD801\udc37"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_accepted_surrogate_pairs,                R"json(  ["\ud83d\ude39\ud83d\udc8d"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_allowed_escapes,                         R"json(  ["\"\\\/\b\f\n\r\t"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_backslash_and_u_escaped_zero,            R"json(  ["\\u0000"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_backslash_doublequotes,                  R"json(  ["\""]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_comments,                                R"json(  ["a/*b*/c/*d//e"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_double_escape_a,                         R"json(  ["\\a"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_double_escape_n,                         R"json(  ["\\n"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_escaped_control_character,               R"json(  ["\u0012"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_escaped_noncharacter,                    R"json(  ["\uFFFF"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_in_array,                                R"json(  ["asd"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_in_array_with_leading_space,             R"json(  [ "asd"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_last_surrogates_1_and_2,                 R"json(  ["\uDBFF\uDFFF"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_nbsp_uescaped,                           R"json(  ["new\u00A0line"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_nonCharacterInUTF_8_U10FFFF,             R"json(  ["􏿿"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_nonCharacterInUTF_8_UFFFF,               R"json(  ["￿"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_null_escape,                             R"json(  ["\u0000"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_one_byte_utf_8,                          R"json(  ["\u002c"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_pi,                                      R"json(  ["π"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_reservedCharacterInUTF_8_U1BFFF,         R"json(  ["𛿿"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_simple_ascii,                            R"json(  ["asd "]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_space,                                   R"json(  [" "]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_surrogates_U1D11E_MUSICAL_SYMBOL_G_CLEF, R"json(  ["\uD834\uDd1e"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_three_byte_utf_8,                        R"json(  ["\u0821"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_two_byte_utf_8,                          R"json(  ["\u0123"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_u2028_line_sep,                          R"json(  [""]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_u2029_par_sep,                           R"json(  [""]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_uescaped_newline,                        R"json(  ["new\u000Aline"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_uEscape,                                 R"json(  ["\u0061\u30af\u30EA\u30b9"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_unescaped_char_delete,                   R"json(  [""]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_unicode_2,                               R"json(  ["⍂㈴⍂"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_unicodeEscapedBackslash,                 R"json(  ["\u005C"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_unicode_escaped_double_quote,            R"json(  ["\u0022"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_unicode,                                 R"json(  ["\uA66D"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_unicode_U10FFFE_nonchar,                 R"json(  ["\uDBFF\uDFFE"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_unicode_U1FFFE_nonchar,                  R"json(  ["\uD83F\uDFFE"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_unicode_U200B_ZERO_WIDTH_SPACE,          R"json(  ["\u200B"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_unicode_U2064_invisible_plus,            R"json(  ["\u2064"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_unicode_UFDD0_nonchar,                   R"json(  ["\uFDD0"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_unicode_UFFFE_nonchar,                   R"json(  ["\uFFFE"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_utf8,                                    R"json(  ["€𝄞"]  )json")
JSON_PARSER_EXPECT_SUCCESS(y_string_with_del_character,                      "[\"a\x7fa\"]")