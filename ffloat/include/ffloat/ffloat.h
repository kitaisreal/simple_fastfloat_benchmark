#pragma once

#include <system_error>
#include <limits>

#include "defines.h"
#include "shift10.h"
#include "string_utils.h"

namespace ffloat {

    struct from_chars_result {
        const char *ptr;
        std::errc ec;
    };

    template <size_t N, typename T>
    static inline void readUIntTextUpToNSignificantDigits(T & x, char *&begin, char *end)
    {
        for (size_t i = 0; i < N; ++i)
        {
            if (begin != end && isNumericASCII(*begin))
            {
                x *= 10;
                x += *begin & 0x0F;
                ++begin;
            }
            else {
                return;
            }
        }

        while (begin != end && isNumericASCII(*begin))
        {
            ++begin;
        }
    }

    template<typename T>
    from_chars_result from_chars(char * begin, char * end, T& x) {
        static_assert(std::is_same_v<T, double> || std::is_same_v<T, float>, "Argument must be float or double");
        static_assert('a' > '.' && 'A' > '.' && '\n' < '.' && '\t' < '.' && '\'' < '.' && '"' < '.', "Layout of char is not like ASCII"); //-V590

        bool negative = false;
        x = 0;
        uint64_t before_point = 0;
        uint64_t after_point = 0;
        int after_point_exponent = 0;
        int exponent = 0;

        if (begin == end)
        {
            return { begin, std::errc::invalid_argument };
        }

        while (begin != end && isWhitespaceASCII(*begin)) 
        {
            ++begin;
        }

        if (*begin == '-')
        {
            negative = true;
            ++begin;
        }


        auto begin_after_sign = begin;

        constexpr int significant_digits = std::numeric_limits<uint64_t>::digits10;
        readUIntTextUpToNSignificantDigits<significant_digits>(before_point, begin, end);

        int read_digits = begin - begin_after_sign;

        if (unlikely(read_digits > significant_digits))
        {
            int before_point_additional_exponent = read_digits - significant_digits;
            x = shift10(before_point, before_point_additional_exponent);
        }
        else
        {
            x = before_point;

            /// Shortcut for the common case when there is an integer that fit in Int64.
            if (read_digits && (begin == end || *begin < '.'))
            {
                if (negative)
                    x = -x;
                return { begin, std::errc() };
            }
        }

        if (*begin == '.')
        {
            ++begin;
            auto after_point_begin = begin;

            while (begin != end && *begin == '0')
                ++begin;

            auto after_leading_zeros_begin = begin;
            auto after_point_num_leading_zeros = after_leading_zeros_begin - after_point_begin;

            readUIntTextUpToNSignificantDigits<significant_digits>(after_point, begin, end);
            read_digits = begin - after_leading_zeros_begin;
            after_point_exponent = (read_digits > significant_digits ? -significant_digits : -read_digits) - after_point_num_leading_zeros;
        }

        if (*begin == 'e' || *begin == 'E')
        {
            ++begin;
            
            if (begin == end)
            {
                return { begin, std::errc::invalid_argument };
            }

            bool exponent_negative = false;
            if (*begin == '-')
            {
                exponent_negative = true;
                ++begin;
            }
            else if (*begin == '+')
            {
                ++begin;
            }

            readUIntTextUpToNSignificantDigits<4>(exponent, begin, end);
            if (exponent_negative)
                exponent = -exponent;
        }

        if (after_point)
            x += shift10(after_point, after_point_exponent);

        if (exponent)
            x = shift10(x, exponent);

        if (negative)
            x = -x;

        auto num_characters_without_sign = begin - begin_after_sign;

        /// Denormals. At most one character is read before denormal and it is '-'.
        if (num_characters_without_sign == 0)
        {
            if (begin == end)
            {
                return { begin, std::errc::invalid_argument };
            }

            if (*begin == '+')
            {
                ++begin;
                if (begin == end)
                {
                    return { begin, std::errc::invalid_argument };
                }
                else if (negative)
                {
                    return { begin, std::errc::invalid_argument };
                }
            }

            /// TODO: Fix
            // if (*in.position() == 'i' || *in.position() == 'I')
            // {
            //     if (assertOrParseInfinity<throw_exception>(in))
            //     {
            //         x = std::numeric_limits<T>::infinity();
            //         if (negative)
            //             x = -x;
            //         return ReturnType(true);
            //     }
            //     return ReturnType(false);
            // }
            // else if (*in.position() == 'n' || *in.position() == 'N')
            // {
            //     if (assertOrParseNaN<throw_exception>(in))
            //     {
            //         x = std::numeric_limits<T>::quiet_NaN();
            //         if (negative)
            //             x = -x;
            //         return ReturnType(true);
            //     }
            //     return ReturnType(false);
            // }
        }

        return { begin, std::errc() };
    }
}