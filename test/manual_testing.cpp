#include <iostream>
#include <cstring>
#include <cmath>

#include "JSONParser.h"
#include "parser.h"

#define XSTR(x) STR(x)
#define STR(x) #x

#define SUPPRESS_UNUSED(x) ((void)x)

using namespace std;

const char *test_json = R"json(
{
  "mainly": true,
  "get": "shot",
  "dry": "safety"
}
)json";

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
        // test.dump_state();
        // getchar();
    }
    test.end_of_transmission();
    test.reset();

    cout << json::Path((json::RawParser*)&test).size() << endl;
    cout << json::Path((json::RawParser*)&test).as_name(0) << endl;

    return 0;
}
