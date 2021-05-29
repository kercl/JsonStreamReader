#pragma once

#include <stdint.h>

#include "JSONConfig.h"
#include "JSONErrors.h"

#define JSONSTREAM_STACK_LIMIT JSONSTREAM_MAX_DEPTH + 5

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

enum Type: uint8_t {
    DocumentObject,
    DocumentArray,
    ObjectBegin,
    Object,
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

class Path;

class RawParser {
private:
    friend class Path;

    char m_buffers[JSONSTREAM_MAX_DEPTH][JSONSTREAM_BUFFER_LIMIT];
    Type m_stack[JSONSTREAM_STACK_LIMIT];
    buffer_int_t m_buffer_sizes[JSONSTREAM_MAX_DEPTH];
    stack_int_t m_buffer_level, m_stack_top;
    uint8_t m_parse_flags;

    uint8_t unicode_idx;
    uint16_t unicode_codepoint;

    inline void push(Type state);
    inline Type pop();
    inline Type peek();

    inline void descend();
    inline void ascend();
    inline void clear_data();

    inline void buffer_append_char(char c);
    inline void buffer_assign_int(int i);
    inline void buffer_increment_int();
    inline char buffer_last();
    inline char buffer_first();

    bool is_whitespace(char c);

    void parse_document(char c);
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

    virtual void write(char c);
    void dump_state();

#if JSONSTREAM_TRIGGER_DOCUMENT_BEGIN
    virtual void on_document_begin(Type type) = 0;
#endif
#if JSONSTREAM_TRIGGER_DOCUMENT_END
    virtual void on_document_end(Type type) = 0;
#endif

#if JSONSTREAM_TRIGGER_ARRAY_BEGIN
    virtual void on_array_begin(const Path& path) = 0;
#endif
#if JSONSTREAM_TRIGGER_ARRAY_EMPTY
    virtual void on_array_empty(const Path& path) = 0;
#endif
#if JSONSTREAM_TRIGGER_ARRAY_END
    virtual void on_array_end(const Path& path) = 0;
#endif

#if JSONSTREAM_TRIGGER_OBJECT_BEGIN
    virtual void on_object_begin(const Path& path) = 0;
#endif
#if JSONSTREAM_TRIGGER_OBJECT_EMPTY
    virtual void on_object_empty(const Path& path) = 0;
#endif
#if JSONSTREAM_TRIGGER_OBJECT_END
    virtual void on_object_end(const Path& path) = 0;
#endif

    virtual void on_string(const Path& path, const char *value) = 0;
    virtual void on_number(const Path& path, const char *str) = 0;
    virtual void on_true(const Path& path) = 0;
    virtual void on_false(const Path& path) = 0;
    virtual void on_null(const Path& path) = 0;

    virtual void on_error(ErrorCode error_code) = 0;
};

class Parser : public RawParser {
public:
    virtual void on_number(const Path& path, const char *str) override;
    virtual void on_true(const Path& path) override;
    virtual void on_false(const Path& path) override;

    virtual void on_int(const Path& path, int value) = 0;
    virtual void on_float(const Path& path, float value) = 0;
    virtual void on_boolean(const Path path, bool value) = 0;
    virtual void on_null(const Path& path) = 0;
};

class Path {
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
