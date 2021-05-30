#include <iostream>
#include <cstring>
#include <cstdio>
#include "JSONParser.h"

#if defined __has_cpp_attribute
    #if __has_cpp_attribute(fallthrough)
        #define SUPPRESS_FALLTHROUGH_WARNING [[fallthrough]];
    #else
        #define SUPPRESS_FALLTHROUGH_WARNING
    #endif
#else
    #define SUPPRESS_FALLTHROUGH_WARNING
#endif

#define CHAR_ESCAPED     (1 << 0)
#define CHAR_UNICODE     (1 << 1)
#define NUM_HAS_COMMA    (1 << 2)
#define NUM_IS_EXP       (1 << 3)
#define DOCUMENT_INVALID (1 << 4)
#define DOCUMENT_ENDED   (1 << 5)

#define FLAG_SET(flags) (m_parse_flags |= flags)
#define FLAG_CLEAR(flags) (m_parse_flags &= ~(flags))
#define FLAG_IS_SET(flags) (m_parse_flags & (flags))
#define FLAG_IS_CLEARED(flags) (!FLAG_IS_SET(flags))

#define IS_DIGIT(c) (c >= '0' && c <= '9')
#define IS_HEX(c) (IS_DIGIT(c) \
                    || (c >= 'a' && c <= 'f') \
                    || (c >= 'A' && c <= 'F'))

#define SKIP_WHITESPACES(c) \
    if(is_whitespace(c)) { \
        return;            \
    }                      \


#ifdef JSONSTREAM_CERR_ERROR_MSG
#   define ERROR2(err_code, ret) \
        FLAG_SET(DOCUMENT_INVALID); \
        std::cerr << __FILE__ << ":" << __LINE__ << ": error: " << err_code << std::endl; \
        on_error(ErrorUnexpectedCharacter); \
        return ret;
#else
#   define ERROR2(err_code, ret) \
        FLAG_SET(DOCUMENT_INVALID); \
        on_error(err_code); \
        return ret;
#endif

#define ERROR(err_code) ERROR2(err_code,)

#define ASSERT(cond, err_code)             \
    if(!(cond)) {                          \
        ERROR(err_code)      \
    }

#define EXPECT(cond) ASSERT(cond, ErrorUnexpectedCharacter)

#define EXPECT_WHITESPACE(c) \
    if(!is_whitespace(c)) { \
        ERROR(ErrorUnexpectedCharacter) \
    }

json::RawParser::RawParser()
    : m_buffer_level(0)
    , m_stack_top(0)
    , m_parse_flags(0)
{
    m_buffer_sizes[0] = 0;
}

void json::RawParser::reset() {
    m_buffer_level = 0;
    m_stack_top = 0;
    m_parse_flags = 0;
    m_buffer_sizes[0] = 0;
}

json::RawParser::~RawParser() {}

bool json::RawParser::is_whitespace(char c) {
    switch(c) {
    case '\t':
    case '\r':
    case '\n':
    case ' ':
        return true;
    default:
        return false;
    }
}

inline void json::RawParser::push(Type structure) {
    if(m_stack_top == JSONSTREAM_STACK_LIMIT - 1) {
        ERROR(ErrorOutOfMemory)
    }

    m_stack[m_stack_top++] = structure;
}

inline json::Type json::RawParser::pop() {
    if(m_stack_top == 0) {
        ERROR2(ErrorStackEmpty, Undefined);
    }

    return m_stack[--m_stack_top];
}

inline json::Type json::RawParser::peek() {
    if(m_stack_top == 0) {
        ERROR2(ErrorStackEmpty, Undefined);
    }

    return m_stack[m_stack_top - 1];
}

inline void json::RawParser::buffer_append_char(char c) {
    if(m_buffer_level == JSONSTREAM_MAX_DEPTH || 
       m_buffer_sizes[m_buffer_level] == JSONSTREAM_BUFFER_LIMIT)
    {
        ERROR(ErrorOutOfMemory);
    }

    m_buffers[m_buffer_level][m_buffer_sizes[m_buffer_level]] = c;
    m_buffer_sizes[m_buffer_level]++;
}

inline void json::RawParser::buffer_assign_int(int i) {
    int *data = (int*)&m_buffers[m_buffer_level][0];
    *data = i;
    m_buffer_sizes[m_buffer_level] = sizeof(int);
}

inline int json::RawParser::buffer_as_int() {
    int *data = (int*)&m_buffers[m_buffer_level][0];
    return *data;
}

inline void json::RawParser::buffer_increment_int() {
    int *data = (int*)&m_buffers[m_buffer_level][0];
    *data = *data + 1;
    m_buffer_sizes[m_buffer_level] = sizeof(int);
}

inline char json::RawParser::buffer_last() {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        return '\0';
    }
    return m_buffers[m_buffer_level][m_buffer_sizes[m_buffer_level] - 1];
}

inline char json::RawParser::buffer_first() {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        return '\0';
    }
    return m_buffers[m_buffer_level][0];
}

inline void json::RawParser::descend() {
    if(m_buffer_level == JSONSTREAM_MAX_DEPTH - 1) {
        ERROR(ErrorOutOfMemory);
    }

    m_buffer_sizes[++m_buffer_level] = 0;
}

inline void json::RawParser::ascend() {
    if(m_buffer_level == 0) {
        ERROR(ErrorBufferEmpty);
    }

    m_buffer_sizes[--m_buffer_level] = 0;
}

inline void json::RawParser::clear_data() {
    m_buffer_sizes[m_buffer_level] = 0;
}


void json::RawParser::parse_object(char c) {
    SKIP_WHITESPACES(c)

    switch(pop()) {
    case ObjectBegin:
        if(c == '}') {
            if(m_stack_top == 0) {
                FLAG_SET(DOCUMENT_ENDED);
                ascend();
            }
            else {
                ascend();
            }
            TRIGGER_OBJECT_EMPTY(Path(this));
            TRIGGER_OBJECT_END(Path(this));
            return;
        }
        SUPPRESS_FALLTHROUGH_WARNING
    case ObjectKey:
        EXPECT(c == '\"');
        push(ObjectColon);
        push(StringBegin);
        parse_string(c);
        return;
    case ObjectColon:
        EXPECT(c == ':');
        push(Object);
        push(Value);
        descend();
        return;
    case Object:
        if(c == '}') {
            if(m_stack_top == 0) {
                FLAG_SET(DOCUMENT_ENDED);
                TRIGGER_OBJECT_END(Path(this));
            } else {
                ascend();
                TRIGGER_OBJECT_END(Path(this));
            }
            return;
        }
        EXPECT(c == ',');
        push(ObjectKey);
        return;
    default:
        ERROR(ErrorNotImplemented);
    }
}

void json::RawParser::parse_array(char c) {
    SKIP_WHITESPACES(c)

    if(peek() == ArrayBegin) {
        pop();
        if(c == ']') {
            TRIGGER_ARRAY_EMPTY(Path(this));
            TRIGGER_ARRAY_END(Path(this));

            if(m_stack_top == 0) {
                FLAG_SET(DOCUMENT_ENDED);
            } else {
                ascend();
            }
            return;
        }
        EXPECT(c != ',');
        push(Array);
        push(Value);
        buffer_assign_int(0);
        descend();
        parse_value(c);
        return;
    }

    if(c == ']') {
        pop();
        if(m_stack_top == 0) {
            FLAG_SET(DOCUMENT_ENDED);
            TRIGGER_ARRAY_END(Path(this));
            TRIGGER_DOCUMENT_END(Array);
        } else {
            ascend();
            TRIGGER_ARRAY_END(Path(this));
        }
        return;
    }

    EXPECT(c == ',');
    push(Value);
    buffer_increment_int();
    descend();
}

void json::RawParser::parse_value(char c) {
    SKIP_WHITESPACES(c)

    switch(c) {
    case '{':
        pop();
        push(ObjectBegin);
        TRIGGER_OBJECT_BEGIN(Path(this));
        return;
    case '[':
        pop();
        push(ArrayBegin);
        TRIGGER_ARRAY_BEGIN(Path(this));
        return;
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        push(Number);
        parse_number(c);
        return;
    case '\"':
        push(StringBegin);
        parse_string(c);
        return;
    case 't':
        parse_true(c);
        return;
    case 'f':
        parse_false(c);
        return;
    case 'n':
        parse_null(c);
        return;
    default:
        break;
    }

    ERROR(ErrorUnexpectedCharacter)
}

void json::RawParser::parse_string(char c) {
    if(peek() == StringBegin) {
        EXPECT(c == '\"');
        pop();
        push(String);
        FLAG_CLEAR(CHAR_ESCAPED);
        return;
    }

    if(FLAG_IS_SET(CHAR_ESCAPED)) {
        switch(c) {
        case '\\':
        case '"':
        case '/':
            buffer_append_char(c);
            break;
        case 'b':
            buffer_append_char('\b');
            break;
        case 'f':
            buffer_append_char('\f');
            break;
        case 'n':
            buffer_append_char('\n');
            break;
        case 'r':
            buffer_append_char('\r');
            break;
        case 't':
            buffer_append_char('\t');
            break;
        case 'u':
            unicode_idx = 0;
            unicode_codepoint = 0;
            FLAG_SET(CHAR_UNICODE);
            break;
        default:
            ERROR(ErrorUnknownEscapeCharacter);
        }
        FLAG_CLEAR(CHAR_ESCAPED);
    } else if(FLAG_IS_SET(CHAR_UNICODE)) {
        EXPECT(IS_HEX(c));

        if(c >= 'a') {
            unicode_codepoint = (unicode_codepoint << 4) + c - 'a' + 0xa;
        } else if(c >= 'A') {
            unicode_codepoint = (unicode_codepoint << 4) + c - 'A' + 0xa;
        } else if(c >= '0') {
            unicode_codepoint = (unicode_codepoint << 4) + (c - '0');
        }
        unicode_idx++;

        if(unicode_idx == 4) {
            register char x = 0;
            if(unicode_codepoint < 0x80) {
                x = (char)unicode_codepoint;
                buffer_append_char(x);
            } else if(unicode_codepoint < 0x8000) {
                x = (char)(0xc0 | (unicode_codepoint >> 6));
                buffer_append_char(x);
                x = (char)(0x80 | (unicode_codepoint & 0x3f));
                buffer_append_char(x);
            } else {
                x = (char)(0xe0 | (unicode_codepoint >> 12));
                buffer_append_char(x);
                x = (char)(0x80 | ((unicode_codepoint >> 6) & 0x3f));
                buffer_append_char(x);
                x = (char)(0x80 | (unicode_codepoint & 0x3f));
                buffer_append_char(x);
            }

            FLAG_CLEAR(CHAR_UNICODE);
            return;
        }
    } else {
        if(c == '\\') {
            FLAG_SET(CHAR_ESCAPED);
            return;
        }

        if(c == '"') {
            buffer_append_char('\0');
            pop();
            return;
        }

        ASSERT(c >= 0x20 && c != 0x7f,
            ErrorUnescapedControlCharacter);

        buffer_append_char(c);
    }
}

void json::RawParser::parse_number(char c) {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        FLAG_CLEAR(NUM_HAS_COMMA | NUM_IS_EXP);

        EXPECT(IS_DIGIT(c) || c == '-');
        buffer_append_char(c);
        return;
    }
    if (m_buffer_sizes[m_buffer_level] == 1) {
        if(IS_DIGIT(c)) {
            EXPECT(buffer_last() != '0');
            buffer_append_char(c);
            return;
        }
        if(buffer_last() == '-') {
            EXPECT(IS_DIGIT(c));
            buffer_append_char(c);
            return;
        }
    }
    if (m_buffer_sizes[m_buffer_level] == 2) {
        if(c == '0' && buffer_first() == '-') {
            EXPECT(buffer_last() != '0');
            buffer_append_char(c);
            return;
        }
        if(buffer_first() == '-' && buffer_last() == '0') {
            EXPECT(c == '.');
            buffer_append_char(c);
            return;
        }
        if(IS_DIGIT(c)) {
            buffer_append_char(c);
            return;
        }
    }

    if(c == '.') {
        EXPECT(FLAG_IS_CLEARED(NUM_HAS_COMMA | NUM_IS_EXP));
        EXPECT(IS_DIGIT(buffer_last()));

        FLAG_SET(NUM_HAS_COMMA);
        buffer_append_char(c);
        return;
    }
    if(c == 'e' || c == 'E') {
        EXPECT(FLAG_IS_CLEARED(NUM_IS_EXP));
        EXPECT(IS_DIGIT(buffer_last()));

        FLAG_SET(NUM_IS_EXP);
        buffer_append_char(c);
        return;
    }

    if(buffer_last() == '.') {
        EXPECT(IS_DIGIT(c));
        buffer_append_char(c);
        return;
    }
    if(buffer_last() == '+' || buffer_last() == '-') {
        EXPECT(IS_DIGIT(c));
        buffer_append_char(c);
        return;
    }
    if(buffer_last() == 'e' || buffer_last() == 'E') {
        EXPECT(c == '+' || c == '-' || IS_DIGIT(c));
        buffer_append_char(c);
        return;
    }
    if(IS_DIGIT(c)) {
        buffer_append_char(c);
        return;
    }

    buffer_append_char('\0');
    pop();
}

void json::RawParser::parse_true(char c) {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        EXPECT(c == 't');
        buffer_append_char('t');
        push(ValueTrue);
        return;
    }

    switch(buffer_last()) {
    case 't':
        EXPECT(c == 'r');
        break;
    case 'r':
        EXPECT(c == 'u');
        break;
    case 'u':
        EXPECT(c == 'e');
        buffer_append_char('e');
        buffer_append_char('\0');
        pop();
        return;
    default:
        ERROR(ErrorUnexpectedCharacter);
    }
    buffer_append_char(c);
}

void json::RawParser::parse_false(char c) {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        EXPECT(c == 'f');
        buffer_append_char('f');
        push(ValueFalse);
        return;
    }

    switch(buffer_last()) {
    case 'f':
        EXPECT(c == 'a');
        break;
    case 'a':
        EXPECT(c == 'l');
        break;
    case 'l':
        EXPECT(c == 's');
        break;
    case 's':
        EXPECT(c == 'e');
        buffer_append_char('e');
        buffer_append_char('\0');
        pop();
        return;
    default:
        ERROR(ErrorUnexpectedCharacter);
    }
    buffer_append_char(c);
}

void json::RawParser::parse_null(char c) {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        EXPECT(c == 'n');
        buffer_append_char('n');
        push(ValueNull);
        return;
    }

    switch(buffer_last()) {
    case 'n':
        EXPECT(c == 'u');
        break;
    case 'u':
        EXPECT(c == 'l');
        break;
    case 'l':
        EXPECT(c == 'l');
        buffer_append_char('l');
        buffer_append_char('\0');
        pop();
        return;
    default:
        ERROR(ErrorUnexpectedCharacter);
    }
    buffer_append_char(c);
}

void json::RawParser::parse(char c) {
    if(FLAG_IS_SET(DOCUMENT_INVALID)) {
        return;
    }

    if(FLAG_IS_SET(DOCUMENT_ENDED)) {
        EXPECT_WHITESPACE(c);
        return;
    }

    if(m_stack_top == 0) {
        SKIP_WHITESPACES(c);

        if(c == '[') {
            TRIGGER_DOCUMENT_BEGIN(Array);
            TRIGGER_ARRAY_BEGIN(Path(this));
            push(ArrayBegin);
            return;
        }
        if(c == '{') {
            TRIGGER_DOCUMENT_BEGIN(Object);
            TRIGGER_OBJECT_BEGIN(Path(this));
            push(ObjectBegin);
            return;
        }
        ERROR(ErrorUnexpectedCharacter);
    }

    switch(peek()) {
    case ObjectBegin:
    case ObjectKey:
    case ObjectColon:
    case Object:
        parse_object(c);
        break;
    case ArrayBegin:
    case Array:
        parse_array(c);
        break;
    case Value:
        parse_value(c);
        break;
    case StringBegin:
    case String:
        parse_string(c);
        if(peek() == Value) {
            pop();
            on_string(Path(this), m_buffers[m_buffer_level]);
            ascend();
        }
        break;
    case Number:
        parse_number(c);
        if(peek() == Value) {
            pop();
            on_number(Path(this), m_buffers[m_buffer_level]);
            ascend();
            parse(c);
        }
        break;
    case ValueTrue:
        parse_true(c);
        if(peek() == Value) {
            pop();
            on_true(Path(this));
            ascend();
        }
        break;
    case ValueFalse:
        parse_false(c);
        if(peek() == Value) {
            pop();
            on_false(Path(this));
            ascend();
        }
        break;
    case ValueNull:
        parse_null(c);
        if(peek() == Value) {
            pop();
            on_null(Path(this));
            ascend();
        }
        break;
    default:
        ERROR(ErrorNotImplemented);
    }
}

void json::RawParser::write(char c) {
    parse(c);
}

void json::RawParser::end_of_transmission() {
    if(FLAG_IS_SET(DOCUMENT_INVALID)) {
        return;
    }

    ASSERT(FLAG_IS_SET(DOCUMENT_ENDED), ErrorDocumentNotClosed);

    reset();
}

json::Path::Path(const json::RawParser *parser) : m_parser(parser) {}

bool json::Path::is_name(unsigned int i) const {
    return i > m_parser->m_buffer_level
        ? false
        : m_parser->m_stack[i] == Object;
}

bool json::Path::is_index(unsigned int i) const {
    return i > m_parser->m_buffer_level
        ? false
        : m_parser->m_stack[i] == Array;
}

int json::Path::as_index(unsigned int i) const {
    if(i > m_parser->m_buffer_level) {
        return 0;
    }

    if(m_parser->m_stack[i] == Array)
    {
        return *(int*)&m_parser->m_buffers[i][0];
    }

    return 0;
}

const char* json::Path::as_name(unsigned int i) const {
    if(i > m_parser->m_buffer_level) {
        return nullptr;
    }

    if(m_parser->m_stack[i] == Object)
    {
        return m_parser->m_buffers[i];
    }

    return nullptr;
}

unsigned int json::Path::key_length(unsigned int i) const {
    if(i > m_parser->m_buffer_level || (m_parser->m_parse_flags & DOCUMENT_ENDED)) {
        return 0;
    }

    return m_parser->m_buffer_sizes[i];
}

unsigned int json::Path::size() const {
    if(m_parser->m_parse_flags & DOCUMENT_ENDED) {
        return 0;
    }
    return m_parser->m_buffer_level + 1;
}













const char _dbg_struct_names[json::Undefined + 1][40] = {
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

void json::RawParser::dump_state() {
    unsigned int struct_name_max_len = 0;
    for(unsigned int i = 0; i < Undefined; i++) {
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