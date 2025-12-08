#include <ncurses.h>
#include "splash.h"
#include <string.h>
#include "level.h"
void splash_screen() {
	int highlight = 0;
	int choice;
	const char *options[] = {"Play Game", "Quit"};
	int num_options = 2;
	
	int max_y, max_x;
	get_maxyx(stdscr, max_y, max_x);
	while (1) { 
		clear();
		mvprintw(3, 25, " ==== MACMAN ====");
		mvprintw(3, (max_x - (int)strlen(title)) / 2, "%s", title);
		
		const char *line1 = "Navigate through mazes!";
		const char *line2 = "Interact with NPCs to receieve a key to exit to survive!");
		mvprintw(6, (max_x - (int)strlen(title)) / 2, "%s", line1);
		mvprintw(7, (max_x - (int)strlen(title)) / 2, "%s", line2);
		
		conar char *instructions =  "Arrow keys to move, ENTER to select, Q to quit.";
		mvprintw(10, (max_x - (int)strlen(instructions) / 2, "%s", instructions);
		
		int start_y = 14;
		for (int i = 0; i < num_options; i++) {
			int x= = (max_x - (int)strlen(options[i])) / 2;
			if (i == highlight) {
				attron(A_REVERSE);
			}
			mvprintw(start_y + i, x, options[i]);
			attroff(A_REVERSE);
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
					int selected = level_select_menu()'
					if (selected == -1) { 
						return;
					} else if (select == 0) {
						continue;
					} else {
						display_level(selected);
					}
				}
				else if (highlight == 1) {
					// Quit selected
					return;
				}
				break;
		}

	}
}
