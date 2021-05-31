#pragma once

#include <string>
#include <sstream>

#include "JSONParser.h"

#ifndef BOOST_TEST_MESSAGE
#   include <iostream>
#   define BOOST_TEST_MESSAGE(msg) std::cout << msg << std::endl;
#endif

#define SUPPRESS_UNUSED(x) ((void)x)

#define JSON_PARSER_EXPECT_FAILURE(name, json_doc) \
    BOOST_AUTO_TEST_CASE(name) { \
        std::string test_case(json_doc); \
        BOOST_TEST(TestParser::parse_string(test_case) != json::NoError, \
                   "Parsing of the document " << json_doc << " is expected to fail.");}

#define JSON_PARSER_EXPECT_SUCCESS(name, json_doc) \
    BOOST_AUTO_TEST_CASE(name) { \
        std::string test_case(json_doc); \
        BOOST_TEST(TestParser::parse_string(test_case) == json::NoError, \
                   "Parsing of the document " << json_doc << " is expected to succeed.");}

class TestParser : public json::RawParser {
public:
    json::ErrorCode m_last_error;
    bool m_print_errors, m_generate_parser_data;
    std::stringstream output_data;

    TestParser(bool print_errors, bool generate_parser_data)
        : m_last_error(json::NoError)
        , m_print_errors(print_errors)
        , m_generate_parser_data(generate_parser_data)
    {}

#ifdef JSONSTREAM_ACT_ON_DOCUMENT_BEGIN
    void on_document_begin(json::Type type) override {
		if(m_generate_parser_data) {
            output_data << "DOCUMENT BEGIN (";
            switch(type) {
            case json::Array:
                output_data << "Array";
                break;
            case json::Object:
                output_data << "Object";
                break;
            case json::Value:
                output_data << "Value";
                break;
            default:
                break;
            }
            output_data << ")\n";
        }
    }
#endif

#ifdef JSONSTREAM_ACT_ON_DOCUMENT_END
    void on_document_end(json::Type type) override {
		if(m_generate_parser_data) {
            output_data << "DOCUMENT END   (";
            switch(type) {
            case json::Array:
                output_data << "Array";
                break;
            case json::Object:
                output_data << "Object";
                break;
            case json::Value:
                output_data << "Value";
                break;
            default:
                break;
            }
            output_data << ")\n";
        }
    }
#endif

#ifdef JSONSTREAM_ACT_ON_ARRAY_BEGIN
    void on_array_begin(const json::Path& path) override {
		if(m_generate_parser_data) {
            output_data << "ARRAY  BEGIN:  ";
            print_value(path, nullptr);
            output_data << "\n";
        }
    }
#endif

#ifdef JSONSTREAM_ACT_ON_ARRAY_EMPTY
    void on_array_empty(const json::Path& path) override {
		if(m_generate_parser_data) {
            output_data << "ARRAY  EMPTY:  ";
            print_value(path, nullptr);
            output_data << "\n";
        }
    }
#endif

#ifdef JSONSTREAM_ACT_ON_ARRAY_END
    void on_array_end(const json::Path& path) override {
		if(m_generate_parser_data) {
            output_data << "ARRAY  END:    ";
            print_value(path, nullptr);
            output_data << "\n";
        }
    }
#endif

#ifdef JSONSTREAM_ACT_ON_OBJECT_BEGIN
    void on_object_begin(const json::Path& path) override {
		if(m_generate_parser_data) {
            output_data << "OBJECT BEGIN:  ";
            print_value(path, nullptr);
            output_data << "\n";
        }
    }
#endif

#ifdef JSONSTREAM_ACT_ON_OBJECT_EMPTY
    void on_object_empty(const json::Path& path) override {
		if(m_generate_parser_data) {
            output_data << "OBJECT EMPTY:  ";
            print_value(path, nullptr);
            output_data << "\n";
        }
    }
#endif

#ifdef JSONSTREAM_ACT_ON_OBJECT_END
    void on_object_end(const json::Path& path) override {
		if(m_generate_parser_data) {
            output_data << "OBJECT END:    ";
            print_value(path, nullptr);
            output_data << "\n";
        }
    }
#endif

    void on_string(const json::Path& path, const char *value) override {
		if(m_generate_parser_data) {
            output_data << "VALUE  STRING: ";
            print_value(path, value);
            output_data << "\n";
        }
    }

    void on_number(const json::Path& path, const char *value) override {
		if(m_generate_parser_data) {
            output_data << "VALUE  NUMBER: ";
            print_value(path, value);
            output_data << "\n";
        }
    }

    void on_true(const json::Path& path) override {
		if(m_generate_parser_data) {
            output_data << "VALUE  TRUE:   ";
            print_value(path, "true");
            output_data << "\n";
        }
    }

    void on_false(const json::Path& path) override {
		if(m_generate_parser_data) {
            output_data << "VALUE  FALSE:  ";
            print_value(path, "false");
            output_data << "\n";
        }
    }

    void on_null(const json::Path& path) override {
		if(m_generate_parser_data) {
            output_data << "VALUE  NULL:   ";
            print_value(path, "null");
            output_data << "\n";
        }
    }

    void on_error(const json::ErrorCode error_code) override {
        m_last_error = error_code;

        if(!m_print_errors)
            return;

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
        TestParser parser(false, false);
        for(char &c: doc) {
            if(parser.m_last_error != json::NoError)
                break;
            parser.parse(c);
        }
        parser.end_of_transmission();

        return parser.m_last_error;
    }

    void print_value(const json::Path& path, const char *value) {
        for(unsigned int i = 0; i < path.size(); i++) {
            if(path.is_name(i)) {
                output_data << "[" << path.as_name(i) << "]";
            } else if(path.is_index(i)) {
                output_data << "<" << path.as_index(i) << ">";
            }
        }

        if(value)
            output_data << " --> " << value;
    }

    std::string to_string() {
        return output_data.str();
    }

#ifdef JSONSTREAM_DEVELOPMENT
	void dump_state() {
		static const char _dbg_struct_names[json::Undefined + 1][40] = {
			"DocumentObject",
			"DocumentArray",
			"Object",
			"ObjectBegin",
			"ObjectKey",
			"ObjectColon",
			"Value",
			"ArrayBegin",
			"Array",
			"Number",
			"StringBegin",
			"String",
			"ValueTrue",
			"ValueFalse",
			"ValueNull",
			"Undefined"
		};

		unsigned int struct_name_max_len = 0;
		for(unsigned int i = 0; i < json::Undefined; i++) {
			if(strlen(_dbg_struct_names[i]) > struct_name_max_len) {
				struct_name_max_len = strlen(_dbg_struct_names[i]);
			}
		}

		printf("Stack: %4d", m_stack_top);
		for(unsigned int i = 0; i < struct_name_max_len - 11; i++)
			printf(" ");
		printf(" | Data: %3d\n", m_buffer_level);
		printf("================================\n");

		for(unsigned int i = 0; i < std::max(m_buffer_level, m_stack_top); ++i) {
			if(i < m_stack_top) {
				printf("%*s",
					struct_name_max_len,
					_dbg_struct_names[m_stack[i]]);
			} else {
				for(unsigned int j = 0; j < struct_name_max_len; j++)
					printf(" ");
			}

			if(i <= m_buffer_level) {
				if(m_buffer_sizes[i] == sizeof(int) &&
				m_buffers[i][sizeof(int) - 1] == '\0')
				{
					int *data = (int*)&m_buffers[i][0];
					printf(" | (int): %d\n",
						*data);
				} else {
					printf(" | %4d: %.*s\n", 
						m_buffer_sizes[i],
						m_buffer_sizes[i],
						m_buffers[i]);
				}
			} else {
				printf("\n");
			}
		}
	}
#endif
};
