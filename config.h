#ifndef __CONFIG_6TCIIEDT_H
#define __CONFIG_6TCIIEDT_H

#include "chinwag.h"

// data used for versioning information
extern const char* const CW_VERSION;
extern const char* const CW_DATE_YEAR;
extern const char* const CW_DATE_MONTH;
extern const char* const CW_DATE_DAY;
extern const char* const CW_REVISION;

// minimum and maximum fixed buffer sizes
extern const unsigned CW_SMALL_BUFFER;
extern const unsigned CW_LARGE_BUFFER;

// minimum size allowed for dictionary tokens (more == better randomization)
extern const unsigned CW_MIN_DICT_SIZE;

// individual characters used to deliminate dictionary tokens
extern const char* const CW_DELIMITERS;

// pre-defined caps for determining randomized sentence lengths
extern const unsigned CW_SENTENCE_MIN_WORD;
extern const unsigned CW_SENTENCE_MAX_WORD;

// pre-defined caps for determining randomized paragraph lengths
extern const unsigned CW_PARAGRAPH_MIN_SENTENCE;
extern const unsigned CW_PARAGRAPH_MAX_SENTENCE;

// modifiable defaults
extern unsigned CW_DEFAULT_TYPE;
extern unsigned CW_DEFAULT_MIN_OUTPUT;
extern unsigned CW_DEFAULT_MAX_OUTPUT;

#endif
