#ifndef __UTILITY_B2605RTY_H
#define __UTILITY_B2605RTY_H

#include "chinwag.h"

bool exclude(char const* str, char const* substr);
bool exclude_num(U32* nums, U32 size, U32 subnum);
bool include(char const* str, char const* substr);
bool include_mtx(char const* str,const char* const* substr,const int sz);

U32 count(char* string, char const* delimiters);

char* substring_with_size(const char* string, U32 start, U32 end);
char* sample_substring_with_size(const char* string, U32 size);
char* add_suffix(char* string, char* suffix);
char* upcase(char* word);
char* downcase(char* word);
char* capitalize(char* word);

#endif
