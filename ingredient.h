// Interface of the custom atomics abstraction layer
//
// Standardized/hybridized, cross-platform, atomic
// abstract types. Should be used in place of normal
// C/C++ built-ins!
//
// Created by Chris Calo on 7.3.2013
// Copyright (c) 2012 - 2014, Vulcan Creative, LLC.

#ifndef __INGREDIENT_9TPR28FI_H
#define __INGREDIENT_9TPR28FI_H

#include <xmmintrin.h>

#include "chinwag.h"

// byte values
typedef unsigned char Byte;

// bool values
typedef bool B8;

// floating-point values
typedef float F32;

// signed integer values
typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

// unsigned integer values
typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

// packed 128-bit values
/*
class VF128
{
    public:
    VF128(const __m128& vf);
    VF128(const F32& xyzw = 0.0);
    VF128(const F32& x, const F32& y, const F32& z, const F32& w);
    VF128& operator=(const VF128& righthand);
    
    B8 operator!=(const VF128& righthand) const;
    // F32 operator[] (const int& index) const;
    VF128 operator+(const VF128& addend);
    VF128 operator-(const VF128& subtrahend);
    VF128 operator*(const VF128& factor);
    VF128 operator/(const VF128& divisor);

    __m128 cvalue();
    // F32* as_array();
    void set_zero();

    private:
    __m128 p_vecfloat;
};
*/
// global constants
// const U32 BYTES  = 1;
// const U32 KILOBYTES = 1024;
// const U32 MEGABYTES = 1048576;
// const U32 GIGABYTES = 1073741824;

#endif
