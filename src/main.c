#include <ncurses.h>
#include "splash.h"

int main() {
	intscr();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);

	splash_screen();
	endwin();
	return 0;
	

}
