#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>

#include "userinterface.h"
#include "pendu.h"

#define STRBUF_SIZE 64

#define PADDING_LEFT 2
#define TITLE_TOP 0
#define PENDU_TOP 2
#define PENDU_HEIGHT 9
#define WORD_TOP (PENDU_TOP+PENDU_HEIGHT)
#define MESSAGE_TOP (WORD_TOP+1)

enum { MYCOLOR_DEFAULT = 1, MYCOLOR_TITLE, MYCOLOR_PENDU, MYCOLOR_PENDU_WARN, MYCOLOR_PENDU_LOST, MYCOLOR_LETTERS, MYCOLOR_INFO, MYCOLOR_ERROR };

/// GUI globals ///
static WINDOW* mainwin; // fenetre principale
static int mainwin_width;  // taille horizontale de la fenetre principale (char)
static int mainwin_height; // taille verticale de la fenetre principale (char)

static char strbuf[STRBUF_SIZE];

static void setColor(int color) {
  wattron(mainwin, COLOR_PAIR(color));
}
static void cleanLine(int line) {
  mvwhline(mainwin, line, 0, ' ', mainwin_width);
}
static void drawTitle() {
  wattrset(mainwin, A_BOLD);
  setColor(MYCOLOR_TITLE);
  mvwprintw(mainwin, TITLE_TOP, (mainwin_width-15)/2, "Le jeu du Pendu");
  wattrset(mainwin, A_NORMAL);
}
static void drawPendu(int remainingTry) {
  if(remainingTry>3)
    setColor(MYCOLOR_PENDU);
  else if(remainingTry==0)
    setColor(MYCOLOR_PENDU_LOST);
  else
    setColor(MYCOLOR_PENDU_WARN);
  int y = PENDU_TOP, x = (mainwin_width-8) / 2;
  mvwprintw(mainwin, y++, x, (remainingTry<7 ? "+------ " : "        ") );
  mvwprintw(mainwin, y++, x, (remainingTry<6 ? "|/  |   " : (remainingTry<8 ? "|/      " : (remainingTry<9 ? "|       " : "        "))) );
  mvwprintw(mainwin, y++, x, (remainingTry<5 ? "|   o   " : (remainingTry<9 ? "|       " : "        ")) );
  mvwprintw(mainwin, y++, x, (remainingTry<4 ? "|  -+-  " : (remainingTry<9 ? "|       " : "        ")) );
  mvwprintw(mainwin, y++, x, (remainingTry<3 ? "|   ^   " : (remainingTry<9 ? "|       " : "        ")) );
  mvwprintw(mainwin, y++, x, (remainingTry<1 ? "|  / \\  ": (remainingTry<2 ? "|  /    " : (remainingTry<9 ? "|       " : "        "))) );
  mvwprintw(mainwin, y++, x, (remainingTry<9 ? "|       " : "        ") );
  mvwprintw(mainwin, y++, x, (remainingTry<10 ? "========" : "        ") );
}
static void drawWord(char* word) {
  wattrset(mainwin, A_BOLD);
  setColor(MYCOLOR_DEFAULT);
  mvwprintw(mainwin, WORD_TOP, PADDING_LEFT, word);
  wattrset(mainwin, A_NORMAL);
}
static void drawLetters(char* letters) {
  setColor(MYCOLOR_LETTERS);
  int len = strlen(letters);
  mvwprintw(mainwin, WORD_TOP, mainwin_width-len-2, letters);
}
static void drawMessageAtLine(char* message, int line) {
  cleanLine(line);
  mvwprintw(mainwin, line, (mainwin_width-strlen(message))/2, message);
}
static void drawMessage(char* message) {
  drawMessageAtLine(message, MESSAGE_TOP);
}
static void drawInfoMessage(char* message) {
  setColor(MYCOLOR_INFO);
  drawMessage(message);
}
static void drawErrorMessage(char* message) {
  setColor(MYCOLOR_ERROR);
  drawMessage(message);
}

// Curses init
extern void ui_init(Game* game) {
  mainwin = initscr();
  mainwin_width = COLS;
  mainwin_height = LINES;
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(MYCOLOR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
  init_pair(MYCOLOR_TITLE, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(MYCOLOR_PENDU, COLOR_GREEN, COLOR_BLACK);
  init_pair(MYCOLOR_PENDU_WARN, COLOR_YELLOW, COLOR_BLACK);
  init_pair(MYCOLOR_PENDU_LOST, COLOR_RED, COLOR_BLACK);
  init_pair(MYCOLOR_LETTERS, COLOR_CYAN, COLOR_BLACK);
  init_pair(MYCOLOR_INFO, COLOR_YELLOW, COLOR_BLACK);
  init_pair(MYCOLOR_ERROR, COLOR_RED, COLOR_BLACK);
  refresh();
}
extern void ui_close() {
  endwin();
}
extern void ui_onGameStart(Game* game) {
  drawTitle();
  drawPendu(game_getRemainingTry(game));
  drawWord(game_getObfuscatedWord(game));
  drawLetters(game_getLetters(game));
  drawInfoMessage("");
  refresh();
}
extern void ui_onLetterGived(Game* game, char letter, int status) {
  int remainingTry;
  if(status == -1)
    drawErrorMessage("This is not a letter !");
  else if(status==1)
    drawErrorMessage("You have already gived this letter !");
  else {
    remainingTry = game_getRemainingTry(game);
    drawPendu(remainingTry);
    drawWord(game_getObfuscatedWord(game));
    drawLetters(game_getLetters(game));
    snprintf(strbuf, STRBUF_SIZE, "%d try remaining.", remainingTry);
    drawInfoMessage(strbuf);
  }
  refresh();
}
extern void ui_onGameEnd(Game* game, int status) {
  if(status==-1)
    drawInfoMessage("You lose !");
  else if(status==1)
    drawInfoMessage("You win !");
  setColor(MYCOLOR_DEFAULT);
  drawMessageAtLine("Press any key to quit.", MESSAGE_TOP+2);
  refresh();
  getch();
}
extern int ui_getChar(char* ch) {
  char keycode = getch();
  if(keycode == ERR) return 0;
  *ch = keycode;
  return 1;
}
