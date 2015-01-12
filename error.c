#include "error.h"

char* cwerror_string(cwdict_t dict, cwerror_t code)
{
  char* result = (char*)malloc(CW_SMALL_BUFFER);

  if(code == CWERROR_INVALID_OUTPUT_TYPE)
  {
    sprintf(result, "CWERROR_INVALID_OUTPUT_TYPE : ");
    sprintf(result, "requested generation type invalid");
  }
  else if(code == CWERROR_MIN_LESS_THAN_ONE)
{
  sprintf(result, "CWERROR_MAX_LESS_THAN_ONE : ");
  sprintf(result, "min threshold must be at least one");
}
  else if(code == CWERROR_MAX_LESS_THAN_MIN)
  {
    sprintf(result, "CWERROR_MAX_LESS_THAN_MIN : ");
    sprintf(result, "max threshold must be greater than min");
  }
  else if(code == CWERROR_MAX_TOO_HIGH)
  {
    sprintf(result, "CWERROR_MAX_TOO_HIGH : ");
    sprintf(result, "max cannot be in excess of 10000");
  }
  else if(code == CWERROR_DICT_TOO_SMALL)
  {
    sprintf(result, "CWERROR_DICT_TOO_SMALL : ");
    if(dict.name != NULL && strlen(dict.name) > 0)
    {
      sprintf(result, "dict \"%s\" has too few acceptable entries (%d of %d)",
      dict.name, cwdict_length(dict), CW_MIN_DICT_SIZE);
    }
    else
    {
      sprintf(result, "dict has too few acceptable entries (%d of %d)",
      cwdict_length(dict), CW_MIN_DICT_SIZE);
    }
  }
  else if(code == CWERROR_DICT_UNSORTABLE)
  {
    sprintf(result, "CWERROR_DICT_UNSORTABLE : ");
    if(dict.name != NULL && strlen(dict.name) > 0)
    {
      sprintf(result, "unable to sort dict \"%s\"", dict.name);
    }
    else sprintf(result, "unable to sort dict");
  }
  else
  {
    sprintf(result, "CWERROR_DICT_UNKNOWN : ");
    if(dict.name != NULL && strlen(dict.name) > 0)
    {
      sprintf(result, "something weird happened while validating dict \"%s\"",
      dict.name);
    }
    else sprintf(result, "something weird happened while validating dict");
  }

  return result;
}

void cwerror_warn(cwdict_t dict, cwerror_t code)
{
  char* message = cwerror_string(dict, code);

  fprintf(stderr, "%s\n", message);

  free(message);
}

void cwerror_fail(cwdict_t dict, cwerror_t code)
{
  cwerror_warn(dict, code);
  exit(EXIT_FAILURE);
}
