#pragma once

namespace ffloat {

    inline bool isASCII(char c)
    {
        return static_cast<unsigned char>(c) < 0x80;
    }

    inline bool isLowerAlphaASCII(char c)
    {
        return (c >= 'a' && c <= 'z');
    }

    inline bool isUpperAlphaASCII(char c)
    {
        return (c >= 'A' && c <= 'Z');
    }

    inline bool isAlphaASCII(char c)
    {
        return isLowerAlphaASCII(c) || isUpperAlphaASCII(c);
    }

    inline bool isNumericASCII(char c)
    {
        /// This is faster than
        /// return UInt8(UInt8(c) - UInt8('0')) < UInt8(10);
        /// on Intel CPUs when compiled by gcc 8.
        return (c >= '0' && c <= '9');
    }

    inline bool isAlphaNumericASCII(char c)
    {
        return isAlphaASCII(c)
            || isNumericASCII(c);
    }


    inline bool isWhitespaceASCII(char c)
    {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
    }
}