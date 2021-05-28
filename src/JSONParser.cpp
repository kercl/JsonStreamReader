#include <iostream>
#include <cstring>
#include <cstdio>
#include "JSONParser.h"

#define CHAR_ESCAPED     (1 << 0)
#define NUM_HAS_COMMA    (1 << 1)
#define NUM_IS_EXP       (1 << 2)
#define DOCUMENT_INVALID (1 << 3)

#define FLAG_SET(flags) (m_parse_flags |= flags)
#define FLAG_CLEAR(flags) (m_parse_flags &= ~(flags))
#define FLAG_IS_SET(flags) (m_parse_flags & (flags))
#define FLAG_IS_CLEARED(flags) (!FLAG_IS_SET(flags))

#define IS_DIGIT(c) (c >= '0' && c <= '9')

#define SKIP_WHITESPACES(c) \
    if(is_whitespace(c)) { \
        return;            \
    }                      \

#define ERROR2(msg, ret) \
    FLAG_SET(DOCUMENT_INVALID); \
    std::cout << "(" << __LINE__ << ") ERROR: " << msg << std::endl; \
    return ret;

#define ERROR(msg) ERROR2(msg,)

#define EXPECT(cond)                       \
    if(!(cond)) {                          \
        ERROR("Unexpected character")      \
    }

#define EXPECT_WHITESPACE(c) \
    if(!is_whitespace(c)) { \
        ERROR("Unexpected character") \
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
    if(m_stack_top == JSONSTREAM_STACK_LIMIT) {
        ERROR("oom")
    }

    m_stack[m_stack_top++] = structure;
}

inline json::Type json::RawParser::pop() {
    if(m_stack_top == 0) {
        ERROR2("runtime", Undefined);
    }

    return m_stack[--m_stack_top];
}

inline json::Type json::RawParser::peek() {
    if(m_stack_top == 0) {
        ERROR2("runtime", Undefined);
    }

    return m_stack[m_stack_top - 1];
}

inline void json::RawParser::append_data(char c) {
    if(m_buffer_level == JSONSTREAM_MAX_DEPTH || 
       m_buffer_sizes[m_buffer_level] == JSONSTREAM_BUFFER_LIMIT)
    {
        ERROR("oom");
    }

    m_buffers[m_buffer_level][m_buffer_sizes[m_buffer_level]] = c;
    m_buffer_sizes[m_buffer_level]++;
}

inline void json::RawParser::set_data(int i) {
    int *data = (int*)&m_buffers[m_buffer_level][0];
    *data = i;
    m_buffer_sizes[m_buffer_level] = sizeof(int);
}

inline void json::RawParser::increment_data() {
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
    if(m_buffer_level == JSONSTREAM_MAX_DEPTH) {
        ERROR("oom");
    }

    m_buffer_sizes[++m_buffer_level] = 0;
}

inline void json::RawParser::ascend() {
    if(m_buffer_level == 0) {
        ERROR("runtime");
    }

    m_buffer_sizes[--m_buffer_level] = 0;
}

inline void json::RawParser::clear_data() {
    m_buffer_sizes[m_buffer_level] = 0;
}

void json::RawParser::parse_document(char c) {
    SKIP_WHITESPACES(c)

    switch(c) {
    case '{':
        push(DocumentObject);
        push(ObjectKey);
#if JSONSTREAM_TRIGGER_DOCUMENT_BEGIN
        on_document_begin(Object);
#endif
        return;
    case '[':
        push(DocumentArray);
        push(ArrayBegin);
#if JSONSTREAM_TRIGGER_DOCUMENT_BEGIN
        on_document_begin(Array);
#endif
        parse_array(c);
        return;
    }
}

void json::RawParser::parse_object(char c) {
    SKIP_WHITESPACES(c)

    switch(pop()) {
    case ObjectKey:
        if(c == '}') {
#if JSONSTREAM_TRIGGER_OBJECT_EMPTY
            on_object_empty(Path(this));
#endif
            if(peek() != DocumentObject) {
                ascend();
            } 
#if JSONSTREAM_TRIGGER_DOCUMENT_END
            else {
                on_document_end(Object);
            }
#endif
            return;
        }
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
#if JSONSTREAM_TRIGGER_OBJECT_END
            on_object_end(Path(this));
#endif
            if(peek() != DocumentObject) {
                ascend();
            }
#if JSONSTREAM_TRIGGER_DOCUMENT_END
            else {
                on_document_end(Object);
            }
#endif
            return;
        }
        EXPECT(c == ',');
        push(ObjectKey);
        return;
    default:
        ERROR("unexpected token");
    }
}

void json::RawParser::parse_value(char c) {
    SKIP_WHITESPACES(c)

    switch(c) {
    case '{':
        pop();
        push(ObjectKey);
        return;
    case '[':
        pop();
        push(ArrayBegin);
        parse_array(c);
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
    case ']':
        pop();
        EXPECT(peek() == Array);
        ascend();
#if JSONSTREAM_TRIGGER_OBJECT_EMPTY
        on_array_empty(Path(this));
#endif
        pop();
        return;
    }

    ERROR("unexpected char")
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
            append_data(c);
            break;
        case 'b':
            append_data('\b');
            break;
        case 'f':
            append_data('\f');
            break;
        case 'n':
            append_data('\n');
            break;
        case 'r':
            append_data('\r');
            break;
        case 't':
            append_data('\t');
            break;
        default:
            ERROR("unknown escape character")
        }
        FLAG_CLEAR(CHAR_ESCAPED);
    } else {
        if(c == '\\') {
            FLAG_SET(CHAR_ESCAPED);
            return;
        }

        if(c == '"') {
            append_data('\0');
            pop();
            return;
        }

        append_data(c);
    }
}

void json::RawParser::parse_number(char c) {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        FLAG_CLEAR(NUM_HAS_COMMA | NUM_IS_EXP);

        EXPECT(IS_DIGIT(c) || c == '-');
        append_data(c);
        return;
    }
    if (m_buffer_sizes[m_buffer_level] == 1) {
        if(c == '0') {
            EXPECT(buffer_last() != '0');
            append_data(c);
            return;
        }
        if(IS_DIGIT(c)) {
            append_data(c);
            return;
        }
    }
    if (m_buffer_sizes[m_buffer_level] == 2) {
        if(c == '0' && buffer_first() == '-') {
            EXPECT(buffer_last() != '0');
            append_data(c);
            return;
        }
        if(IS_DIGIT(c)) {
            append_data(c);
            return;
        }
    }

    if(c == '.') {
        EXPECT(FLAG_IS_CLEARED(NUM_HAS_COMMA | NUM_IS_EXP));
        EXPECT(IS_DIGIT(buffer_last()));

        FLAG_SET(NUM_HAS_COMMA);
        append_data(c);
        return;
    }
    if(c == 'e' || c == 'E') {
        EXPECT(FLAG_IS_CLEARED(NUM_IS_EXP));
        EXPECT(IS_DIGIT(buffer_last()));

        FLAG_SET(NUM_IS_EXP);
        append_data(c);
        return;
    }

    if(buffer_last() == '.') {
        EXPECT(IS_DIGIT(c));
        append_data(c);
        return;
    }
    if(buffer_last() == 'e' || buffer_last() == 'E') {
        EXPECT(c == '+' || c == '-' || IS_DIGIT(c));
        append_data(c);
        return;
    }
    if(IS_DIGIT(c)) {
        append_data(c);
        return;
    }

    append_data('\0');
    pop();
}

void json::RawParser::parse_true(char c) {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        EXPECT(c == 't');
        append_data('t');
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
        append_data('e');
        append_data('\0');
        pop();
        return;
    default:
        ERROR("unexpected character");
    }
    append_data(c);
}

void json::RawParser::parse_false(char c) {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        EXPECT(c == 'f');
        append_data('f');
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
        append_data('e');
        append_data('\0');
        pop();
        return;
    default:
        ERROR("unexpected character");
    }
    append_data(c);
}

void json::RawParser::parse_null(char c) {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        EXPECT(c == 'n');
        append_data('n');
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
        append_data('l');
        append_data('\0');
        pop();
        return;
    default:
        ERROR("unexpected character");
    }
    append_data(c);
}

void json::RawParser::parse_array(char c) {
    SKIP_WHITESPACES(c)

    if(peek() == ArrayBegin) {
        EXPECT(c == '[');
        pop();
        push(Array);
        push(Value);
        set_data(0);
        descend();
        return;
    }

    if(c == ']') {
#if JSONSTREAM_TRIGGER_ARRAY_END
        on_array_end(Path(this));
#endif
        pop();
        if(peek() != DocumentArray) {
            ascend();
        }
#if JSONSTREAM_TRIGGER_DOCUMENT_END
        else {
            on_document_end(Array);
        }
#endif
        return;
    }

    EXPECT(c == ',');
    push(Value);
    increment_data();
    descend();
}

void json::RawParser::parse(char c) {
    if(FLAG_IS_SET(DOCUMENT_INVALID)) {
        return;
    }

    if(m_stack_top == 0) {
        parse_document(c);
        return;
    }

    switch(peek()) {
    case DocumentObject:
    case DocumentArray:
        EXPECT_WHITESPACE(c);
        break;
    case ObjectKey:
    case ObjectColon:
    case Object:
        parse_object(c);
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
    case ArrayBegin:
    case Array:
        parse_array(c);
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
        ERROR(c);
    }
}

void json::RawParser::write(char c) {
    parse(c);
}

json::Path::Path(const json::RawParser *parser) : m_parser(parser) {}

bool json::Path::is_name(unsigned int i) const {
    return i >= m_parser->m_buffer_level
        ? false
        : m_parser->m_stack[i + 1] == Object;
}

bool json::Path::is_index(unsigned int i) const {
    return i >= m_parser->m_buffer_level
        ? false
        : m_parser->m_stack[i + 1] == Array;
}

int json::Path::as_index(unsigned int i) const {
    if(i >= m_parser->m_buffer_level) {
        return 0;
    }

    if(m_parser->m_stack[i + 1] == Array)
    {
        return *(int*)&m_parser->m_buffers[i][0];
    }

    return 0;
}

const char* json::Path::as_name(unsigned int i) const {
    if(i >= m_parser->m_buffer_level) {
        return nullptr;
    }

    if(m_parser->m_stack[i + 1] == Object)
    {
        return m_parser->m_buffers[i];
    }

    return nullptr;
}

unsigned int json::Path::key_length(unsigned int i) const {
    if(i >= m_parser->m_buffer_level) {
        return 0;
    }

    return m_parser->m_buffer_sizes[i];
}

unsigned int json::Path::size() const {
    return m_parser->m_buffer_level;
}
