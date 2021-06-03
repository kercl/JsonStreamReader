#include "JSONParser.h"

#ifdef JSONSTREAM_CHECK_OVERFLOWS
#include <limits.h>
#include <math.h>
#include <float.h>
#endif

#if defined __has_cpp_attribute
    #if __has_cpp_attribute(fallthrough)
        #define __SUPPRESS_FALLTHROUGH_WARNING [[fallthrough]];
    #else
        #define __SUPPRESS_FALLTHROUGH_WARNING
    #endif
#else
    #define __SUPPRESS_FALLTHROUGH_WARNING
#endif

#define JSONSTREAM_CHAR_ESCAPED     (1 << 0)
#define JSONSTREAM_CHAR_UNICODE     (1 << 1)
#define JSONSTREAM_NUM_HAS_COMMA    (1 << 2)
#define JSONSTREAM_NUM_IS_EXP       (1 << 3)
#define JSONSTREAM_DOCUMENT_INVALID (1 << 4)
#define JSONSTREAM_DOCUMENT_ENDED   (1 << 5)

#define JSONSTREAM_FLAG_CLEAR(flags) (m_parse_flags &= ~(flags))
#define JSONSTREAM_FLAG_SET(flags) (m_parse_flags |= flags)
#define JSONSTREAM_FLAG_IS_CLEARED(flags) (!JSONSTREAM_FLAG_IS_SET(flags))
#define JSONSTREAM_FLAG_IS_SET(flags) (m_parse_flags & (flags))

#define JSONSTREAM_IS_DIGIT(c) (c >= '0' && c <= '9')
#define JSONSTREAM_IS_HEX_ALPHA(c) ((c & 0xdf) >= 'A' && (c & 0xdf) <= 'F') // c & 0xdf -> toupper
#define JSONSTREAM_IS_HEX(c) (JSONSTREAM_IS_DIGIT(c) || JSONSTREAM_IS_HEX_ALPHA(c))

#define JSONSTREAM_SKIP_WHITESPACES(c) \
    if(is_whitespace(c)) {             \
        return;                        \
    }


#ifdef JSONSTREAM_CERR_ERROR_MSG
#   include <iostream>
#   define JSONSTREAM_ERROR2(err_code, ret)                \
        JSONSTREAM_FLAG_SET(JSONSTREAM_DOCUMENT_INVALID);  \
        std::cerr << __FILE__ << ":" << __LINE__ <<        \
                     ": error: " << (err_code) << std::endl; \
        on_error(ErrorUnexpectedCharacter);                \
        return ret;
#else
#   define JSONSTREAM_ERROR2(err_code, ret)                \
        JSONSTREAM_FLAG_SET(JSONSTREAM_DOCUMENT_INVALID);  \
        on_error(err_code);                                \
        return ret;
#endif

#define JSONSTREAM_ERROR(err_code) JSONSTREAM_ERROR2(err_code,)

#define JSONSTREAM_ASSERT(cond, err_code) \
    if(!(cond)) {                         \
        JSONSTREAM_ERROR(err_code)        \
    }

#define JSONSTREAM_EXPECT(cond) \
    JSONSTREAM_ASSERT(cond, ErrorUnexpectedCharacter)

#define JSONSTREAM_EXPECT_WHITESPACE(c)            \
    if(!is_whitespace(c)) {                        \
        JSONSTREAM_ERROR(ErrorUnexpectedCharacter) \
    }

json::RawParser::RawParser()
    : m_buffer_level(0)
    , m_stack_top(0)
    , m_parse_flags(0)
{
    m_buffer_sizes[0] = 0;
    m_buffers[0][0] = 0;
    m_stack[0] = Undefined;
}

void json::RawParser::reset() {
    m_buffer_level = 0;
    m_stack_top = 0;
    m_parse_flags = 0;
    m_buffer_sizes[0] = 0;
    m_buffers[0][0] = 0;
    m_stack[0] = Undefined;
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

inline void json::RawParser::push(ParserState structure) {
    m_stack[m_stack_top++] = structure;
}

inline json::ParserState json::RawParser::pop() {
    return m_stack[--m_stack_top];
}

inline json::ParserState json::RawParser::peek() {
    return m_stack[m_stack_top - 1];
}

void json::RawParser::buffer_append_char(char c) {
    if(m_buffer_sizes[m_buffer_level] == JSONSTREAM_BUFFER_LIMIT)
    {
        JSONSTREAM_ERROR(ErrorOutOfMemory);
    }

    m_buffers[m_buffer_level][m_buffer_sizes[m_buffer_level]] = c;
    m_buffer_sizes[m_buffer_level]++;
}

void json::RawParser::buffer_assign_int(int i) {
    int *data = (int*)&m_buffers[m_buffer_level][0];
    *data = i;
    m_buffer_sizes[m_buffer_level] = sizeof(int);
}

int json::RawParser::buffer_as_int() {
    int *data = (int*)&m_buffers[m_buffer_level][0];
    return *data;
}

void json::RawParser::buffer_increment_int() {
    int *data = (int*)&m_buffers[m_buffer_level][0];
    *data = *data + 1;
}

char json::RawParser::buffer_last() {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        return '\0';
    }
    return m_buffers[m_buffer_level][m_buffer_sizes[m_buffer_level] - 1];
}

char json::RawParser::buffer_first() {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        return '\0';
    }
    return m_buffers[m_buffer_level][0];
}

void json::RawParser::descend() {
    m_buffer_sizes[++m_buffer_level] = 0;
}

void json::RawParser::ascend() {
    if(m_buffer_level == 0) {
        JSONSTREAM_ERROR(ErrorBufferEmpty);
    }

    m_buffer_sizes[--m_buffer_level] = 0;
}

void json::RawParser::clear_data() {
    m_buffer_sizes[m_buffer_level] = 0;
}

void json::RawParser::parse_object(char c) {
    JSONSTREAM_SKIP_WHITESPACES(c)

    switch(pop()) { // pop: ObjectBegin|ObjectKey|ObjectColon|Object
    case ObjectBegin:
        if(c == '}') {
            TRIGGER_OBJECT_EMPTY(Path(this));
            TRIGGER_OBJECT_END(Path(this));
            if(m_stack_top == 0) {
                JSONSTREAM_FLAG_SET(JSONSTREAM_DOCUMENT_ENDED);
                TRIGGER_DOCUMENT_END(Object);
            }
            else {
                ascend();
            }
            return;
        }
        __SUPPRESS_FALLTHROUGH_WARNING
    case ObjectKey:
        JSONSTREAM_EXPECT(c == '\"');
        push(ObjectColon);
        push(StringBegin);
        // parse_string(c);
        return;
    case ObjectColon:
        JSONSTREAM_EXPECT(c == ':');
        push(Object);
        push(Value);
        descend();
        return;
    case Object:
        if(c == '}') {
            if(m_stack_top == 0) {
                JSONSTREAM_FLAG_SET(JSONSTREAM_DOCUMENT_ENDED);
                TRIGGER_OBJECT_END(Path(this));
                TRIGGER_DOCUMENT_END(Object);
            } else {
                ascend();
                TRIGGER_OBJECT_END(Path(this));
            }
            return;
        }
        JSONSTREAM_EXPECT(c == ',');
        push(ObjectKey);
        return;
    default:
        JSONSTREAM_ERROR(ErrorNotImplemented);
    }
}

void json::RawParser::parse_array(char c) {
    JSONSTREAM_SKIP_WHITESPACES(c)

    if(peek() == ArrayBegin) {
        pop();
        if(c == ']') {
            TRIGGER_ARRAY_EMPTY(Path(this));
            TRIGGER_ARRAY_END(Path(this));

            if(m_stack_top == 0) {
                JSONSTREAM_FLAG_SET(JSONSTREAM_DOCUMENT_ENDED);
                TRIGGER_DOCUMENT_END(Array);
            } else {
                ascend();
            }
            return;
        }
        JSONSTREAM_EXPECT(c != ',');
        push(Array);
        push(Value);
        buffer_assign_int(0);
        descend();
        parse_value(c);
        return;
    }

    if(c == ']') {
        pop(); // pop Array
        if(m_stack_top == 0) {
            JSONSTREAM_FLAG_SET(JSONSTREAM_DOCUMENT_ENDED);
            TRIGGER_ARRAY_END(Path(this));
            TRIGGER_DOCUMENT_END(Array);
        } else {
            ascend();
            TRIGGER_ARRAY_END(Path(this));
        }
        return;
    }

    JSONSTREAM_EXPECT(c == ',');
    push(Value);
    buffer_increment_int();
    descend();
}

void json::RawParser::parse_value(char c) {
    JSONSTREAM_SKIP_WHITESPACES(c)

    if(JSONSTREAM_IS_DIGIT(c) || c == '-') {
        push(Number);
        JSONSTREAM_FLAG_CLEAR(JSONSTREAM_NUM_HAS_COMMA | JSONSTREAM_NUM_IS_EXP);

        JSONSTREAM_EXPECT(JSONSTREAM_IS_DIGIT(c) || c == '-');
        buffer_append_char(c);
        return;
    }

    switch(c) {
    case '{':
        pop(); // pop Value
        push(ObjectBegin);
        TRIGGER_OBJECT_BEGIN(Path(this));
        return;
    case '[':
        pop(); // pop Value
        push(ArrayBegin);
        TRIGGER_ARRAY_BEGIN(Path(this));
        return;
    case '\"':
        push(StringBegin);
        //parse_string(c);
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

    JSONSTREAM_ERROR(ErrorUnexpectedCharacter)
}

void json::RawParser::parse_string(char c) {
    if(peek() == StringBegin) {
        pop();
        push(String);
        JSONSTREAM_FLAG_CLEAR(JSONSTREAM_CHAR_ESCAPED|JSONSTREAM_CHAR_UNICODE);
    }

    if(JSONSTREAM_FLAG_IS_SET(JSONSTREAM_CHAR_ESCAPED)) {
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
            JSONSTREAM_FLAG_SET(JSONSTREAM_CHAR_UNICODE);
            break;
        default:
            JSONSTREAM_ERROR(ErrorUnknownEscapeCharacter);
        }
        JSONSTREAM_FLAG_CLEAR(JSONSTREAM_CHAR_ESCAPED);
    } else if(JSONSTREAM_FLAG_IS_SET(JSONSTREAM_CHAR_UNICODE)) {
        JSONSTREAM_EXPECT(JSONSTREAM_IS_HEX(c));

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

            JSONSTREAM_FLAG_CLEAR(JSONSTREAM_CHAR_UNICODE);
            return;
        }
    } else {
        if(c == '\\') {
            JSONSTREAM_FLAG_SET(JSONSTREAM_CHAR_ESCAPED);
        } else if(c == '"') {
            buffer_append_char('\0');
            pop();
        } else {
            JSONSTREAM_ASSERT(c >= 0x20 && c != 0x7f,
                ErrorUnescapedControlCharacter);

            buffer_append_char(c);
        }
    }
}

void json::RawParser::parse_number(char c) {
    switch(m_buffer_sizes[m_buffer_level]) {
    case 1:
        if(JSONSTREAM_IS_DIGIT(c)) {
            JSONSTREAM_EXPECT(buffer_last() != '0');
            buffer_append_char(c);
            return;
        }
        if(buffer_last() == '-') {
            JSONSTREAM_EXPECT(JSONSTREAM_IS_DIGIT(c));
            buffer_append_char(c);
            return;
        }
        break;
    case 2:
        if(c == '0' && buffer_first() == '-') {
            JSONSTREAM_EXPECT(buffer_last() != '0');
            buffer_append_char(c);
            return;
        }
        if(buffer_first() == '-' && buffer_last() == '0') {
            JSONSTREAM_EXPECT(!JSONSTREAM_IS_DIGIT(c));
            break;
        }
        if(JSONSTREAM_IS_DIGIT(c)) {
            buffer_append_char(c);
            return;
        }
        break;
    default:
        break;
    }

    switch(c) {
    case '.':
        JSONSTREAM_EXPECT(
            JSONSTREAM_FLAG_IS_CLEARED(
                JSONSTREAM_NUM_HAS_COMMA | JSONSTREAM_NUM_IS_EXP)
            && JSONSTREAM_IS_DIGIT(buffer_last()));

        JSONSTREAM_FLAG_SET(JSONSTREAM_NUM_HAS_COMMA);
        buffer_append_char(c);
        return;
    case 'e':
    case 'E':
        JSONSTREAM_EXPECT(
            JSONSTREAM_FLAG_IS_CLEARED(JSONSTREAM_NUM_IS_EXP)
            && JSONSTREAM_IS_DIGIT(buffer_last())
        );

        JSONSTREAM_FLAG_SET(JSONSTREAM_NUM_IS_EXP);
        buffer_append_char(c);
        return;
    default:
        if(JSONSTREAM_IS_DIGIT(c)) {
            buffer_append_char(c);
            return;
        }
    }

    switch(buffer_last()) {
    case '.':
    case '+':
    case '-':
        JSONSTREAM_EXPECT(JSONSTREAM_IS_DIGIT(c));
        buffer_append_char(c);
        return;
    case 'e':
    case 'E':
        JSONSTREAM_EXPECT(c == '+' || c == '-' || JSONSTREAM_IS_DIGIT(c));
        buffer_append_char(c);
        return;
    default:
        break;
    }

    buffer_append_char('\0');
    pop();
}

void json::RawParser::parse_true(char c) {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        JSONSTREAM_EXPECT(c == 't');
        buffer_append_char('t');
        push(ValueTrue);
        return;
    }

    switch(buffer_last()) {
    case 't':
        JSONSTREAM_EXPECT(c == 'r');
        break;
    case 'r':
        JSONSTREAM_EXPECT(c == 'u');
        break;
    case 'u':
        JSONSTREAM_EXPECT(c == 'e');
        buffer_append_char('e');
        buffer_append_char('\0');
        pop();
        return;
    default:
        JSONSTREAM_ERROR(ErrorUnexpectedCharacter);
    }
    buffer_append_char(c);
}

void json::RawParser::parse_false(char c) {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        JSONSTREAM_EXPECT(c == 'f');
        buffer_append_char('f');
        push(ValueFalse);
        return;
    }
    
    switch(buffer_last()) {
    case 'f':
        JSONSTREAM_EXPECT(c == 'a');
        break;
    case 'a':
        JSONSTREAM_EXPECT(c == 'l');
        break;
    case 'l':
        JSONSTREAM_EXPECT(c == 's');
        break;
    case 's':
        JSONSTREAM_EXPECT(c == 'e');
        buffer_append_char('e');
        buffer_append_char('\0');
        pop();
        return;
    default:
        JSONSTREAM_ERROR(ErrorUnexpectedCharacter);
    }
    buffer_append_char(c);
}

void json::RawParser::parse_null(char c) {
    if(m_buffer_sizes[m_buffer_level] == 0) {
        JSONSTREAM_EXPECT(c == 'n');
        buffer_append_char('n');
        push(ValueNull);
        return;
    }

    switch(buffer_last()) {
    case 'n':
        JSONSTREAM_EXPECT(c == 'u');
        break;
    case 'u':
        JSONSTREAM_EXPECT(c == 'l');
        break;
    case 'l':
        JSONSTREAM_EXPECT(c == 'l');
        buffer_append_char('l');
        buffer_append_char('\0');
        pop();
        return;
    default:
        JSONSTREAM_ERROR(ErrorUnexpectedCharacter);
    }
    buffer_append_char(c);
}

void json::RawParser::parse(char c) {
    if(JSONSTREAM_FLAG_IS_SET(JSONSTREAM_DOCUMENT_INVALID)) {
        return;
    }

    if(m_stack_top >= JSONSTREAM_STACK_LIMIT - 2
        || m_buffer_level == JSONSTREAM_MAX_DEPTH - 1) 
    {
        JSONSTREAM_ERROR(ErrorOutOfMemory);
    }

    if(JSONSTREAM_FLAG_IS_SET(JSONSTREAM_DOCUMENT_ENDED)) {
        JSONSTREAM_EXPECT_WHITESPACE(c);
        return;
    }

    if(m_stack_top == 0) {
        JSONSTREAM_SKIP_WHITESPACES(c);

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
        TRIGGER_DOCUMENT_BEGIN(Value);
        push(Value);
    }

    switch(peek()) {
    case ObjectBegin:
    case ObjectKey:
    case ObjectColon:
    case Object:
        parse_object(c);
        return;
    case ArrayBegin:
    case Array:
        parse_array(c);
        return;
    case Value:
        parse_value(c);
        return;
    case StringBegin:
    case String:
        parse_string(c);
        if(peek() == Value) {
            pop();
            on_string(Path(this), m_buffers[m_buffer_level]);
            if(m_stack_top) {
                ascend();
                return;
            }
            break;
        }
        return;
    case Number:
        parse_number(c);
        if(peek() == Value) {
            pop();
            on_number(Path(this), m_buffers[m_buffer_level]);
            if(m_stack_top) {
                ascend();
                parse(c);
                return;
            }
            JSONSTREAM_EXPECT_WHITESPACE(c);
            break;
        }
        return;
    case ValueTrue:
        parse_true(c);
        if(peek() == Value) {
            pop();
            on_true(Path(this));
            if(m_stack_top) {
                ascend();
                return;
            }
            break;
        }
        return;
    case ValueFalse:
        parse_false(c);
        if(peek() == Value) {
            pop();
            on_false(Path(this));
            if(m_stack_top) {
                ascend();
                return;
            }
            break;
        }
        return;
    case ValueNull:
        parse_null(c);
        if(peek() == Value) {
            pop();
            on_null(Path(this));
            if(m_stack_top) {
                ascend();
                return;
            }
            break;
        }
        return;
    default:
        JSONSTREAM_ERROR(ErrorNotImplemented);
    }
    JSONSTREAM_FLAG_SET(JSONSTREAM_DOCUMENT_ENDED);
    TRIGGER_DOCUMENT_END(Value);
}

void json::RawParser::write(const char *data) {
    for(size_t i = 0; data[i]; ++i) {
        write(data[i]);
    }
}

void json::RawParser::write(uint8_t *data, size_t len) {
    for(size_t i = 0; i < len; ++i) {
        write((char)data[i]);
    }
}

void json::RawParser::write(char c) {
    parse(c);
}

void json::RawParser::end_of_transmission() {
    if(JSONSTREAM_FLAG_IS_SET(JSONSTREAM_DOCUMENT_INVALID)) {
        return;
    }
    parse('\n');

    JSONSTREAM_ASSERT(JSONSTREAM_FLAG_IS_SET(JSONSTREAM_DOCUMENT_ENDED),
                      ErrorDocumentNotClosed);

    reset();
}


bool json::Parser::add_digit(int *number, uint8_t digit) {
#ifdef JSONSTREAM_CHECK_OVERFLOWS
    if(*number > INT_MAX / 10) {
        return false;
    }
#endif
    *number *= 10;
#ifdef JSONSTREAM_CHECK_OVERFLOWS
    if(*number > INT_MAX - digit) {
        return false;
    }
#endif
    *number += digit;
    return true;
}

// TODO: loss of significance check is not working properly yet
void json::Parser::on_number(const Path& path, const char *str) {
    int8_t is_signed = 0, exp_is_signed = 0;

#ifndef JSONSTREAM_ALL_NUMBERS_FLOAT
    if(!JSONSTREAM_FLAG_IS_SET(JSONSTREAM_NUM_HAS_COMMA) &&
       !JSONSTREAM_FLAG_IS_SET(JSONSTREAM_NUM_IS_EXP))
    {
        int parsed_num = 0;
        if(str[0] == '-') {
            is_signed = 1;
        }
        for(int8_t i = is_signed; i < m_buffer_sizes[m_buffer_level] - 1; ++i) {
            bool success = add_digit(&parsed_num, str[i] - '0');
#ifdef JSONSTREAM_CHECK_OVERFLOWS
            JSONSTREAM_ASSERT(success, is_signed ?
                ErrorUnderflow : ErrorOverflow);
#endif
        }

        on_integer(path, parsed_num * (1 - (is_signed << 1)));
        return;
    }
#endif

    int32_t num_parts[3] = {0, 0, 0};
    int8_t part_idx = 0, post_comma_digits = 0;
    float result = 0.0f;

    if(str[0] == '-') {
        is_signed = 1;
    }

    for(int8_t i = is_signed; i < m_buffer_sizes[m_buffer_level] - 1; ++i) {
        if(str[i] == '.') {
            part_idx = 1;
        } else if(str[i] == 'e' || str[i] == 'E') {
            part_idx = 2;
            if(str[i + 1] == '-') {
                exp_is_signed = 1;
                ++i;
            } else if(str[i + 1] == '+') {
                ++i;
            }
        } else {
            bool success = add_digit(&num_parts[part_idx], str[i] - '0');

            if(part_idx == 0) {
#ifdef JSONSTREAM_CHECK_OVERFLOWS
                JSONSTREAM_ASSERT(success, is_signed ?
                    ErrorUnderflow : ErrorOverflow);
#endif
            } else if(part_idx == 1) {
#ifdef JSONSTREAM_CHECK_LOSS_OF_SIGNIFICANCE
                JSONSTREAM_ASSERT(success, ErrorLossOfSignificance);
#endif
                if(success) {
                    ++post_comma_digits;
                }
            } else {
                if(exp_is_signed) {
#ifdef JSONSTREAM_CHECK_LOSS_OF_SIGNIFICANCE
                    JSONSTREAM_ASSERT(success, ErrorLossOfSignificance);
#endif
                } else {
#ifdef JSONSTREAM_CHECK_OVERFLOWS
                    JSONSTREAM_ASSERT(success, ErrorOverflow);
#endif
                }

            }
        }
    }

#ifdef JSONSTREAM_CHECK_OVERFLOWS
    JSONSTREAM_ASSERT(num_parts[2] < FLT_MAX_10_EXP, ErrorOverflow);
#endif

    result = num_parts[1];
    while(post_comma_digits) {
        if(post_comma_digits >= 8) {
            result *= 1e-8f;
            post_comma_digits -= 8;
        } else if(post_comma_digits >= 4) {
            result *= 1e-4f;
            post_comma_digits -= 4;
        } else if(post_comma_digits >= 2) {
            result *= 1e-2f;
            post_comma_digits -= 2;
        } else if(post_comma_digits >= 1) {
            result *= 1e-1f;
            post_comma_digits -= 1;
        }
    }

    result += num_parts[0];

    const float factors_n[] = {1e-1f, 1e-2f, 1e-4f, 1e-8f, 1e-16f, 1e-32f},
                factors_p[] = {1e+1f, 1e+2f, 1e+4f, 1e+8f, 1e+16f, 1e+32f};

    while(num_parts[2] > 0) {
        for(uint8_t i = 5; i != 0; --i) {
            if(num_parts[2] >= (1 << i)) {
                result *= exp_is_signed ? factors_n[i] : factors_p[i];
                num_parts[2] -= 1 << i;
                break;
            }
        }
    }

#ifdef JSONSTREAM_CHECK_OVERFLOWS
    JSONSTREAM_ASSERT(!isinf(result), is_signed ?
        ErrorUnderflow : ErrorOverflow);
#endif

    on_float(path, result * (is_signed ? -1.0f : 1.0f));
}

void json::Parser::on_true(const Path& path) {
    on_boolean(path, true);
}

void json::Parser::on_false(const Path& path) {
    on_boolean(path, false);
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
    if(i > m_parser->m_buffer_level
       || (m_parser->m_parse_flags & JSONSTREAM_DOCUMENT_ENDED))
    {
        return 0;
    }

    return m_parser->m_buffer_sizes[i];
}

unsigned int json::Path::size() const {
    if(m_parser->m_parse_flags & JSONSTREAM_DOCUMENT_ENDED) {
        return 0;
    }
    return m_parser->m_buffer_level + 1;
}
