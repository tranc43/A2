/*
@name Christian Tran
@name Brice Matias
@file main.c
@brief Entry point for the MACMAN game

this file intiializes ncurses, launches and the splash screen 
then after cleans up the terminal environment when the game ends.


Main function does not run gameplay directly it only delegates.

	splash_screen() -> menu
	level_select_menu() -> choose a level
	play_level1() / play_level2() -> gameplay loops

*/



#include <ncurses.h>
#include "splash.h"


/*
@brief Program entry point intiailzies ncurses and run splash screen
@return int always return 0 on clean exit.

*/

int main() {
	initscr();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);

	splash_screen();
	endwin();	
	splash_screen();
	endwin();
	return 0;

}
