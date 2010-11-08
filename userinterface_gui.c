#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>

#include "userinterface.h"
#include "pendu.h"

#define STRBUF_SIZE 64


/// GUI globals ///

#define MAINWIN_HEIGHT 16
#define MAINWIN_WIDTH 50

#define TITLE_TOP 1
#define PENDU_LEFT 5
#define PENDU_TOP (TITLE_TOP+2)
#define PENDU_HEIGHT 8
#define WORD_LEFT (PENDU_LEFT+15)
#define WORD_TOP (PENDU_TOP+3)
#define LETTERS_TOP (WORD_TOP+2)
#define MESSAGE_TOP (PENDU_TOP+PENDU_HEIGHT+1)

enum { MYCOLOR_DEFAULT=1, MYCOLOR_TITLE, MYCOLOR_PENDU, MYCOLOR_PENDU_WARN,
       MYCOLOR_PENDU_LOST, MYCOLOR_LETTERS, MYCOLOR_INFO, MYCOLOR_ERROR };

static WINDOW* mainwin; // fenetre principale

static char strbuf[STRBUF_SIZE];


/// Graphic functions ///

static void setColor(int color) {
  wattron(mainwin, COLOR_PAIR(color));
}
static void cleanLine(int line) {
  mvwhline(mainwin, line, 1, ' ', MAINWIN_WIDTH-2);
}
static void drawMessageAtLine(char* message, int line) {
  cleanLine(line);
  mvwprintw(mainwin, line, (MAINWIN_WIDTH-strlen(message))/2, message);
}
static void drawTitle() {
  wattrset(mainwin, A_BOLD);
  setColor(MYCOLOR_TITLE);
  drawMessageAtLine("Le jeu du Pendu", TITLE_TOP);
  wattrset(mainwin, A_NORMAL);
}
static void drawPendu(int remainingTry) {
  setColor(remainingTry>3 ? MYCOLOR_PENDU : (remainingTry==0 ? MYCOLOR_PENDU_LOST : MYCOLOR_PENDU_WARN));
  int y = PENDU_TOP, x = PENDU_LEFT;
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
  setColor(MYCOLOR_DEFAULT);
  mvwprintw(mainwin, WORD_TOP, WORD_LEFT, "   Word: ");
  wattrset(mainwin, A_BOLD);
  mvwprintw(mainwin, WORD_TOP, WORD_LEFT+9, word);
  wattrset(mainwin, A_NORMAL);
}
static void drawLetters(char* letters) {
  setColor(MYCOLOR_DEFAULT);
  mvwprintw(mainwin, LETTERS_TOP, WORD_LEFT, "Letters: ");
  setColor(MYCOLOR_LETTERS);
  mvwprintw(mainwin, LETTERS_TOP, WORD_LEFT+9, letters);
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


/// User Interface implementations ///

extern void ui_init(Game* game) {
  initscr();
  cbreak();
  noecho();
  mainwin = newwin(MAINWIN_HEIGHT, MAINWIN_WIDTH, (LINES-MAINWIN_HEIGHT)/2, (COLS-MAINWIN_WIDTH)/2);
  keypad(mainwin, TRUE);
  curs_set(0);
  start_color();
  init_pair(MYCOLOR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
  init_pair(MYCOLOR_TITLE, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(MYCOLOR_PENDU, COLOR_GREEN, COLOR_BLACK);
  init_pair(MYCOLOR_PENDU_WARN, COLOR_YELLOW, COLOR_BLACK);
  init_pair(MYCOLOR_PENDU_LOST, COLOR_RED, COLOR_BLACK);
  init_pair(MYCOLOR_LETTERS, COLOR_CYAN, COLOR_BLACK);
  init_pair(MYCOLOR_INFO, COLOR_YELLOW, COLOR_BLACK);
  init_pair(MYCOLOR_ERROR, COLOR_RED, COLOR_BLACK);
  box(mainwin, 0, 0);
  refresh();
}
extern void ui_close() {
  clrtoeol();
  refresh();
  endwin();
}
extern void ui_onGameStart(Game* game) {
  drawTitle();
  drawPendu(game_getRemainingTry(game));
  drawWord(game_getObfuscatedWord(game));
  drawLetters(game_getLetters(game));
  drawInfoMessage("Give me a letter... ");
  wrefresh(mainwin);
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
  wrefresh(mainwin);
}
extern void ui_onGameEnd(Game* game, int status) {
  if(status==-1)
    drawInfoMessage("You lose !");
  else if(status==1)
    drawInfoMessage("You win !");
  refresh();
  napms(500);
  setColor(MYCOLOR_DEFAULT);
  drawMessageAtLine("Press any key to quit... ", MESSAGE_TOP+2);
  wrefresh(mainwin);
  getch();
}
extern int ui_getChar(char* ch) {
  char keycode = getch();
  if(keycode == ERR) return 0;
  *ch = keycode;
  return 1;
}
