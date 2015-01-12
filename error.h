#ifndef __ERROR_PEN15_H
#define __ERROR_PEN15_H

#include "chinwag.h"

char* cwerror_string(cwdict_t dict, cwerror_t code);
void cwerror_warn(cwdict_t dict, cwerror_t code);
void cwerror_fail(cwdict_t dict, cwerror_t code);

#endif
