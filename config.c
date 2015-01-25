#include "config.h"

const char* const CW_VERSION = "1.2.3";
const char* const CW_DATE_YEAR = "2015";
const char* const CW_DATE_MONTH = "01";
const char* const CW_DATE_DAY = "24";
const char* const CW_REVISION = "75";

const unsigned CW_SMALL_BUFFER = 1024;
const unsigned CW_LARGE_BUFFER = 5120;

const unsigned CW_MIN_DICT_SIZE = 300;

const char* const CW_DELIMITERS = "\r\n,;:\034";

const unsigned CW_SENTENCE_MIN_WORD = 2;
const unsigned CW_SENTENCE_MAX_WORD = 25;

const unsigned CW_PARAGRAPH_MIN_SENTENCE = 4;
const unsigned CW_PARAGRAPH_MAX_SENTENCE = 6;

unsigned CW_DEFAULT_TYPE = 1; // == WORDS
unsigned CW_DEFAULT_MIN_OUTPUT = 1;
unsigned CW_DEFAULT_MAX_OUTPUT = 5;
