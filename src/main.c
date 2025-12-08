#include <ncurses.h>
#include "splash.h"

int main() {
	initscr();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);

	splash_screen();
	endwin();
	return 0;

	if (has_colors()) {
		start_color();
		use_default_colors();

		init_pair(1, COLOR_GREEN, -1);
	}	
	splash_screen();
	endwin();
	return 0;

}
