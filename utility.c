#include "utility.h"

bool exclude(char const* str, char const* substr)
{
  if(strstr(str, substr) == NULL) return true;

  return false;
}

bool exclude_num(U32* nums, U32 size, U32 subnum)
{
  for(U32 i = 0; i != size; ++i)
  {
    if(nums[i] == subnum) return false;
  }

  return true;
}

bool include(char const* str, char const* substr)
{
  if(strstr(str, substr) != NULL) return true;

  return false;
}

bool include_mtx(char const* str,const char* const* substr,const int size)
{
  for(int i = 0; i != size; ++i)
  {
    if(strstr(str, substr[i]) != NULL) return true;
  }

  return false;
}

U32 count(char* string, char const* delimiters)
{
  char* tok = NULL;
  U32 count = 0;

  tok = strtok(string, delimiters);
  while(tok != NULL)
  {
    ++count;
    tok = strtok(NULL, delimiters);
  }

  return count;
}

char* substring_with_size(const char* string, U32 start, U32 end)
{
  char* result = NULL;
  U32 len = (U32)strlen(string), size = end - start;

  if(len == 0 || start > end || start >= len || end >= len) return result;

  if(start == 0)
  {
    result = (char*)malloc(2);
    result[0] = string[start];
    result[1] = '\0';
  }
  else
  {
    result = (char*)malloc(size + 1);
    result = memcpy(result, &string[start], size);
    result[size] = '\0';
  }

  return result;
}

char* sample_substring_with_size(const char* string, U32 size)
{
  char* result = NULL;
  U32 len = (U32)strlen(string), access = 0, difference = 0;

  if(len == 0 || len < size || size == 0) return result;

  if(len == size)
  {
    result = (char*)malloc(size + 1);
    result = strcpy(result, string);
    result[len] = '\0';
  }
  else
  {
    access = motherr(0, len - 1);

    difference = (len - 1) - access;
    while(difference < size) { --access; difference = (len - 1) - access; }

    result = (char*)malloc(size + 1);
    result = memcpy(result, &string[access], size);
    result[size] = '\0';
  }

  return result;
}

char* add_suffix(char* string, char* suffix)
{
  U32 len = (U32)strlen(string) + (U32)strlen(suffix);
  char* temp = (char*)malloc(len + 1);

  strcpy(temp, string);
  strcat(temp, suffix);
  temp[len] = '\0';

  free(string);
  string = temp; temp = NULL;

  return string;
}

char* upcase(char* word)
{
  for(U32 i = 0; i != strlen(word); ++i)
  {
    word[i] = (char)toupper(word[i]);
  }

  return word;
}

char* downcase(char* word)
{
  for(U32 i = 0; i != strlen(word); ++i)
  {
    word[i] = (char)tolower(word[i]);
  }

  return word;
}

char* capitalize(char* word)
{
  word[0] = toupper(word[0]);

  return word;
}
