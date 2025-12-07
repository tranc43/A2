#include <ncurses.h>
#include "splash.h"

int main() {
	intscrt();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);

	splash_SCREEN();
	endwin();
	return 0;
	

}
