#ifndef _USERINTERFACE_H
#define _USERINTERFACE_H

#include "pendu.h"

/**
 * User Interface module : manage the UI of the game
 */

/**
 * Init the UI
 * @param game : a Game instance
 */
extern void ui_init();

/**
 * Close the UI
 */
extern void ui_close();

/**
 * Called on game start (just after game init)
 * @param game : a Game instance
 */
extern void ui_onGameStart(Game* game);

/**
 * Read an character from UI and store it into ch
 * @param ch : read value is store into ch
 * @return 1 if a char has been read, 0 else
 */
extern int ui_getChar(char* ch);

/**
 * Called after a letter has been given
 * @param game : a Game instance
 */
extern void ui_onLetterGived(Game* game, char letter, int status);

/**
 * Called on game end (just before game close)
 * @param game : a Game instance
 */
extern void ui_onGameEnd(Game* game, int status);

#endif