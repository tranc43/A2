#include <ncurses.h>
#include "splash.h"
#include <string.h>
#include "level.h"

void center_text(int y, const char *text) {
	int max_y, max_x;
	getmaxyx(stdscr,  max_y, max_x);
	mvprintw(y, (max_x - (int)strlen(text)) / 2, "%s", text);
	

}

void splash_screen() {
	int highlight = 0;
	int choice;
	const char *options[] = {"Play Game", "Quit"};
	int num_options = 2;
	
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	while (1) { 
		clear();
		box(stdscr, 0, 0);
	

		attron(A_BOLD);
		center_text(3,"========= MACMAN =========");
		attroff(A_BOLD);


		center_text(6, "Navigate through mazes!");
		center_text(7, "Interact with NPCs to receieve a key to exit to survive!");
	
		attron(A_BOLD);		
		center_text(10, "Arrow keys to move, ENTER to select, Q to quit.");
		attroff(A_BOLD);
		int start_y = 14;
		for (int i = 0; i < num_options; i++) {
			if (i == highlight) {
				attron(A_REVERSE);
			}
			center_text(start_y + i, options[i]);
			if (i == highlight) {
				attroff(A_REVERSE);
			}


		}
		refresh();
		choice = getch();
		// Quit 
		if (choice == 'q' || choice == 'Q') {
			return;
		}
		switch (choice) {
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
			case 10: // ENTER
				if (highlight == 0) {
					int selected = level_select_menu();
					if (selected == -1) 
						return;
					 
					if (selected == 0) 
						continue;
					if (selected == 1) 
						display_level(selected);
				}
				else if (highlight == 1) {
					// Quit selected
					return;
				}
				break;
		}

	}
}
