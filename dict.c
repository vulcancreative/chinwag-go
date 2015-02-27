#include "dict.h"

// dictionary row utilities
cwdrow_t cwdrow_open();

cwdrow_t cwdrow_add_word
(cwdrow_t drow, const char* word);

cwdrow_t cwdrow_add_word_strict
(cwdrow_t drow, const char* word, U32 size);

cwdrow_t cwdrow_sort
(cwdrow_t drow);

bool cwdrow_word_blank
(cwdrow_t drow, U32 i);

bool cwdrow_word_present
(cwdrow_t drow, U32 i);

bool cwdrow_exclude
(cwdrow_t drow, char const* str);

bool cwdrow_include
(cwdrow_t drow, char const* str);

void cwdrow_close
(cwdrow_t drow);

void puts_cwdrow
(cwdrow_t drow);

// dictionary utilities
I32 cwdict_find_row_of_size
(cwdict_t dict, U32 largest);

bool cwdict_blanks
(cwdict_t dict);

cwdrow_t cwdrow_open()
{
  cwdrow_t d;

  // set default values
  d.sorted = false;
  d.count = 0;
  d.largest = 0;
  d.largest_pos = 0;
  d.words = NULL;

  return d;
}

cwdrow_t cwdrow_add_word
(cwdrow_t drow, const char* word)
{
  // cache strlen of word
  U32 len = (U32)strlen(word);

  if(len > 0)
  {
    // increment word count
    ++drow.count;

    // increase dictionary size
    drow.words = (char**)realloc(drow.words, sizeof(char*) * drow.count);

    // add space for new word at added location; copy word
    drow.words[drow.count - 1] = (char*)malloc(len + 1);
    strcpy(drow.words[drow.count - 1], word);
    drow.words[drow.count - 1][len] = '\0';

    // set new largest (if applicable)
    if(drow.largest < len)
    {
      drow.largest = len;
      drow.largest_pos = drow.count - 1;
    }
  }

  return drow;
}

cwdrow_t cwdrow_add_word_strict
(cwdrow_t drow, const char* word, U32 size)
{
  if(strlen(word) == size) drow = cwdrow_add_word(drow, word);
  return drow;
}

cwdrow_t cwdrow_sort
(cwdrow_t drow)
{
  for(U32 i = 0; i != drow.count - 1; ++i)
  {
    if(cwdrow_word_blank(drow, i) && cwdrow_word_present(drow, i + 1))
    {
      // bubble next one into current one
      U32 len = (U32)strlen(drow.words[i + 1]);

      // fprintf(stdout, "drow.words[i] : %s\n", drow.words[i]);
      if(drow.words[i] != NULL) free(drow.words[i]);
      drow.words[i] = (char*)malloc(len + 1);
      strcpy(drow.words[i], drow.words[i + 1]);
      drow.words[i][len] = '\0';

      // clear next one that we just copied
      free(drow.words[i + 1]);
      drow.words[i + 1] = NULL;
    }
  }

  drow.sorted = true;

  return drow;
}

bool cwdrow_word_blank
(cwdrow_t drow, U32 i)
{
  if(drow.words[i] == NULL || strlen(drow.words[i]) < 1)
  {
    return true;
  }

  return false;
}

bool cwdrow_word_present
(cwdrow_t drow, U32 i)
{
  if(drow.words[i] != NULL && strlen(drow.words[i]) > 0)
  {
    return true;
  }

  return false;
}

bool cwdrow_exclude
(cwdrow_t drow, char const* str)
{
  for(U32 i = 0; i != drow.count; ++i)
  {
    if(strcmp(drow.words[i], str) == 0) return false;
  }

  return true;
}

bool cwdrow_include
(cwdrow_t drow, char const* str)
{
  for(U32 i = 0; i != drow.count; ++i)
  {
    if(strcmp(drow.words[i], str) == 0) return true;
  }

  return false;
}

char* cwdrow_sample
(cwdrow_t drow)
{
  // immediately fail if empty
  if(drow.count == 0) return NULL;

  U32 max = (drow.count == 1 ? 0 : (U32)(drow.count - 1));
  U32 internal = (max == 0 ? 0 : motherr(0, max));

  return drow.words[internal];
}

void cwdrow_close
(cwdrow_t drow)
{
  for(U32 i = 0; i != drow.count; ++i)
  {
    if(drow.words[i]) free(drow.words[i]);
  }

  if(drow.words) free(drow.words);
}

void puts_cwdrow
(cwdrow_t drow)
{
  #ifdef DEBUG
  fprintf(stdout, "(%lu)", drow.count);
  #endif

  fprintf(stdout, "[");

  for(U32 i = 0; i != drow.count; ++i)
  {
    fprintf(stdout, "\"%s\"", drow.words[i]);
    if(i < drow.count - 1) fprintf(stdout, ",");
  }

  fprintf(stdout, "]");
}

I32 cwdict_find_row_of_size
(cwdict_t dict, U32 largest);

cwdict_t cwdict_open()
{
  cwdict_t d;

  // set default values
  d.sorted = false;
  d.count = 0;
  d.drows = NULL;
  d.name = NULL;

  return d;
}

cwdict_t cwdict_open_with_name
(const char* name)
{
  cwdict_t d;

  // set default values
  d.sorted = false;
  d.count = 0;
  d.drows = NULL;
  d.name = NULL;

  d.name = (char*)malloc((strlen(name) + 1) * sizeof(char));
  strcpy(d.name, name);

  return d;
}

cwdict_t cwdict_open_with_tokens
(const char* const buffer, const char* delimiters)
{
  cwdict_t d = tokenize(buffer, delimiters);
  d.name = NULL;

  return d;
}

cwdict_t cwdict_open_with_name_and_tokens
(const char* name, const char* const buffer, const char* delimiters)
{
  cwdict_t d = tokenize(buffer, delimiters);
  d.name = NULL;

  d.name = (char*)malloc((strlen(name) + 1) * sizeof(char));
  strcpy(d.name, name);

  return d;
}

cwdict_t cwdict_add_row
(cwdict_t dict, cwdrow_t drow)
{
  ++dict.count;

  dict.drows = (cwdrow_t*)realloc(dict.drows, sizeof(cwdrow_t) * dict.count);
  dict.drows[dict.count - 1] = drow;

  return dict;
}

cwdict_t cwdict_add_row_strict
(cwdict_t dict, cwdrow_t drow, U32 size)
{
  if(drow.count >= size) dict = cwdict_add_row(dict, drow);
  return dict;
}


cwdict_t cwdict_place_word
(cwdict_t dict, const char* word)
{
  cwdrow_t drow = cwdrow_open();

  drow = cwdrow_add_word(drow, word);
  dict = cwdict_add_row(dict, drow);

  return dict;
}

cwdict_t cwdict_place_words
(cwdict_t dict, const char* const* words, U32 s)
{
  for(U32 i = 0; i != s; ++i)
  {
    dict = cwdict_place_word(dict, words[i]);
  }

  return dict;
}

cwdict_t cwdict_place_word_strict
(cwdict_t dict, const char* word)
{
  bool inserted = false;
  U32 len = (U32)strlen(word);

  for(U32 i = 0; i != dict.count; ++i)
  {
    if(dict.drows[i].largest == len)
    {
      dict.drows[i] = cwdrow_add_word(dict.drows[i], word);
      inserted = true;
    }
  }

  if(inserted == false) dict = cwdict_place_word(dict, word);

  return dict;
}

cwdict_t cwdict_place_words_strict
(cwdict_t dict, const char* const* words, U32 s)
{
  for(U32 i = 0; i != s; ++i)
  {
    dict = cwdict_place_word_strict(dict, words[i]);
  }

  return dict;
}

cwdict_t cwdict_sort
(cwdict_t dict)
{
  cwdrow_t temp;

  // sort individual drows' contents
  for(U32 i = 0; i != dict.count; ++i)
  {
    dict.drows[i] = cwdrow_sort(dict.drows[i]);
  }

  // sort individual drows within dict
  for(U32 i = 0; i != dict.count - 1; ++i)
  {
    for(U32 j = 0; j != dict.count - 1; ++j)
    {
      if(dict.drows[j].largest > dict.drows[j + 1].largest)
      {
        temp = dict.drows[j];
        dict.drows[j] = dict.drows[j + 1];
        dict.drows[j + 1] = temp;
      }
    }
  }

  dict.sorted = true;

  return dict;
}

cwdict_t cwdict_prune
(cwdict_t dict, bool sorted, bool deep)
{
  U32 len = 0, size = 0, null_count = 0;
  char* against = NULL;

  if(!deep)
  {
    for(U32 i = 0; i != dict.count; ++i)
    {
      for(U32 j = 0; j != dict.drows[i].count; ++j)
      {
        if(dict.drows[i].words[j] == NULL) continue;
        against = dict.drows[i].words[j];

        for(U32 k = 0; k != dict.drows[i].count; ++k)
        {
          if(dict.drows[i].words[k] == NULL) continue;
          if(strcmp(against, dict.drows[i].words[k]) == 0 && k != j)
          {
            free(dict.drows[i].words[k]);
            dict.drows[i].words[k] = NULL;
          }
        }
      }
    }
  }
  else
  {
    for(U32 i = 0; i != dict.count; ++i)
    {
      for(U32 j = 0; j != dict.drows[i].count; ++j)
      {
        if(dict.drows[i].words[j] == NULL) continue;
        against = dict.drows[i].words[j];

        for(U32 k = 0; k != dict.count; ++k)
        {
          for(U32 m = 0; m != dict.drows[k].count; ++m)
          {
            if(dict.drows[k].words[m] == NULL) continue;
            if(i == k && dict.drows[k].count == 1) continue;
            if(i == k && j == m) continue;
            if(strcmp(against, dict.drows[k].words[m]) == 0)
            {
              free(dict.drows[k].words[m]);
              dict.drows[k].words[m] = NULL;
              dict.drows[k].marks += 1;
            }
          }
        }
      }
    }

    // reorganize rows, then lop of any empty ones
    bool null_rows = true;
    while(null_rows)
    {
      for(U32 i = 0; i != dict.count; ++i)
      {
        null_rows = false;

        // check if drow needs to be destroyed
        bool marked = false;

        if(dict.drows[i].marks == dict.drows[i].count ||
        dict.drows[i].words == NULL || dict.drows[i].count == 0)
        {
          if(dict.drows[i].marks > 0) marked = true;
        }

        if(marked && i < dict.count - 1)
        {
          dict.drows[i] = dict.drows[i + 1];
          dict.drows[i].marks = 0;

          dict.drows[i + 1] = cwdrow_open();
          dict.drows[i + 1].marks += 1;
        }

        if(marked && i == dict.count - 1)
        {
          dict.count -= 1;
          size = (U32)dict.count * (U32)sizeof(cwdrow_t);

          dict.drows = (cwdrow_t*)realloc(dict.drows, size);
          break;
        }
      }
    }

    for(U32 i = 0; i != dict.count; ++i)
    {
      if(dict.drows[i].marks == dict.drows[i].count ||
      dict.drows[i].words == NULL || dict.drows[i].count == 0) null_rows = true;
    }
  }

  if(sorted) dict = cwdict_sort(dict);

  // resize individual drows within dict
  for(U32 i = 0; i != dict.count; ++i)
  {
    null_count = 0;

    for(U32 j = 0; j != dict.drows[i].count; ++j)
    {
      if(dict.drows[i].words[j] == NULL) ++null_count;
    }

    if(null_count > 0)
    {
      len = (U32)(dict.drows[i].count) - null_count;
      size = sizeof(char*) * len;

      dict.drows[i].words = (char**)realloc(dict.drows[i].words, size);
      dict.drows[i].count = len;
    }
  }

  if(cwdict_blanks(dict)) dict = cwdict_prune(dict, sorted, deep);

  return dict;
}

cwdict_t cwdict_clean
(cwdict_t dict)
{
  cwdict_t new = cwdict_open();
  dict = cwdict_prune(dict, true, false);

  for(U32 i = 0; i != dict.count; ++i)
  {
    for(U32 j = 0; j != dict.drows[i].count; ++j)
    {
      new = cwdict_place_word_strict(new, dict.drows[i].words[j]);
    }
  }

  cwdict_close(dict);
  return new;
}

cwdict_t cwdict_map
(cwdict_t dict, char* (*f)(char*))
{
  U32 len = 0;
  char* temp = (char*)malloc(sizeof(char) * CW_SMALL_BUFFER);

  for(U32 i = 0; i != dict.count; ++i)
  {
    for(U32 j = 0; j != dict.drows[i].count; ++j)
    {
      strcpy(temp, (*f)(dict.drows[i].words[j]));
      len = (U32)strlen(temp);

      // only resize if necessary
      if(len != strlen(dict.drows[i].words[j]))
      {
        dict.drows[i].words[j]=(char*)realloc(dict.drows[i].words[j], len);
      }

      strcpy(dict.drows[i].words[j], temp);
    }
  }

  free(temp);

  return dict;
}

cwdict_t cwdict_clone
(cwdict_t dict)
{
  cwdict_t new;

  if(dict.name) new = cwdict_open_with_name(dict.name);
  else new = cwdict_open();

  for(U32 i = 0; i != dict.count; ++i)
  {
    for(U32 j = 0; j != dict.drows[i].count; ++j)
    {
      new = cwdict_place_word_strict(new, dict.drows[i].words[j]);
    }
  }

  if(dict.name)
  {
    new.name = (char*)malloc(strlen(dict.name) + 1);
    strcpy(new.name, dict.name);
    new.name[strlen(dict.name)] = '\0';
  }

  if(dict.sorted) new = cwdict_sort(new);

  return new;
}

cwdict_t cwdict_dup
(cwdict_t dict)
{
  return cwdict_clone(dict);
}

bool cwdict_exclude
(cwdict_t dict, char const* str)
{
  for(U32 i = 0; i != dict.count; ++i)
  {
    for(U32 j = 0; j != dict.drows[i].count; ++j)
    {
      if(strcmp(dict.drows[i].words[j], str) == 0) return false;
    }
  }

  return true;
}

bool cwdict_include
(cwdict_t dict, char const* str)
{
  for(U32 i = 0; i != dict.count; ++i)
  {
    for(U32 j = 0; j != dict.drows[i].count; ++j)
    {
      if(strcmp(dict.drows[i].words[j], str) == 0) return true;
    }
  }

  return false;
}

bool cwdict_blanks
(cwdict_t dict)
{
  for(U32 i = 0; i != dict.count; ++i)
  {
    for(U32 j = 0; j != dict.drows[i].count; ++j)
    {
      if(cwdrow_word_blank(dict.drows[i], j)) return true;
    }
  }

  return false;
}

bool cwdict_valid
(cwdict_t dict, cwerror_t* error)
{
U32 count = 0;

  for(U32 i = 0; i != dict.count; ++i)
  {
    for(U32 j = 0; j != dict.drows[i].count; ++j)
    {
      // valid if word excludes a space
      if(exclude(dict.drows[i].words[j], " ")) ++count;
    }
  }

  if(count < CW_MIN_DICT_SIZE)
  {
    *error = CWERROR_DICT_TOO_SMALL;
    return false;
  }
  else if(dict.sorted == false)
  {
    *error = CWERROR_DICT_UNSORTABLE;
    return false;
  }

  return true;
}

bool cwdict_equal
(cwdict_t dict, cwdict_t against)
{
  if(dict.count != against.count) return false;

  for(U32 i = 0; i != dict.count; ++i)
  {
    if(dict.drows[i].count != against.drows[i].count) return false;

    for(U32 j = 0; j != dict.drows[i].count; ++j)
    {
      if(strcmp(dict.drows[i].words[j], against.drows[i].words[j]) != 0)
      return false;
    }
  }

  return true;
}

bool cwdict_inequal
(cwdict_t dict, cwdict_t against)
{
  if(dict.count != against.count) return true;

  for(U32 i = 0; i != dict.count; ++i)
  {
    if(dict.drows[i].count != against.drows[i].count) return true;

    for(U32 j = 0; j != dict.drows[i].count; ++j)
    {
      if(strcmp(dict.drows[i].words[j], against.drows[i].words[j]) != 0)
      return true;
    }
  }

  return false;
}

I32 cwdict_find_row_of_size
(cwdict_t dict, U32 largest)
{
  // sort dict if necessary
  if(dict.sorted == false) dict = cwdict_prune(dict, true, false);

  for(U32 i = 0; i != dict.count; ++i)
  {
    if(dict.drows[i].count == largest) return i;
  }

  return -1;
}

U32 cwdict_length
(cwdict_t dict)
{
  U32 total = 0;

  for(U32 i = 0; i != dict.count; ++i) total += dict.drows[i].count;

  return total;
}

U32 cwdict_size
(cwdict_t dict)
{
  return cwdict_length(dict);
}

U32 cwdict_count
(cwdict_t dict)
{
  return cwdict_length(dict);
}

U32 cwdict_largest
(cwdict_t dict)
{
  U32 largest = 0;

  for(U32 i = 0; i != dict.count; ++i)
  {
    if(i == 0)
    {
      largest = (U32)(dict.drows[i].largest);
    }
    else if((U32)(dict.drows[i].largest) > largest)
    {
      largest = (U32)(dict.drows[i].largest);
    }
  }

  return largest;
}

char* cwdict_sample
(cwdict_t dict)
{
  // immediately fail if empty
  if(dict.count == 0) return NULL;

  U32 max = (dict.count == 1 ? 0 : (U32)(dict.count - 1));
  U32 external = (max == 0 ? 0 : motherr(0, max));

  return cwdrow_sample(dict.drows[external]);
}

char* cwdict_join
(cwdict_t dict, char const* delimiter)
{
  U32 len = 0, del_len = (U32)strlen(delimiter);
  char* string = NULL; char* temp = NULL;

  for(U32 i = 0; i != dict.count; ++i)
  {
    for(U32 j = 0; j != dict.drows[i].count; ++j)
    {
      if(dict.drows[i].words[j] == NULL) continue;
      else if(string) len = (U32)strlen(string) +
      (U32)strlen(dict.drows[i].words[j]);
      else len = (U32)strlen(dict.drows[i].words[j]);

      if(i <= dict.count - 1 && j <= dict.drows[i].count - 1)
      { len += del_len; }

      temp = (char*)malloc(len + 1);

      if(string)
      {
        strcpy(temp, string);
        strcat(temp, dict.drows[i].words[j]);
      }
      else strcpy(temp, dict.drows[i].words[j]);

      if(i < dict.count - 1 && j <= dict.drows[i].count - 1)
      { strcat(temp, delimiter); }

      temp[len] = '\0';

      if(string) free(string);
      string = temp; temp = NULL;
    }
  }

  return string;
}

cwdict_t cwdict_close
(cwdict_t dict)
{
  for(U32 i = 0; i != dict.count; ++i) cwdrow_close(dict.drows[i]);
  if(dict.drows) { free(dict.drows); dict.drows = NULL; }

  dict.count = 0;
  if(dict.name) { free(dict.name); dict.name = NULL; }

  return cwdict_open();
}

void puts_cwdict
(cwdict_t dict)
{
  fprintf(stdout, "[");

  for(U32 i = 0; i != dict.count; ++i)
  {
    puts_cwdrow(dict.drows[i]);

    if(i < dict.count - 1) fprintf(stdout, ",");

    #ifdef SPLIT
    if(i < dict.count - 1) fprintf(stdout, "\n");
    #endif
  }

  fprintf(stdout, "]");
}

void validate_cwdict
(cwdict_t dict, char const* function_name)
{
  U32 count = 0;

  for(U32 i = 0; i != dict.count; ++i)
  {
    for(U32 j = 0; j != dict.drows[i].count; ++j)
    {
      // valide if word excludes a space
      if(exclude(dict.drows[i].words[j], " ")) ++count;
    }
  }

  if(count < CW_MIN_DICT_SIZE)
  {
    char e[] =
    "Error in function \"%s\" :\n"
    "  valid dictionary (%d) entry count must be greater than %d\n";
    fprintf(stderr, e, function_name, count, CW_MIN_DICT_SIZE);
    exit(EXIT_FAILURE);
  }

  if(dict.sorted == false)
  {
    char e[] =
    "Error in function \"%s\" :\n"
    "  dictionary is not valid, as it couldn't be properly sorted\n";
    fprintf(stderr, e, function_name);
    exit(EXIT_FAILURE);
  }
}

cwdict_t split_into_cwdict
(const char* buffer, const char* delimiters)
{
  char* tok;
  char* mutable_buffer = (char*)malloc(strlen(buffer) + 1 * sizeof(char));
  cwdict_t dict = cwdict_open();

  // get mutable copy of buffer; a bit slower, but allows for const-ness
  strcpy(mutable_buffer, buffer);
  mutable_buffer[strlen(buffer)] = '\0';

  // natively tokenize input string
  tok = strtok(mutable_buffer, delimiters);
  while(tok != NULL)
  {
    // add word to dict
    dict = cwdict_place_word(dict, tok);

    // get new tok (if any)
    tok = strtok(NULL, delimiters);
  }

  // close mutable buffer
  free(mutable_buffer);

  return dict;
}

#ifdef DICTMAIN
int main(int argc, const char *argv[])
{
  cwdict_t dict = cwdict_open();
  // const char * const words[]= { "the", "quick", "brown", "fox", "jumps",
  // "over", "the", "lazy", "dog", "dawg" };

  dict = cwdict_place_words_strict(dict, argv, argc);
  dict = cwdict_prune(dict, true, false);

  #ifdef DEBUG
  fprintf(stdout, "dict.count : %d\n", dict.count);
  #endif

  puts_cwdict(dict); fprintf(stdout, "\n");

  cwdict_close(dict);

  return 0;
}
#endif
