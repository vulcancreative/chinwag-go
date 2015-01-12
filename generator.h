// Interface of the generator file
//
// Contents are confined to object synthesis,
// and randomized object conversions
//
// Created by Chris Calo on 7.3.2013
// Copyright (c) 2012 - 2014, Vulcan Creative, LLC.
// Original "mother" term coined by George Marsaglia, RIP

#ifndef __GENERATOR_Z21Z1VE3_H
#define __GENERATOR_Z21Z1VE3_H

#include "chinwag.h"

// custom string to 32-bit integer hashing function
U32 hash(const char* string);
U32 hashr(const char* string, U32 min, U32 max);

// custom mother-ish RNG; it is mother-like in terms
// of the quality of the random numbers, but 2.5 times as fast. 
U32 mother();
U32 motherr(U32 min, U32 max);
F32 motherf();

#endif
