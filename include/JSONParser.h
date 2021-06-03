#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "JSONConfig.h"
#include "JSONErrors.h"

#define JSONSTREAM_STACK_LIMIT JSONSTREAM_MAX_DEPTH + 3

static_assert(JSONSTREAM_BUFFER_LIMIT >= sizeof(int),
    "JSON_MAX_DATA_LENGTH needs to be at least "
    "sizeof(int) to be able to store array indices.");

namespace json {

#if JSONSTREAM_BUFFER_LIMIT <= 0xff
typedef uint8_t buffer_int_t;
#elif JSONSTREAM_BUFFER_LIMIT <= 0xffff
typedef uint16_t buffer_int_t;
#elif JSONSTREAM_BUFFER_LIMIT <= 0xffffffff
typedef uint32_t buffer_int_t;
#else
typedef uint64_t buffer_int_t;
#endif

#if JSONSTREAM_STACK_LIMIT <= 0xff
typedef uint8_t stack_int_t;
#elif JSONSTREAM_STACK_LIMIT <= 0xffff
typedef uint16_t stack_int_t;
#elif JSONSTREAM_STACK_LIMIT <= 0xffffffff
typedef uint32_t stack_int_t;
#else
typedef uint64_t stack_int_t;
#endif

#ifdef JSONSTREAM_ACT_ON_DOCUMENT_BEGIN
#   define TRIGGER_DOCUMENT_BEGIN(type) \
        on_document_begin(type)
    #define DEF_TRIGGER_DOCUMENT_BEGIN \
        virtual void on_document_begin(Type type) = 0
#else
#   define TRIGGER_DOCUMENT_BEGIN
#   define DEF_TRIGGER_DOCUMENT_BEGIN
#endif
#ifdef JSONSTREAM_ACT_ON_DOCUMENT_END
#   define TRIGGER_DOCUMENT_END(type) \
        on_document_end(type)
#   define DEF_TRIGGER_DOCUMENT_END \
        virtual void on_document_end(Type type) = 0
#else
#   define TRIGGER_DOCUMENT_END
#   define DEF_TRIGGER_DOCUMENT_END
#endif
#ifdef JSONSTREAM_ACT_ON_OBJECT_BEGIN
#   define TRIGGER_OBJECT_BEGIN(path) \
        on_object_begin(path)
    #define DEF_TRIGGER_OBJECT_BEGIN \
        virtual void on_object_begin(const Path& path) = 0
#else
#   define TRIGGER_OBJECT_BEGIN
#   define DEF_TRIGGER_OBJECT_BEGIN
#endif
#ifdef JSONSTREAM_ACT_ON_OBJECT_END
#   define TRIGGER_OBJECT_END(path) \
        on_object_end(path)
    #define DEF_TRIGGER_OBJECT_END \
        virtual void on_object_end(const Path& path) = 0
#else
#   define TRIGGER_OBJECT_END
#   define DEF_TRIGGER_OBJECT_END
#endif
#ifdef JSONSTREAM_ACT_ON_OBJECT_EMPTY
#   define TRIGGER_OBJECT_EMPTY(path) \
        on_object_empty(path)
    #define DEF_TRIGGER_OBJECT_EMPTY \
        virtual void on_object_empty(const Path& path) = 0
#else
#   define TRIGGER_OBJECT_EMPTY
#   define DEF_TRIGGER_OBJECT_EMPTY
#endif
#ifdef JSONSTREAM_ACT_ON_ARRAY_BEGIN
#   define TRIGGER_ARRAY_BEGIN(path) \
        on_array_begin(path)
    #define DEF_TRIGGER_ARRAY_BEGIN \
        virtual void on_array_begin(const Path& path) = 0
#else
#   define TRIGGER_ARRAY_BEGIN
#   define DEF_TRIGGER_ARRAY_BEGIN
#endif
#ifdef JSONSTREAM_ACT_ON_ARRAY_END
#   define TRIGGER_ARRAY_END(path) \
        on_array_end(path)
    #define DEF_TRIGGER_ARRAY_END \
        virtual void on_array_end(const Path& path) = 0
#else
#   define TRIGGER_ARRAY_END
#   define DEF_TRIGGER_ARRAY_END
#endif
#ifdef JSONSTREAM_ACT_ON_ARRAY_EMPTY
#   define TRIGGER_ARRAY_EMPTY(path) \
        on_array_empty(path)
    #define DEF_TRIGGER_ARRAY_EMPTY \
        virtual void on_array_empty(const Path& path) = 0
#else
#   define TRIGGER_ARRAY_EMPTY
#   define DEF_TRIGGER_ARRAY_EMPTY
#endif

enum ParserState: uint8_t {
    DocumentObject,
    DocumentArray,
    Object,
    ObjectBegin,
    ObjectKey,
    ObjectColon,
    Value,
    ArrayBegin,
    Array,
    Number,
    StringBegin,
    String,
    ValueTrue,
    ValueFalse,
    ValueNull,
    Undefined
};

typedef ParserState Type;

class Path;
class Parser;

class RawParser {
private:
    friend class Path;
    friend class Parser;

#ifdef JSONSTREAM_DEVELOPMENT
public:
#endif
    char m_buffers[JSONSTREAM_MAX_DEPTH][JSONSTREAM_BUFFER_LIMIT];
    ParserState m_stack[JSONSTREAM_STACK_LIMIT + 1];
    buffer_int_t m_buffer_sizes[JSONSTREAM_MAX_DEPTH];
    stack_int_t m_buffer_level, m_stack_top;
    uint8_t m_parse_flags;

    uint8_t unicode_idx;
    uint16_t unicode_codepoint;

    inline void push(ParserState state);
    inline ParserState pop();
    inline ParserState peek();

    void descend();
    void ascend();
    void clear_data();

    void buffer_append_char(char c);
    void buffer_assign_int(int i);
    int buffer_as_int();
    void buffer_increment_int();
    char buffer_last();
    char buffer_first();

    bool is_whitespace(char c);

    void parse_object(char c);
    void parse_value(char c);
    void parse_string(char c);
    void parse_number(char c);
    void parse_true(char c);
    void parse_false(char c);
    void parse_null(char c);
    void parse_array(char c);
public:
    RawParser();
    virtual ~RawParser();

    void reset();
    void parse(char c);
    void end_of_transmission();

    void write(const char *data);
    void write(uint8_t *data, size_t len);
    void write(char c);
#ifdef JSONSTREAM_DEVELOPMENT
    void dump_state();
#endif

    DEF_TRIGGER_DOCUMENT_BEGIN;
    DEF_TRIGGER_DOCUMENT_END;
    DEF_TRIGGER_OBJECT_BEGIN;
    DEF_TRIGGER_OBJECT_END;
    DEF_TRIGGER_OBJECT_EMPTY;
    DEF_TRIGGER_ARRAY_BEGIN;
    DEF_TRIGGER_ARRAY_END;
    DEF_TRIGGER_ARRAY_EMPTY;

    virtual void on_string(const Path& path, const char *value) = 0;
    virtual void on_number(const Path& path, const char *value) = 0;
    virtual void on_true(const Path& path) = 0;
    virtual void on_false(const Path& path) = 0;
    virtual void on_null(const Path& path) = 0;

    virtual void on_error(ErrorCode error_code) = 0;
};

class Parser : public RawParser {
    bool add_digit(int *number, uint8_t digit);
public:
    virtual void on_number(const Path& path, const char *str) override;
    virtual void on_true(const Path& path) override;
    virtual void on_false(const Path& path) override;

    virtual void on_integer(const Path& path, int value) = 0;
    virtual void on_float(const Path& path, float value) = 0;
    virtual void on_boolean(const Path& path, bool value) = 0;
};

class Path {
#ifdef JSONSTREAM_DEVELOPMENT
    public:
#endif
    friend class RawParser;

    const RawParser *m_parser;
    Path(const RawParser *parser);
public:
    int as_index(unsigned int i) const;
    const char* as_name(unsigned int i) const;

    unsigned int key_length(unsigned int i) const;

    bool is_index(unsigned int i) const;
    bool is_name(unsigned int i) const;

    unsigned int size() const;
};

}
