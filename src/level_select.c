/*
@name Christian tran
@name Brice Matias
@file level_select.c
@brief Implements level selection menu for the game

this module displays a menu allowiong the player to choose
	- level 1 (easy)
	- level 2 (hard)
	- Go back to the splash screen
	- Quit the game (Q)


Navigation:
	- Arrow keys move the lighlight options
	- Enter selects 
	- Q to quit

function returns an int to indicate player choice
and this value is used by the splash screen to launch the correct level. 
*/



#include <ncurses.h>
#include <string.h>
#include "level_select.h"

/*
@brief helper function to print centered text on the screen

@param y the y coordinate to print.
@param txt thew text to print it centered

*/
static void center_text(int y, const char *text) {
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	mvprintw(y, (max_x - (int)strlen(text)) / 2, "%s", text);

}

/*
@brief displ;ays the level select menu and returns the users choice.

@return int
	- 1 -> level 1
	- 2 -> level 2
	- 0 -> go back tp splash
	- -1 -> quit

*/

int level_select_menu(void) {
	const char *options[] = {"Level 1", "Level 2", "Return to Menu"};
	int num_options = 3;
	int highlight = 0;
	int ch;
	
	while (1) {
		clear();
		box(stdscr,0,0);
		
		center_text(3, "========== SELECT LEVEL ==========");
		center_text(6, "Use arrow keys to navigate, ENTER to select.");
		center_text(7, "Press Q to quit.");

		int start_y = 10;
		for (int i = 0; i < num_options; i++) {
			if (i == highlight)
				attron(A_REVERSE);
			center_text(start_y + i , options[i]);

			if (i == highlight)
				attroff(A_REVERSE);

		}
		refresh();
		ch = getch();
		if (ch == 'q' || ch == 'Q')
			return -1;
		switch (ch) {
			case KEY_UP:
				highlight = (highlight - 1 + num_options) % num_options;
				break;
			case KEY_DOWN:
				highlight = (highlight + 1) % num_options;
				break;
			case 10: // ENTER
				if (highlight == 0) return 1;
				if (highlight == 1) return 2;
				if (highlight == 2) return 0; 

		}

	}

}
