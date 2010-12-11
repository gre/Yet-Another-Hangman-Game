/**
 * Yet Another Hangman Game http://github.com/gre/Yet-Another-Hangman-Game
 * @license GNU GPL version 3 http://www.gnu.org/licenses/gpl.html
 * @author Gaetan Renaudeau <pro at grenlibre.fr>
 */
#ifndef _PENDU_H
#define _PENDU_H

/**
 * Pendu module : manage the pendu game
 */

/**
 * Game is the type which contains all states of a running game
 * all functions of this module take an instance of Game
 */
typedef struct _Game Game;

/**
 * Init a game
 * @return an instance of Game
 */
extern Game* game_init();

/**
 * Close a game
 * @param game : instance of Game
 */
extern void game_close(Game* game);

/**
 * Begin a new pendu game with a new word to guess.
 * @param word : word the user has to find
 * @param game : instance of Game
 */
extern void game_start(Game* game, const char* word);

/**
 * give a letter to the current game.
 * @param letter : letter to give as new guess
 * @param game : instance of Game
 * @return
 *    -1 : if char gived is not a letter
 *    1 : if letter already gived
 *    0 : if letter is succefully gived
 */
extern int game_giveLetter(Game* game, char letter);

/**
 * get the current game status
 * @param game : instance of Game
 * @return
 *   -1 : game is finished, player loses
 *    0 : game not finished
 *    1 : game is finished, player wins
 */
extern int game_getStatus(Game* game);

/**
 * get an obfuscated version of the word to guess in the current game,
 * It is ready to be displayed for the player.
 * secret characters are replaced by a '_'.
 * @param game : instance of Game
 * @return the obfuscated word.
 */
extern char* game_getObfuscatedWord(Game* game);

/**
 * get the word to guess in the current game.
 * @param game : instance of Game
 * @return the word to guess.
 */
extern const char* game_getWord(Game* game);

/**
 * get the remaining letters guess in the current game.
 * @param game : instance of Game
 * @return remaining letters guess
 */
extern int game_getRemainingTry(Game* game);

/**
 * get all letters gived by the player.
 * @param game : instance of Game
 * @return all gived letters
 */
extern char* game_getLetters(Game* game);

#endif
