#pragma once

namespace json {

enum ErrorCode {
    NoError = 0,
    ErrorOutOfMemory,
    ErrorStackEmpty,
    ErrorBufferEmpty,
    ErrorUnexpectedCharacter,
    ErrorNotImplemented,
    ErrorUnknownEscapeCharacter,
    ErrorDocumentNotClosed,
    ErrorUnescapedControlCharacter,
    ErrorOverflow,
    ErrorUnderflow,
    ErrorLossOfSignificance
};

}
