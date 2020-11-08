#pragma once

#include <cstdint>

namespace ffloat {
    
    double shift10(double x, int exponent);
    float shift10(float x, int exponent);

    double shift10(uint64_t x, int exponent);
    double shift10(int64_t x, int exponent);

}