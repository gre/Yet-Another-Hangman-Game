#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "globals.h"
#include "utils.h"
#include "pendu.h"
#include "dictionary.h"

#define DIC_PATH_DEFAULT "dictionaries/fr.txt"

static void printHelp(char** argv);
static void startConsoleGame(Game* game);

/**
 * read args and running one game with console display
 */
int main(int argc, char * argv[]) {
  int status;
  int level, levelFallback;
  char * dic_path;
  
  /// consume all args ///
  
  // check if --help is requested
  if(util_containsArg(argc, argv, "--help")
  || util_containsArg(argc, argv, "-h")
  || util_containsArg(argc, argv, "help")) {
    printHelp(argv);
    return 0;
  }
  if(util_getArgValue(argc, argv, "--level", &level) != 0
  && util_getArgValue(argc, argv, "-l",      &level) != 0) {
    level = 5;
  }
  else {
    if(level<0 || level>NB_LEVEL-1) {
      levelFallback = level<0 ? 0 : NB_LEVEL-1;
      printf("WARNING: level must be between 0 and %d. You give %d, fallback on level %d.\n", NB_LEVEL-1, level, levelFallback);
      level = levelFallback;
    }
  }
  if(util_getArgString(argc, argv, "--dictionary", &dic_path) != 0
  && util_getArgString(argc, argv, "-d",           &dic_path) != 0)
      dic_path = DIC_PATH_DEFAULT;
  
  /// Init dictionary
  printf("Loading dictionary %s... ", dic_path);
  fflush(stdout);
  status = dic_init(dic_path);
  if(status == -1) {
    fprintf(stderr, "Unable to load the dictionary\n");
    return -1;
  }
  if(status == 0) {
    fprintf(stderr, "No word was found.\n");
    return -2;
  }
  printf("loaded.\n");
  printf("Difficulty is set to %d.\n", level);
  printf("Let's begin the game. Good luck !\n");
  
  Game* game = game_init();
  game_start(game, dic_getWord(level));
  startConsoleGame(game);
  game_close(game);
  dic_close();
  
  return 0;
}

void printHelp(char** argv) {
  printf("Usage: %s [--dic <path>] [--level <level>]\n", argv[0]);
  printf("\n");
  printf("Options:\n");
  printf("  -l, --level\t\tspecific the level (value between 0 and %d)\n", NB_LEVEL-1);
  printf("  -d, --dictionary\t\tprovide the dictionary file path\n");
  printf("  -h, --help\t\tdisplay this help\n");
  printf("\n");
}

void startConsoleGame(Game* game) {
  int status;
  char letter;
  printf("\nword: %s\n\n", game_getObfuscatedWord(game));
  do {
    printf("Give me a letter: ");
    if(scanf("%c", &letter)==1) {
      while(getchar()!='\n'); // clear stdin
      status = game_giveLetter(game, letter);
      if(status == -1)
        printf("This is not a letter !\n");
      else if(status==1)
        printf("You have already gived this letter !\n");
      else
        printf("%d try remaining.\nletters: %s\nword: %s\n\n", game_getRemainingTry(game), game_getLetters(game), game_getObfuscatedWord(game));
    }
  } while((status=game_getStatus(game))==0);
  if(status==-1) {
    printf("You lose !\nThe word was : %s\n", game_getWord(game));
  } else if(status==1) {
    printf("You win !\n");
  }
}