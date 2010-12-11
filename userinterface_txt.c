/**
 * Yet Another Hangman Game http://github.com/gre/Yet-Another-Hangman-Game
 * @license GNU GPL version 3 http://www.gnu.org/licenses/gpl.html
 * @author Gaetan Renaudeau <pro at grenlibre.fr>
 */
#include <stdlib.h>
#include <stdio.h>

#include "userinterface.h"
#include "pendu.h"

extern void ui_init() {
  printf("Let's begin the game. Good luck !\n");
}
extern void ui_close() {
  printf("Bye!\n");
}
extern void ui_onGameStart(Game* game) {
  printf("\nword: %s\n\n", game_getObfuscatedWord(game));
}
extern void ui_onLetterGived(Game* game, char letter, int status) {
  if(status == -1)
    printf("This is not a letter !\n");
  else if(status==1)
    printf("You have already gived this letter !\n");
  else
    printf("%d try remaining.\nletters: %s\nword: %s\n\n", game_getRemainingTry(game), game_getLetters(game), game_getObfuscatedWord(game));
}
extern void ui_onGameEnd(Game* game, int status) {
  if(status==-1) {
    printf("You lose !\nThe word was : %s\n", game_getWord(game));
  } else if(status==1) {
    printf("You win !\n");
  }
}
extern int ui_getChar(char* ch) {
  printf("Give me a letter: ");
  char letter;
  int ret = scanf("%c", &letter);
  if(ret==1) {
    while(getchar()!='\n'); // clear stdin
    *ch = letter;
    return 1;
  }
  return 0;
}
