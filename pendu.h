#ifndef _PENDU_H
#define _PENDU_H

/**
 * Pendu module : manage the pendu game
 */

/**
 * Init the game module
 */
extern void game_init();

/**
 * Close the game module
 */
extern void game_close();

/**
 * Begin a new pendu game with a new word to guess.
 * @param word : word the user has to find
 */
extern void game_start(const char* word);

/**
 * give a letter to the current game.
 * @param letter : letter to give as new guess
 * @return
 *    -1 : if char gived is not a letter
 *    1 : if letter already gived
 *    0 : if letter is succefully gived
 */
extern int game_giveLetter(char letter);

/**
 * get the current game status
 * @return
 *   -1 : game is finished, player loses
 *    0 : game not finished
 *    1 : game is finished, player wins
 */
extern int game_getStatus();

/**
 * get an obfuscated version of the word to guess in the current game,
 * It is ready to be displayed for the player.
 * secret characters are replaced by a '_'.
 * @return the obfuscated word.
 */
extern char* game_getObfuscatedWord();

/**
 * get the word to guess in the current game.
 * @return the word to guess.
 */
extern const char* game_getWord();

/**
 * get the remaining letters guess in the current game.
 * @return remaining letters guess
 */
extern int game_getRemainingTry();

/**
 * get all letters gived by the player.
 * @return all gived letters
 */
extern char* game_getLetters();

#endif
