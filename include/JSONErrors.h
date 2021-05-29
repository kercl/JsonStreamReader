#pragma once

namespace json {

enum ErrorCode {
    NoError = 0,
    ErrorOutOfMemory,
    ErrorUnexpectedCharacter,
    ErrorStackEmpty,
    ErrorBufferEmpty,
    ErrorNotImplemented,
    ErrorUnknownEscapeCharacter
};

}
