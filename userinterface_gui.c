#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>

#include "userinterface.h"
#include "pendu.h"

#define STRBUF_SIZE 64

#define MAINWIN_HEIGHT 20
#define MAINWIN_WIDTH 50
#define MAINWIN_PADDING_TOP ((LINES-MAINWIN_HEIGHT)/2)
#define MAINWIN_PADDING_LEFT ((COLS-MAINWIN_WIDTH)/2)

#define TITLE_TOP (MAINWIN_PADDING_TOP)
#define PENDU_TOP (TITLE_TOP+2)
#define PENDU_HEIGHT 8
#define WORD_LEFT (MAINWIN_PADDING_LEFT+20)
#define WORD_TOP (PENDU_TOP+3)
#define LETTERS_TOP (WORD_TOP+2)
#define MESSAGE_TOP (PENDU_TOP+PENDU_HEIGHT+1)

enum { MYCOLOR_BACKGROUND, MYCOLOR_DEFAULT, MYCOLOR_TITLE, MYCOLOR_PENDU, MYCOLOR_PENDU_WARN,
       MYCOLOR_PENDU_LOST, MYCOLOR_LETTERS, MYCOLOR_INFO, MYCOLOR_ERROR };

/// GUI globals ///
static WINDOW* mainwin; // fenetre principale

static char strbuf[STRBUF_SIZE];

static void setColor(int color) {
  wattron(mainwin, COLOR_PAIR(color));
}
static void cleanLine(int line) {
  mvwhline(mainwin, line, 0, ' ', COLS);
}
static void drawMessageAtLine(char* message, int line) {
  cleanLine(line);
  mvwprintw(mainwin, line, MAINWIN_PADDING_LEFT+(MAINWIN_WIDTH-strlen(message))/2, message);
}
static void drawTitle() {
  wattrset(mainwin, A_BOLD);
  setColor(MYCOLOR_TITLE);
  drawMessageAtLine("Le jeu du Pendu", TITLE_TOP);
  wattrset(mainwin, A_NORMAL);
}
static void drawPendu(int remainingTry) {
  setColor(remainingTry>3 ? MYCOLOR_PENDU : (remainingTry==0 ? MYCOLOR_PENDU_LOST : MYCOLOR_PENDU_WARN));
  int y = PENDU_TOP, x = MAINWIN_PADDING_LEFT;
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

// Curses init
extern void ui_init(Game* game) {
  mainwin = initscr();
  noecho();
  cbreak();
  keypad(mainwin, TRUE);
  curs_set(0);
  start_color();
  init_pair(MYCOLOR_BACKGROUND, COLOR_BLACK, COLOR_WHITE);
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
  drawInfoMessage("Give me a letter... ");
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
  refresh();
  napms(500);
  setColor(MYCOLOR_DEFAULT);
  drawMessageAtLine("Press any key to quit... ", MESSAGE_TOP+2);
  refresh();
  getch();
}
extern int ui_getChar(char* ch) {
  char keycode = getch();
  if(keycode == ERR) return 0;
  *ch = keycode;
  return 1;
}
