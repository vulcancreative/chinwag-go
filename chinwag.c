#include "chinwag.h"

cw_t cw_default_type = CW_WORDS;
unsigned long cw_default_min_output = 1;
unsigned long cw_default_max_output = 5;

char* chinwag
(cw_t type, unsigned long min, unsigned long max, cwdict_t dict, cwerror_t* e)
{
  if(min == 0 || max == 0)
  {
    if(e) *e = CWERROR_MIN_LESS_THAN_ONE;
    return NULL;
  }

  if(max < min)
  {
    if(e) *e = CWERROR_MAX_LESS_THAN_MIN;
    return NULL;
  }

  if(type >= 4)
  {
    if(e) *e = CWERROR_INVALID_OUTPUT_TYPE;
    return NULL;
  }

  char* result = NULL;

  if(type == CW_LETTERS) result = cw_ltr_rng(min, max, dict, NULL);
  else if(type == CW_WORDS) result = cw_wrd_rng(min, max, dict, NULL);
  else if(type == CW_SENTENCES) result = cw_snt_rng(min, max, dict, NULL);
  else if(type == CW_PARAGRAPHS) result = cw_pgf_rng(min, max, dict, NULL);

  return result;
}

char* cw_ltr_rng
(unsigned long min, unsigned long max, cwdict_t dict, cwerror_t* e)
{
  if(min == 0 || max == 0)
  {
    if(e) *e = CWERROR_MIN_LESS_THAN_ONE;
    return NULL;
  }

  if(max < min)
  {
    if(e) *e = CWERROR_MAX_LESS_THAN_MIN;
    return NULL;
  }


  cwdict_t temp = cwdict_open();
  I32 amount = motherr((U32)min, (U32)max), total = 0; U32 len = 0;
  char* s = (char*)malloc(CW_SMALL_BUFFER); char* sample = NULL;
  char* result = NULL; char* vowels = "aeiou";

  while(amount > 0)
  {
    if(amount == 2)
    {
      // SSWS : modifies destination, can't modify source, new string
      sample = sample_substring_with_size(vowels, 1);
      s = strcpy(s, sample); free(sample);
      len = 1; total += len;
      s[len] = '\0';
    }
    else
    {
      s = strcpy(s, cwdict_sample(dict));
      len = (U32)strlen(s); total += len; s[len] = '\0';
      if(len > amount || include(s, " ") || include(s, "-")) continue;
    }

    amount -= len;

    // postfixed alteration (append vowel chain/remove trailing character)
    if(amount + 1 == 0)
    {
      // SWS : modifies destination, can't modify source, new string
      sample = substring_with_size(s, 0, len - 2);
      s = strcpy(s, sample); free(sample);
      len -= 1; total -= 1; s[len] = '\0';

      amount += 1;
    }
    else if(amount - 1 == 0)
    {
      // SSWS : modifies destination, can't modify source, new string
      sample = sample_substring_with_size(vowels, 1);
      s = add_suffix(s, sample); free(sample);
      len += 1; total += 1; s[len] = '\0';

      amount -= 1;
    }
    else if(amount - 2 == 0)
    {
      // SSWS : modifies destination, can't modify source, new string
      sample = sample_substring_with_size(vowels, 2);
      s = add_suffix(s, sample); free(sample);
      len += 2; total += 2; s[len] = '\0';

      amount -= 2;
    }

    temp = cwdict_place_word(temp, s);
    if(amount > 0 && amount != 1) --amount;
  }

  // post-process dict (pass utility::capitalize function as parameter)
  temp = cwdict_prune(temp, false, false);
  temp = cwdict_map(temp, capitalize);
  result = cwdict_join(temp, " ");

  cwdict_close(temp);
  free(s); s = NULL;

  return result;
}

char* cw_wrd_rng
(unsigned long min, unsigned long max, cwdict_t dict, cwerror_t* e)
{
  if(min == 0 || max == 0)
  {
    if(e) *e = CWERROR_MIN_LESS_THAN_ONE;
    return NULL;
  }

  if(max < min)
  {
    if(e) *e = CWERROR_MAX_LESS_THAN_MIN;
    return NULL;
  }

  cwdict_t temp = cwdict_open();
  U32 amount = motherr((U32)min, (U32)max), total = cwdict_length(dict);
  char* sample = NULL; char* result = NULL;
  bool invalid = true;

  // add words to dict
  for(U32 i = 0; i != amount; ++i)
  {
    while(invalid)
    {
      sample = cwdict_sample(dict);

      // valid if no space, hyphen, or duplicate (latter depends on size)
      if(exclude(sample, " ") && exclude(sample, "-"))
      {
        if(amount > total) invalid = false;
        else if(cwdict_exclude(temp, sample)) invalid = false;
      }
    }

    temp = cwdict_place_word(temp, sample);
    invalid = true;
  }

  // post-process dict (pass utility::capitalize function as parameter)
  temp = cwdict_map(temp, capitalize);
  result = cwdict_join(temp, " ");

  cwdict_close(temp);

  return result;
}

char* cw_snt_rng
(unsigned long min, unsigned long max, cwdict_t dict, cwerror_t* e)
{
  if(min == 0 || max == 0)
  {
    if(e) *e = CWERROR_MIN_LESS_THAN_ONE;
    return NULL;
  }

  if(max < min)
  {
    if(e) *e = CWERROR_MAX_LESS_THAN_MIN;
    return NULL;
  }

  cwdict_t master = cwdict_open(), temp; cwdrow_t selected;
  U32 word_amount = 0, last = 0, amount = motherr((U32)min, (U32)max), now = 0,
  len = 0, t_minus = 0; U8 comma = 0; I32 punct = 0;
  U32* no_dice = (U32*)malloc(sizeof(U32) * CW_SMALL_BUFFER);
  char* sample = NULL; char* result = NULL; char* s = NULL;
  bool invalid = true;

  for(U32 i = 0; i != amount; ++i)
  {
    temp = cwdict_open();
    word_amount = motherr(CW_SENTENCE_MIN_WORD, CW_SENTENCE_MAX_WORD);

    if(word_amount >= 2) comma = (U8)motherr(0, 1);

    // if comma, determine commma position after first word)
    if(word_amount >= 2 && comma == 1) comma = motherr(1, word_amount - 1);

    // determine sentence rhythm
    for(U32 j = 0; j != word_amount; ++j)
    {
      if(j == 0) now = motherr(5, 10);
      else if(j == word_amount - 1) now = motherr(3, 8);
      else if(t_minus > 0) { now = motherr(1, 10); --t_minus; }
      else if(last <= 10) now = motherr(1, (U32)(dict.count - 1));
      else if(last > 10 || last <= 2) { now = motherr(6, 10); t_minus = 3; }

      selected = dict.drows[now];
      sample = cwdrow_sample(selected);

      while(cwdict_include(temp, sample) && strlen(sample) != now)
      { sample = cwdict_sample(dict); }

      // add comma (if applicable)
      if(comma && j == comma - 1)
      {
        // get local copy of sample for modification
        len = (U32)strlen(sample);
        s = (char*)malloc(strlen(sample) + 1);

        strcpy(s, sample);
        s[len] = '\0';

        s = add_suffix(s, ",");
        temp = cwdict_place_word(temp, s);

        free(s);
      }
      else temp = cwdict_place_word(temp, sample);

      invalid = true;
      last = now;
    }

    // join temporary dict into a sentence; capitalize first word
    s = cwdict_join(temp, " ");
    s = capitalize(s);

    // determine punctuation; 1 - period, 2 - question, 3 - exclamation
    // based on a ratio of 64-21-15, sampled from Shakespeare's Hamlet
    punct = (I32)motherr(0, 99);

    if(punct >= 0 && punct <= 63) s = add_suffix(s, ".");
    else if(punct >= 64 && punct <= 84) s = add_suffix(s, "?");
    else if(punct >= 85 && punct <= 99) s = add_suffix(s, "!");

    // add sentence to master dict and cleanup
    master = cwdict_place_word(master, s);

    cwdict_close(temp);
    free(s);
  }

  result = cwdict_join(master, " ");
  cwdict_close(master);
  free(no_dice);

  return result;
}

char* cw_pgf_rng
(unsigned long min, unsigned long max, cwdict_t dict, cwerror_t* e)
{
  if(min == 0 || max == 0)
  {
    if(e) *e = CWERROR_MIN_LESS_THAN_ONE;
    return NULL;
  }

  if(max < min)
  {
    if(e) *e = CWERROR_MAX_LESS_THAN_MIN;
    return NULL;
  }

  char* result = NULL; char* sentences = NULL;
  U32 amount = motherr((U32)min, (U32)max), sentence_amount = 0;
  cwdict_t master = cwdict_open();

  for(U32 i = 0; i != amount; ++i)
  {
    sentence_amount = motherr(CW_PARAGRAPH_MIN_SENTENCE,
    CW_PARAGRAPH_MAX_SENTENCE);

    sentences = cw_snt(sentence_amount, dict, NULL);
    master = cwdict_place_word(master, sentences);

    free(sentences);
  }

  result = cwdict_join(master, "\n\n");
  cwdict_close(master);

  return result;
}

char* chinwag_defaults(cwdict_t dict, cwerror_t* e)
{
  return chinwag(cw_default_type, cw_default_min_output, cw_default_max_output,
  dict, e);
}
