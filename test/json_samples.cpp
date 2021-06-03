#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE JSON_TEST_SUITE_ARRAY_FAILURES
#include <boost/test/unit_test.hpp>
#include <sstream>

#include "parser.h"

BOOST_AUTO_TEST_CASE(valid_sample_documents) {
	TestParser parser(true, true);
	
	parser.write(R"json(
	{
		"mainly": true,
		"get": "shot",
		"dry": "safety"
	}
	)json");
	parser.end_of_transmission();

	BOOST_TEST(
		parser.output_data.str().compare(
			"DOCUMENT BEGIN (Object)\n"
			"OBJECT BEGIN:  \n"
			"VALUE  TRUE:   [mainly] --> true\n"
			"VALUE  STRING: [get] --> shot\n"
			"VALUE  STRING: [dry] --> safety\n"
			"OBJECT END:    \n"
			"DOCUMENT END   (Object)\n"
		) == 0);
	BOOST_TEST(parser.m_last_error == json::NoError);
	BOOST_TEST_MESSAGE(parser.output_data.str());
	parser.reset();
	parser.output_data.str(std::string());


	parser.write(R"json(
	{
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
	}
	)json");
	parser.end_of_transmission();

	BOOST_TEST(
		parser.output_data.str().compare(
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
			"DOCUMENT END   (Object)\n"
		) == 0);
	BOOST_TEST(parser.m_last_error == json::NoError);
	BOOST_TEST_MESSAGE(parser.output_data.str());
	parser.reset();
	parser.output_data.str(std::string());


	parser.write(R"json(
	[
		1588810816.083109,
		771192619.6796126,
		-1385927455
	]
	)json");
	parser.end_of_transmission();

	BOOST_TEST(
		parser.output_data.str().compare(
			"DOCUMENT BEGIN (Array)\n"
			"ARRAY  BEGIN:  \n"
			"VALUE  NUMBER: <0> --> 1588810816.083109\n"
			"VALUE  NUMBER: <1> --> 771192619.6796126\n"
			"VALUE  NUMBER: <2> --> -1385927455\n"
			"ARRAY  END:    \n"
			"DOCUMENT END   (Array)\n"
		) == 0);
	BOOST_TEST(parser.m_last_error == json::NoError);
	BOOST_TEST_MESSAGE(parser.output_data.str());
	parser.reset();
	parser.output_data.str(std::string());


	parser.write(R"json(
	{
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
	}
	)json");
	parser.end_of_transmission();

	BOOST_TEST(
		parser.output_data.str().compare(
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
			"DOCUMENT END   (Object)\n"
		) == 0);

	BOOST_TEST(parser.m_last_error == json::NoError);
	BOOST_TEST_MESSAGE("==============\n");
	BOOST_TEST_MESSAGE(parser.output_data.str());
	BOOST_TEST_MESSAGE("==============\n");
	BOOST_TEST_MESSAGE(
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
			"DOCUMENT END   (Object)\n"
	);
	parser.reset();
	parser.output_data.str(std::string());
}
