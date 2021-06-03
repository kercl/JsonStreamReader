#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JSON_TEST_SUITE_ARRAY_FAILURES
#include <boost/test/unit_test.hpp>
#include <sstream>

#include "TestParser.h"

#define JSON_OUTPUT_TEST(in, out, err_code) \
	parser.write(in); \
	parser.end_of_transmission(); \
	BOOST_TEST( \
		parser.output_data.str().compare(out) == 0); \
	BOOST_TEST(parser.m_last_error err_code); \
	BOOST_TEST_MESSAGE(parser.output_data.str()); \
	parser.reset(); \
	parser.output_data.str(std::string());

#define JSON_OUTPUT_TEST_SUCCESS(in, out) \
	JSON_OUTPUT_TEST(in, out, ==json::NoError)

#define JSON_OUTPUT_TEST_FAILURE(in, out) \
	JSON_OUTPUT_TEST(in, out, !=json::NoError)

BOOST_AUTO_TEST_CASE(valid_sample_documents) {
	TestParser parser(true, true);
	
	JSON_OUTPUT_TEST_SUCCESS(
		R"json({
			"mainly": true,
			"get": "shot",
			"dry": "safety"
		})json",
		"DOCUMENT BEGIN (Object)\n"
		"OBJECT BEGIN:  \n"
		"VALUE  TRUE:   [mainly] --> true\n"
		"VALUE  STRING: [get] --> shot\n"
		"VALUE  STRING: [dry] --> safety\n"
		"OBJECT END:    \n"
		"DOCUMENT END   (Object)\n")


	JSON_OUTPUT_TEST_SUCCESS(
		R"json({
			"amount": true,
			"burst": {
				"arrive": false,
				"thin": 971563999,
				"mistake": -559644214
			},
			"bowl": [
				"train",
				"dug",
				426277961.9230802
			]
		})json",
		"DOCUMENT BEGIN (Object)\n"
		"OBJECT BEGIN:  \n"
		"VALUE  TRUE:   [amount] --> true\n"
		"OBJECT BEGIN:  [burst]\n"
		"VALUE  FALSE:  [burst][arrive] --> false\n"
		"VALUE  NUMBER: [burst][thin] --> 971563999\n"
		"VALUE  NUMBER: [burst][mistake] --> -559644214\n"
		"OBJECT END:    [burst]\n"
		"ARRAY  BEGIN:  [bowl]\n"
		"VALUE  STRING: [bowl]<0> --> train\n"
		"VALUE  STRING: [bowl]<1> --> dug\n"
		"VALUE  NUMBER: [bowl]<2> --> 426277961.9230802\n"
		"ARRAY  END:    [bowl]\n"
		"OBJECT END:    \n"
		"DOCUMENT END   (Object)\n")

	JSON_OUTPUT_TEST_SUCCESS(
		R"json([
			1588810816.083109,
			771192619.6796126,
			-1385927455
		])json",
		"DOCUMENT BEGIN (Array)\n"
		"ARRAY  BEGIN:  \n"
		"VALUE  NUMBER: <0> --> 1588810816.083109\n"
		"VALUE  NUMBER: <1> --> 771192619.6796126\n"
		"VALUE  NUMBER: <2> --> -1385927455\n"
		"ARRAY  END:    \n"
		"DOCUMENT END   (Array)\n")

	JSON_OUTPUT_TEST_SUCCESS(
		R"json({
			"wave": [
				"signal",
				2085066803.059366,
				-1972210410.9708362
			],
			"goes": 198620571.2804947,
			"raw": {
				"enjoy": "current",
				"throughout": "fastened",
				"setting": false
			}
		})json",
		"DOCUMENT BEGIN (Object)\n"
		"OBJECT BEGIN:  \n"
		"ARRAY  BEGIN:  [wave]\n"
		"VALUE  STRING: [wave]<0> --> signal\n"
		"VALUE  NUMBER: [wave]<1> --> 2085066803.059366\n"
		"VALUE  NUMBER: [wave]<2> --> -1972210410.9708362\n"
		"ARRAY  END:    [wave]\n"
		"VALUE  NUMBER: [goes] --> 198620571.2804947\n"
		"OBJECT BEGIN:  [raw]\n"
		"VALUE  STRING: [raw][enjoy] --> current\n"
		"VALUE  STRING: [raw][throughout] --> fastened\n"
		"VALUE  FALSE:  [raw][setting] --> false\n"
		"OBJECT END:    [raw]\n"
		"OBJECT END:    \n"
		"DOCUMENT END   (Object)\n");

	JSON_OUTPUT_TEST_SUCCESS(
		R"json([
			-977464858,
			[
					[
							1358523526.2398686,
							-2021345409,
							-729284112,
							"mass",
							"practice",
							false,
							"farm",
							"strange",
							524537514.72458434,
							"call",
							false,
							true
					],
					-937591917,
					"tip",
					"driven",
					true,
					false,
					"bank",
					1287244522,
					true,
					-2016720706.89435,
					"acres",
					"high"
			],
			1480916783,
			"fallen",
			false,
			true,
			1594231624.750697,
			false,
			795986448,
			"mind",
			"fallen",
			false
		])json",
		"DOCUMENT BEGIN (Array)\n"
		"ARRAY  BEGIN:  \n"
		"VALUE  NUMBER: <0> --> -977464858\n"
		"ARRAY  BEGIN:  <1>\n"
		"ARRAY  BEGIN:  <1><0>\n"
		"VALUE  NUMBER: <1><0><0> --> 1358523526.2398686\n"
		"VALUE  NUMBER: <1><0><1> --> -2021345409\n"
		"VALUE  NUMBER: <1><0><2> --> -729284112\n"
		"VALUE  STRING: <1><0><3> --> mass\n"
		"VALUE  STRING: <1><0><4> --> practice\n"
		"VALUE  FALSE:  <1><0><5> --> false\n"
		"VALUE  STRING: <1><0><6> --> farm\n"
		"VALUE  STRING: <1><0><7> --> strange\n"
		"VALUE  NUMBER: <1><0><8> --> 524537514.72458434\n"
		"VALUE  STRING: <1><0><9> --> call\n"
		"VALUE  FALSE:  <1><0><10> --> false\n"
		"VALUE  TRUE:   <1><0><11> --> true\n"
		"ARRAY  END:    <1><0>\n"
		"VALUE  NUMBER: <1><1> --> -937591917\n"
		"VALUE  STRING: <1><2> --> tip\n"
		"VALUE  STRING: <1><3> --> driven\n"
		"VALUE  TRUE:   <1><4> --> true\n"
		"VALUE  FALSE:  <1><5> --> false\n"
		"VALUE  STRING: <1><6> --> bank\n"
		"VALUE  NUMBER: <1><7> --> 1287244522\n"
		"VALUE  TRUE:   <1><8> --> true\n"
		"VALUE  NUMBER: <1><9> --> -2016720706.89435\n"
		"VALUE  STRING: <1><10> --> acres\n"
		"VALUE  STRING: <1><11> --> high\n"
		"ARRAY  END:    <1>\n"
		"VALUE  NUMBER: <2> --> 1480916783\n"
		"VALUE  STRING: <3> --> fallen\n"
		"VALUE  FALSE:  <4> --> false\n"
		"VALUE  TRUE:   <5> --> true\n"
		"VALUE  NUMBER: <6> --> 1594231624.750697\n"
		"VALUE  FALSE:  <7> --> false\n"
		"VALUE  NUMBER: <8> --> 795986448\n"
		"VALUE  STRING: <9> --> mind\n"
		"VALUE  STRING: <10> --> fallen\n"
		"VALUE  FALSE:  <11> --> false\n"
		"ARRAY  END:    \n"
		"DOCUMENT END   (Array)\n")

	JSON_OUTPUT_TEST_SUCCESS(
		R"json([
			-2084882453.82427,
			true,
			{
					"walk": false,
					"his": 1251649440,
					"sunlight": true
			}
		])json",
		"DOCUMENT BEGIN (Array)\n"
		"ARRAY  BEGIN:  \n"
		"VALUE  NUMBER: <0> --> -2084882453.82427\n"
		"VALUE  TRUE:   <1> --> true\n"
		"OBJECT BEGIN:  <2>\n"
		"VALUE  FALSE:  <2>[walk] --> false\n"
		"VALUE  NUMBER: <2>[his] --> 1251649440\n"
		"VALUE  TRUE:   <2>[sunlight] --> true\n"
		"OBJECT END:    <2>\n"
		"ARRAY  END:    \n"
		"DOCUMENT END   (Array)\n")

	JSON_OUTPUT_TEST_SUCCESS(
		R"json({
			"edge": "few",
			"enough": {
					"crack": "spell",
					"hidden": true,
					"planet": false
			},
			"child": "court"
		})json",
		"DOCUMENT BEGIN (Object)\n"
		"OBJECT BEGIN:  \n"
		"VALUE  STRING: [edge] --> few\n"
		"OBJECT BEGIN:  [enough]\n"
		"VALUE  STRING: [enough][crack] --> spell\n"
		"VALUE  TRUE:   [enough][hidden] --> true\n"
		"VALUE  FALSE:  [enough][planet] --> false\n"
		"OBJECT END:    [enough]\n"
		"VALUE  STRING: [child] --> court\n"
		"OBJECT END:    \n"
		"DOCUMENT END   (Object)\n")

	JSON_OUTPUT_TEST_SUCCESS(
		R"json({
			"court": [
					758636209,
					-1164263558,
					"police"
			],
			"small": -1236798678.849781,
			"introduced": "occasionally"
		})json",
		"DOCUMENT BEGIN (Object)\n"
		"OBJECT BEGIN:  \n"
		"ARRAY  BEGIN:  [court]\n"
		"VALUE  NUMBER: [court]<0> --> 758636209\n"
		"VALUE  NUMBER: [court]<1> --> -1164263558\n"
		"VALUE  STRING: [court]<2> --> police\n"
		"ARRAY  END:    [court]\n"
		"VALUE  NUMBER: [small] --> -1236798678.849781\n"
		"VALUE  STRING: [introduced] --> occasionally\n"
		"OBJECT END:    \n"
		"DOCUMENT END   (Object)\n")

	JSON_OUTPUT_TEST_SUCCESS(
		R"json([
			"built",
			[
					1623169628.7966807,
					-901690021,
					false
			],
			"noun"
		])json",
		"DOCUMENT BEGIN (Array)\n"
		"ARRAY  BEGIN:  \n"
		"VALUE  STRING: <0> --> built\n"
		"ARRAY  BEGIN:  <1>\n"
		"VALUE  NUMBER: <1><0> --> 1623169628.7966807\n"
		"VALUE  NUMBER: <1><1> --> -901690021\n"
		"VALUE  FALSE:  <1><2> --> false\n"
		"ARRAY  END:    <1>\n"
		"VALUE  STRING: <2> --> noun\n"
		"ARRAY  END:    \n"
		"DOCUMENT END   (Array)\n")
}


BOOST_AUTO_TEST_CASE(invalid_sample_documents) {
	TestParser parser(true, true);
	
	JSON_OUTPUT_TEST_FAILURE(
		R"json({
			"done": {
					"having": true,
					"command": false
					"younger": -909883216
			},
			"shut": true,
			"till": true
		})json",
		"DOCUMENT BEGIN (Object)\n"
		"OBJECT BEGIN:  \n"
		"OBJECT BEGIN:  [done]\n"
		"VALUE  TRUE:   [done][having] --> true\n"
		"VALUE  FALSE:  [done][command] --> false\n"
		"ERROR: Unexpected character.\n")

	JSON_OUTPUT_TEST_FAILURE(
		R"json({
			"done": {
					"having": true,
					"command": false,
					"younger": -909883216
			},
			"shut": true,
			"till": true
		}m)json",
		"DOCUMENT BEGIN (Object)\n"
		"OBJECT BEGIN:  \n"
		"OBJECT BEGIN:  [done]\n"
		"VALUE  TRUE:   [done][having] --> true\n"
		"VALUE  FALSE:  [done][command] --> false\n"
		"VALUE  NUMBER: [done][younger] --> -909883216\n"
		"OBJECT END:    [done]\n"
		"VALUE  TRUE:   [shut] --> true\n"
		"VALUE  TRUE:   [till] --> true\n"
		"OBJECT END:    \n"
		"DOCUMENT END   (Object)\n"
		"ERROR: Unexpected character.\n")

	JSON_OUTPUT_TEST_FAILURE(
		R"json({{
			"done": {
					"having": true,
					"command": false,
					"younger": -909883216
			},
			"shut": true,
			"till": true
		})json",
		"DOCUMENT BEGIN (Object)\n"
		"OBJECT BEGIN:  \n"
		"ERROR: Unexpected character.\n")

	JSON_OUTPUT_TEST_FAILURE(
		R"json({])json",
		"DOCUMENT BEGIN (Object)\n"
		"OBJECT BEGIN:  \n"
		"ERROR: Unexpected character.\n")
}