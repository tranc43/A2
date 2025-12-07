#include <ncurses.h>
#include "splash.h"

void splash_screen() {
	int highlight = 0;
	int choice;
	const char *options[] = {"Play Game", "Quit"};
	int num_options = 2;
	
	while (1) { 
		clear();
		mvprintw(3, 25, " ==== MACMAN ====");
	
		mvprintw(5, 10, "Navigate through mazes!");
		mvprintw(6, 10, "Interact with NPCS to receive a key to exit to survive!");
		
		mvprintw(8, 10, "Arrow keys to move, ENTER to select.");
		mvprintw(9, 10, "Press 'Q' to quit");
	
		for (int i = 0; i < num_options; i++) {
			if (i == highlight) {
				attron(A_REVERSE);
			}
			mvprintw(12 + i, 10, options[i]);
			attroff(A_REVERSE);
		}
		refresh();
		choice = getch();
		// Quit 
		if (choice == 'q' || choice == 'Q') {
			endwin();
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
					clear();
					mvprintw(5, 10, "Starting game.. (Level system will be here)");
					refresh();
					refresh();
					getch();
					return;
				}
				else if (highlight == 1) {
					// Quit selected
					endwin();
					return;
				}
				break;
		}

	}
}
