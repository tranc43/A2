#include <ncurses.h>
#include "splash.h"

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
