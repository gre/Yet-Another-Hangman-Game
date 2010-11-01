#ifndef _UTILS_H
#define _UTILS_H

/// String utils ///

/**
 * Check if word contains letter.
 * @return
 *  1: if letter is in word,
 *  0: else
 */
extern int util_letterInWord(char letter, char* word);

/**
 * Uppercase a string
 * @param s : the string to uppercase
 */
extern void str_toupper(char *s);

/**
 * Trim a string (remove extra spaces around words)
 * @return the trimed string
 */
extern char* str_trim(char *s);


/// Args utils ///

/**
 * 
 */
extern int util_containsArg(int argc, char * argv[], char* key);

/**
 *
 */
extern int util_getArgValue(int argc, char * argv[], char * key, int * value);

/**
 *
 */
extern int util_getArgString(int argc, char * argv[], char * key, char** value);

#endif