#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "utils.h"
#include "pendu.h"

typedef struct _Game {
  int maxTry;
  int nbTry;
  char letters[26];
  const char* word;
  char* obfuscated;
} Game;

static Game game;

extern void game_init() {
  srand(time(NULL));
  game.obfuscated = 0;
}

extern void game_close() {
  if(game.obfuscated)
    free(game.obfuscated);
}

extern void game_start(const char* word) {
  int i, word_len = strlen(word);
  if(game.obfuscated) free(game.obfuscated);
  game.obfuscated = calloc(word_len+1, sizeof(char));
  for(i=0; i<word_len; ++i)
    game.obfuscated[i] = isalpha(word[i]) ? '_' : word[i];
  game.obfuscated[i] = 0;
  game.word = word;
  game.maxTry = 10;
  game.nbTry = 0;
  bzero(game.letters, 26*sizeof(char));
}

extern int game_getRemainingTry() {
  return game.maxTry - game.nbTry;
}

extern int game_getStatus() {
  if(strcmp(game.obfuscated, game.word)==0) return 1; // game finished, player has found the word
  if(game.maxTry == game.nbTry) return -1; // game finished, nbTry reach maxTry
  return 0; // game unfinished
}

extern int game_giveLetter(char letter) {
  int letterFound, word_len = strlen(game.word);
  if(!isalpha(letter))
    return -1;
  letter = toupper(letter);
  if(util_letterInWord(letter, game.letters))
    return 1;
  int i = strlen(game.letters);
  game.letters[i] = letter;
  
  letterFound = 0;
  for(i=0; i<word_len; ++i) {
    if(letter == game.word[i]) {
      game.obfuscated[i] = letter;
      ++ letterFound;
    }
  }
  if(!letterFound)
    ++ game.nbTry;
  
  return 0;
}

extern char* game_getObfuscatedWord() {
  return game.obfuscated;
}
extern const char* game_getWord() {
  return game.word;
}
extern char* game_getLetters() {
  return game.letters;
}