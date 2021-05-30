#pragma once

#include <string>
#include "JSONParser.h"

#define SUPPRESS_UNUSED(x) ((void)x)

#define JSON_PARSER_EXPECT_FAILURE(name, json_doc) \
    BOOST_AUTO_TEST_CASE(name) { \
        std::string test_case(json_doc); \
        BOOST_TEST(TestParser::parse_string(test_case) != json::NoError, \
                   "Parsing of the document " << json_doc << " is expected fail.");}

#define JSON_PARSER_EXPECT_SUCCESS(name, json_doc) \
    BOOST_AUTO_TEST_CASE(name) { \
        std::string test_case(json_doc); \
        BOOST_TEST(TestParser::parse_string(test_case) == json::NoError, \
                   "Parsing of the document " << json_doc << " is expected succeed.");}

class TestParser : public json::RawParser {
    json::ErrorCode m_last_error;
public:
    TestParser() : m_last_error(json::NoError) {}

#ifdef JSONSTREAM_ACT_ON_DOCUMENT_BEGIN
    void on_document_begin(json::Type type) override {
        SUPPRESS_UNUSED(type);
    }
#endif

#ifdef JSONSTREAM_ACT_ON_DOCUMENT_END
    void on_document_end(json::Type type) override {
        SUPPRESS_UNUSED(type);
    }
#endif

#ifdef JSONSTREAM_ACT_ON_ARRAY_BEGIN
    void on_array_begin(const json::Path& path) override {
        SUPPRESS_UNUSED(path);
    }
#endif

#ifdef JSONSTREAM_ACT_ON_ARRAY_EMPTY
    void on_array_empty(const json::Path& path) override {
        SUPPRESS_UNUSED(path);
    }
#endif

#ifdef JSONSTREAM_ACT_ON_ARRAY_END
    void on_array_end(const json::Path& path) override {
        SUPPRESS_UNUSED(path);
    }
#endif

#ifdef JSONSTREAM_ACT_ON_OBJECT_BEGIN
    void on_object_begin(const json::Path& path) override {
        SUPPRESS_UNUSED(path);
    }
#endif

#ifdef JSONSTREAM_ACT_ON_OBJECT_EMPTY
    void on_object_empty(const json::Path& path) override {
        SUPPRESS_UNUSED(path);
    }
#endif

#ifdef JSONSTREAM_ACT_ON_OBJECT_END
    void on_object_end(const json::Path& path) override {
        SUPPRESS_UNUSED(path);
    }
#endif

    void on_string(const json::Path& path, const char *value) override {
        SUPPRESS_UNUSED(path);
        SUPPRESS_UNUSED(value);
    }

    void on_number(const json::Path& path, const char *str) override {
        SUPPRESS_UNUSED(path);
        SUPPRESS_UNUSED(str);
    }

    void on_true(const json::Path& path) override {
        SUPPRESS_UNUSED(path);
    }

    void on_false(const json::Path& path) override {
        SUPPRESS_UNUSED(path);
    }

    void on_null(const json::Path& path) override {
        SUPPRESS_UNUSED(path);
    }

    void on_error(const json::ErrorCode error_code) override {
        m_last_error = error_code;

        switch(error_code) {
        case json::ErrorOutOfMemory:
            BOOST_TEST_MESSAGE("- Error: Out of memory.");
            break;
        case json::ErrorUnexpectedCharacter:
            BOOST_TEST_MESSAGE(" - Error: Unexpected character.");
            break;
        case json::ErrorStackEmpty:
            BOOST_TEST_MESSAGE(" - Error: Stack is empty.");
            break;
        case json::ErrorBufferEmpty:
            BOOST_TEST_MESSAGE(" - Error: Buffer is empty.");
            break;
        case json::ErrorNotImplemented:
            BOOST_TEST_MESSAGE(" - Error: Not implemented.");
            break;
        case json::ErrorUnknownEscapeCharacter:
            BOOST_TEST_MESSAGE(" - Error: Unknown escape character.");
            break;
        case json::ErrorDocumentNotClosed:
            BOOST_TEST_MESSAGE(" - Error: Document not closed.");
            break;
        default:
            break;
        }
    }

    static json::ErrorCode parse_string(std::string doc) {
        BOOST_TEST_MESSAGE("Test case: " << doc << ":");
        TestParser parser;
        for(char &c: doc) {
            if(parser.m_last_error != json::NoError)
                break;
            parser.parse(c);
        }
        parser.end_of_transmission();

        return parser.m_last_error;
    }
};
