#include <ncurses.h>
#include <string.h>
#include "level.h"

int level_select_menu(void) {
	const char *options[] = { "Level 1", "Level 2", "Go back"};
	int num_options 3; 
	int hightlight = 0;	
	int ch;
	
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	
	while (1) {
		clear();
	
		const char *title = "=== LEVEL SELECTION ===";
		mvprintw(3, (max_x - (int)strlen(title)) / 2, "%s", title);
		mvprintw(6, (max_x - 34) / 2, "Choose a level or return to menu");
		mvprintw(8, (max_x - 45) / 2,  "Use UP/DOWN, ENTER to Select, 'Q' to quit");
		int start_y = 11;
		for (int i = 0; i < num_options; i++) {
			int x = (max_x - (int)strlen(options[i])) / 2;
			
			if (i == highlight) 
				attron(A_REVERSE);
			mvprintw(start_y + i, x, "%s", options[i]);

			if (i == highlight)
				attroff(A_REVERSE);
		}

		refresh();
		ch = getch();
		
		if (ch == 'q' || ch == 'Q') {
			return -1;
		}
		switch (ch) {
			case KEY_UP:
				highlight--;
				if (highlight < 0) 
					highlight = num_options -1;
				break;
			case KEY_DOWN:
				highlight++;
				if (highlight >= num_options)
					highlight = 0;
				break;
			case 10:
				if (highlight == 0) return 1;
				if (highlight == 1) return 2;
				if (highlight == 2) return 0;
		}

	}

}


void display_level(int level) {
	clear();
	mvprintw(5, 10, "Starting level %d..", level);
	mvprintw(7, 10, "Level implementation here later"));
	mvprintw(9, 10, "Press any key to return to the main menu.");
	refresh();
	getch();


}
