#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "dictionary.h"
#include "globals.h"

#define DIC_ALLOC_WINDOW 100
#define LINE_MAX_LENGTH 50
#define MIN_WORD_BY_LEVEL 5

typedef struct _Word {
  char* str;
} Word;

typedef struct _Dictionary {
  int length;
  Word* words;
} Dictionary;

static Dictionary dic;

static int comparWords(const void* a, const void* b) {
  char * word_a = ((Word*) a) -> str;
  char * word_b = ((Word*) b) -> str;
  char c;
  int diff, count_a = 0, count_b = 0;
  /* compar different letters number
   * if same count of letters : compar length
   */
  for(c='A'; c<='Z'; ++c) {
    if(util_letterInWord(c, word_a)) ++count_a;
    if(util_letterInWord(c, word_b)) ++count_b;
  }
  diff = count_a - count_b;
  return diff ? diff : strlen(word_a) - strlen(word_b);
}

extern int dic_init(char* dic_path) {
  char word[LINE_MAX_LENGTH];
  char* wordTrim;
  char* wordAllocate;
  int wordLen;
  dic.length=0;
  FILE* dicFile = fopen(dic_path, "r");
  if(!dicFile) return -1;
  Word* words = calloc(DIC_ALLOC_WINDOW, sizeof(Word));
  while(fgets(word, LINE_MAX_LENGTH, dicFile)) {
    wordTrim = str_trim(word);
    wordLen = strlen(wordTrim);
    if(wordLen>0) {
      // register word if not empty
      wordAllocate = malloc(wordLen+1);
      strcpy(wordAllocate, wordTrim);
      str_toupper(wordAllocate);
      words[dic.length].str = wordAllocate;
      dic.length++;
    }
    if(dic.length % DIC_ALLOC_WINDOW == 0) {
      words = realloc(words, (dic.length+DIC_ALLOC_WINDOW)*sizeof(Word));
    }
  }
  fclose(dicFile);
  if(dic.length==0) return 0;
  // sort by difficulty (different letters count and string size)
  qsort(words, dic.length, sizeof(words[0]), comparWords);
  dic.words = words;
  return dic.length;
}

extern void dic_close() {
  int i;
  for(i=0; i<dic.length; ++i)
    free(dic.words[i].str);
  free(dic.words);
}

/**
 * level : 0 to 9
 */
extern const char* dic_getWord(int level) {
  if(level<0) level = 0;
  else if(level>=NB_LEVEL) level = NB_LEVEL-1;
  int from, to, i;
  if(dic.length < MIN_WORD_BY_LEVEL*NB_LEVEL) {
    // not enough words to use level randomisation (min MIN_WORD_BY_LEVEL word by level)
    i = rand() % dic.length;
  }
  else {
    // spliting words into 10 levels
    from = level * dic.length / NB_LEVEL;
    to = (level+1) * dic.length / NB_LEVEL;
    i = (from == to) ? from : from + rand() % (to-from);
  }
  return dic.words[i].str;
}