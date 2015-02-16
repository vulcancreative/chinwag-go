#ifndef __DICT_CO6CSUAJ_H
#define __DICT_CO6CSUAJ_H

#include "chinwag.h"

// exposed dictionary row routines
char* cwdrow_sample
(cwdrow_t drow);

// dictionary utilities
cwdict_t cwdict_open();

cwdict_t cwdict_open_with_name
(const char* name);

cwdict_t cwdict_open_with_tokens
(const char* const buffer, const char* delimiters);

cwdict_t cwdict_open_with_name_and_tokens
(const char* name, const char* const buffer, const char* delimiters);

cwdict_t cwdict_add_row
(cwdict_t dict, cwdrow_t drow);

cwdict_t cwdict_add_row_strict
(cwdict_t dict, cwdrow_t drow, U32 size);

cwdict_t cwdict_place_word
(cwdict_t dict, const char* word);

cwdict_t cwdict_place_words
(cwdict_t dict, const char* const* words, U32 s);

cwdict_t cwdict_place_word_strict
(cwdict_t dict, const char* word);

cwdict_t cwdict_place_words_strict
(cwdict_t dict, const char* const* words, U32 s);

cwdict_t cwdict_sort
(cwdict_t dict);

cwdict_t cwdict_prune
(cwdict_t dict, bool sorted, bool deep);

cwdict_t cwdict_clean // aliases cwdict_prune(dict, true)
(cwdict_t dict);

cwdict_t cwdict_map
(cwdict_t dict, char* (*f)(char*));

cwdict_t cwdict_clone
(cwdict_t dict);

cwdict_t cwdict_dup // aliases cwdict_clone
(cwdict_t dict);

bool cwdict_exclude
(cwdict_t dict, char const* str);

bool cwdict_include
(cwdict_t dict, char const* str);

bool cwdict_valid
(cwdict_t dict, cwerror_t* error);

bool cwdict_equal
(cwdict_t dict, cwdict_t against);

bool cwdict_inequal
(cwdict_t dict, cwdict_t against);

U32 cwdict_length
(cwdict_t dict);

U32 cwdict_size // aliases cwdict_length
(cwdict_t dict);

U32 cwdict_count // aliases cwdict_length
(cwdict_t dict);

U32 cwdict_largest
(cwdict_t dict);

char* cwdict_sample
(cwdict_t dict);

char* cwdict_join
(cwdict_t dict, char const* delimiter);

cwdict_t cwdict_close
(cwdict_t dict);

// secondary utilities
void puts_cwdict
(cwdict_t dict);

void validate_cwdict
(cwdict_t dict, char const* function_name);

cwdict_t split_into_cwdict
(const char* buffer, const char* delimiters);

#endif
