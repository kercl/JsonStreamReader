#include <iostream>
#include <cstring>
#include <cmath>

#include "JSONParser.h"
#include "TestParser.h"

#define XSTR(x) STR(x)
#define STR(x) #x

#define SUPPRESS_UNUSED(x) ((void)x)

using namespace std;

const char *test_json = R"json({])json";

int main(int argc, char **argv) {
    SUPPRESS_UNUSED(argc);
    SUPPRESS_UNUSED(argv);

    TestParser test(true, true);

    cout << "Test case document:\n" << test_json << "\nOutput:\n";
    for(unsigned int i = 0; i < strlen(test_json); i++) {
        // cout << "_______________________________" << endl;
        // switch(test_json[i]){
        // case '\n': cout << ("--- << \\n >> ---") << endl; break;
        // case '\r': cout << ("--- << \\r >> ---") << endl; break;
        // default: cout << "--- << " << test_json[i] << " >> ---" << endl; 
        // }
        test.write(test_json[i]);

        // for(int k = 0; k <= test.m_buffer_level + 1; k++)
        //     cout << (int)test.m_buffer_segments[k] << " | ";
        // cout << endl;
        // cout << "current str len: " << 
        //     test.m_buffer_segments[test.m_buffer_level + 1] - test.m_buffer_segments[test.m_buffer_level];
        // cout << endl;
        // for(int i = 0, k = 0; i < JSONSTREAM_BUFFER_LIMIT + 1; i++) {
        //     cout << i << "[" << test.m_buffer[i] << "]";
        //     if(i == test.m_buffer_segments[k] && k <= test.m_buffer_level + 1) {
        //         cout << "* <- " << (int)test.m_buffer_segments[k];
        //         k++;
        //     }
        //     cout << endl;
        // }
        // // test.dump_state();
        // getchar();
    }
    test.end_of_transmission();

    //cout << test.output_data.str() << endl;

	cout << "JSON_OUTPUT_TEST(\n    R\"json(" << test_json << ")json\",";
    for (std::string line; std::getline(test.output_data, line); )
    {
        cout << "\n    \"" << line << "\\n\"";
    }
    cout << ")\n";

    cout << sizeof(json::RawParser) << endl;


    test.reset();

    return 0;
}
