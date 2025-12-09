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
		
	bool has_key;
	bool key_on_ground;
	int key_y, key_x;
	bool running;

} Level2State;

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

	clear();
	box(stdscr, 0, 0);
	center_text(max_y / 2 - 2, line1);
	center_text(max_y / 2 - 1, line2);
	center_text(max_y / 2 + 1, "Press M to return main menu or Q to quit.");
	refresh();

	int ch;
	while (1) {
		ch = getch();
		if (ch == 'q' || ch == 'Q') {
			endwin();
			exit(0);
		}
		if (ch == 'm' || ch == 'M') {
			break;
		}
	}
}



static char level2_get_tile(int y, int x) {
	 if (y < 0 || y >= LEVEL2_HEIGHT || x < 0 || x >= LEVEL2_WIDTH) {
		return '#';
	}
	return level2_layout[y][x];

}

static char level2_can_move_to(int new_y, int new_x) {
	char tile = level2_get_tile(new_y, new_x);
	return tile != '#';

}

static void level2_render_map(int offset_y, int offset_x) {
	for (int y = 0; y < LEVEL2_HEIGHT; y++) {
		for (int x = 0; x < LEVEL2_WIDTH; x++) {
			char tile = level2_get_tile(y, x);
			mvaddch(offset_y + y, offset_x + x, tile);
		}

	}

}

static void level2_render_entitites(const Level2State *st, int offset_y, int offset_x) {
		mvaddch(offset_y + st->npc_y, offset_x + st->npc_X, 'N');

		if (st->key_on_ground && !st->has_key) {
			mvaddch(offset_y + st->key_y, offset_x + st->key_x, 'K');

		}

}







