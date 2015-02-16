//
//  chinwag.h
//  Chinwag
//
//  Created by Chris Calo on 8/05/14.
//  Copyright (c) 2014 Vulcan Creative, LLC. All rights reserved.
//

#ifndef __CHINWAG_AUGXMJP4_H
#define __CHINWAG_AUGXMJP4_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>

// output types
enum {
  CW_LETTERS      =   0,
  CW_WORDS        =   1,
  CW_SENTENCES    =   2,
  CW_PARAGRAPHS   =   3,
};
typedef unsigned long cw_t;

// error types
enum {
  CWERROR_INVALID_OUTPUT_TYPE   =   0,
  CWERROR_MIN_LESS_THAN_ONE     =   1,
  CWERROR_MAX_LESS_THAN_MIN     =   2,
  CWERROR_MAX_TOO_HIGH          =   3,
  CWERROR_DICT_TOO_SMALL        =   4,
  CWERROR_DICT_UNSORTABLE       =   5,
  CWERROR_DICT_UNKNOWN          =   6,
};
typedef unsigned long cwerror_t;

// internal dictionary row
typedef struct dictionary_type {
  bool sorted;
  unsigned long marks;
  unsigned long count;
  unsigned long largest;
  unsigned long largest_pos;
  char** words;
} cwdrow_t;

// dictionary (row container)
typedef struct dictionary_container_type {
  bool sorted;
  unsigned long count;
  cwdrow_t* drows;
  char* name;
} cwdict_t;

#include "seuss.h"
#include "latin.h"

#include "ingredient.h"
#include "generator.h"
#include "tokenize.h"
#include "utility.h"
#include "config.h"
#include "error.h"
#include "dict.h"
#include "args.h"

// external statics
extern cw_t cw_default_type;
extern unsigned long cw_default_min_output;
extern unsigned long cw_default_max_output;

char* chinwag
(cw_t type, unsigned long min, unsigned long max, cwdict_t dict, cwerror_t* e);

char* cw_ltr_rng
(unsigned long min, unsigned long max, cwdict_t dict, cwerror_t* e);

char* cw_wrd_rng
(unsigned long min, unsigned long max, cwdict_t dict, cwerror_t* e);

char* cw_snt_rng
(unsigned long min, unsigned long max, cwdict_t dict, cwerror_t* e);

char* cw_pgf_rng
(unsigned long min, unsigned long max, cwdict_t dict, cwerror_t* e);

char* chinwag_defaults(cwdict_t dict, cwerror_t* e);

#define cw_ltr(amt, dict, err) cw_ltr_rng(amt, amt, dict, err)
#define cw_wrd(amt, dict, err) cw_wrd_rng(amt, amt, dict, err)
#define cw_snt(amt, dict, err) cw_snt_rng(amt, amt, dict, err)
#define cw_pgf(amt, dict, err) cw_pgf_rng(amt, amt, dict, err)

#endif
