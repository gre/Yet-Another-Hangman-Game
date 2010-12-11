/**
 * Yet Another Hangman Game http://github.com/gre/Yet-Another-Hangman-Game
 * @license GNU GPL version 3 http://www.gnu.org/licenses/gpl.html
 * @author Gaetan Renaudeau <pro at grenlibre.fr>
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "globals.h"
#include "utils.h"
#include "pendu.h"
#include "dictionary.h"
#include "userinterface.h"

#define DIC_PATH_DEFAULT "dictionaries/fr.txt"

static void printHelp(char** argv);

/**
 * read args and running one game with console display
 */
int main(int argc, char * argv[]) {
  int status;
  int level, levelFallback;
  char * dic_path;
  char letter;
  
  /// Consume commandline args
  
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
  printf("Difficulty is set to %d.\n", level);
  
  if(util_getArgString(argc, argv, "--dictionary", &dic_path) != 0
  && util_getArgString(argc, argv, "-d",           &dic_path) != 0)
      dic_path = DIC_PATH_DEFAULT;
  
  /// Init modules
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
  
  Game* game = game_init();
  ui_init();
  
  // Start a new game
  game_start(game, dic_getWord(level));
  ui_onGameStart(game);
  do {
    if(ui_getChar(&letter)) {
      status = game_giveLetter(game, letter);
      ui_onLetterGived(game, letter, status);
    }
  } while((status=game_getStatus(game))==0);
  ui_onGameEnd(game, status);
  
  // Close modules
  ui_close();
  game_close(game);
  dic_close();
  
  return 0;
}

static void printHelp(char** argv) {
  printf("Usage: %s [--dic <path>] [--level <level>]\n", argv[0]);
  printf("\n");
  printf("Options:\n");
  printf("  -l, --level\t\tspecific the level (value between 0 and %d)\n", NB_LEVEL-1);
  printf("  -d, --dictionary\tprovide the dictionary file path\n");
  printf("  -h, --help\t\tdisplay this help\n");
  printf("\n");
}
