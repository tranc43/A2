#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "level2.h"

#define LEVEL2_HEIGHT 18
#define LEVEL2_WIDTH 60


/*
# = wall
' ' = path
D = locked exist door
N = Npc ara

*/
static const char *level2_layout[LEVEL2_HEIGHT] = {
	"############################################################",//1
	"#          #        ######       #           #            D#",//2
	"#  ####### # ###### #   # ###### # ####### ### #############",//3
	"#  #     # #      # #   # #    # # #     #   #           ###",//4
	"#  # ### # ###### # ##### # ## # # # ### ##### ######   ####",//5
	"#  # # # #      # #     # # #  # # # # #     #      #     ##",//6
	"#  # # # ###### # ##### # # # ## # # # ####### #### ## #####",//7
	"#  # # #      # #     # # # #    # # #       # #  #      ###",//8
	"#  # # ###### # ##### # # ######## # ######### #  #####  ###",//9
	"#  # #      # #     # # #        # #         # #      # #  #",//10
	"#  # ###### # ##### # # #######  #  ######## #  ########## #",//11
	"#  #      # #     #           #  #       N   #        #    #",//12
	"#  ###### # ##### ########### #  ########### ######## #   ##",//13
	"#       # #     #           # #         #   #      # #     #",//14
	"####### # ##### ########### # ####### # # ### #### # ###   #",//15
	"#     # #     #         # #           #     #    # #       #",//16
	"#     #       #         # #        #    #        #         #",//17
	"############################################################",//18
};


// Game state
typedef struct {
	int player_y, player_x;
	int npc_y, npc_x;
	int npc_dir; // 1 = move right, -1 = move left

	int enemy_y, enemy_x;
	int enemy_dir; // 1 = moving down, -1 m= moving up

	bool has_key;
	bool running;

} level2State;

static void center_text(int y, const char *text) {
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	mvprintw(y, (max_x - (int)strlen(text)) / 2, "%s", text);
	
}

static void pause_quit_menu(void) {
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);

	clear();
	box(stdscr,0,0);
	center_text(max_y / 2 - 1, "Game is paused.");
	center_text(max_y / 2, "Do you want to quit? (Q = Quit, P =  Continue");

	int ch;
	while (1) {
		ch = getch();
		if (ch == 'q' || ch == 'Q') {
			endwin();
			exit(0);
		}
		if (ch == 'p' || ch == 'P') {
			break'
		}

	}
}

static void game_end_screen(const char *line1, const char *line2) {
	int max_y, int max_x;
	getmaxyx(stdscr< max_y, max_x);

}



