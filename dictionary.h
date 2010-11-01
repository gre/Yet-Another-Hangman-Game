#ifndef _DICTIONARY_H
#define _DICTIONARY_H

/**
 * Dictionary module : manage the dictionary file and the word picking by level criteria.
 * level is defined by different letters count and the word length.
 */

/**
 * Init the dictionary module
 * @param dic_path : the path to the dictionary text file (one word by line)
 * @return
 *  -1 : if the dictionary file is unable to read
 *  0 : if no word was found
 *  n : if n words was found in the dictionary
 */
extern int dic_init(char* dic_path);

/**
 * Close the dictionary module
 */
extern void dic_close();

/**
 * Get a random suitable word for the requested level.
 * To always have a minimum of random, level is ignored if (nb of words) < MIN_WORD_BY_LEVEL * NB_LEVEL.
 * @param level : value between 0 to NB_LEVEL. Higher is the value, Higher is the difficulty.
 * @return a random word
 */
extern const char* dic_getWord(int level);

#endif
