/*
@file level1.c
@brief Implements Level 1: Hard maze including NPC interaction, dropped key
locked door, movement, pause menu and win conidtion

Level 1 includes:
	- Maze navigated using arrow keys
	- Npc that moves horizontrally and interacts with player with T keybind

*/




#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "level1.h"
#include "level2.h"
#define LEVEL1_HEIGHT 18
#define LEVEL1_WIDTH 60


/*
# = wall
' ' = path
D = locked exist door


*/
static const char *level1_layout[LEVEL1_HEIGHT] = {
	"############################################################",//1
	"#          #        #####                    #            D#",//2
	"#  ### ### # ###### #   # ###### # ####### ### #############",//3
	"#  #     # #      # #   # #    # # #     #   #           ###",//4
	"#  # ### # ###### # ##### # ## # # # ### ##### ######   ####",//5
	"#  # # # #      # #     # # #  # # # # #     #      #     ##",//6
	"#  # # # ###### # ##### # # # ## # # # ####### #### ## #####",//7
	"#  # # #      # #     # # # #    # # #       # #  #      ###",//8
	"#  # # ###### ####### # # ###### # # ######### #  #####  ###",//9
	"#  # #      # #     # # #        # #         # #      #  # #",//10
	"#  # ###### # ##### # # #######  #  ######## #  ########## #",//11
	"#  #      # #     #           #  #           #        #    #",//12
	"#  ###### # ##### ########### #  ########### ######## #   ##",//13
	"#       # #     #           # #         #  #       #  #    #",//14
	"####### # ##### ########### # ####### # # ### #### # ###   #",//15
	"#     # #     #         # #           #     #    # #       #",//16
	"#     #       #         # #        #  # #        #         #",//17
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

} Level1State;

static void center_text(int y, const char *text) {
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	mvprintw(y, (max_x - (int)strlen(text)) / 2, "%s", text);
	
}

static void level1_win_menu(void) {
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);

	clear();
	box(stdscr, 0, 0);

	center_text(max_y / 2 - 3, "LEVEL 1 COMPLETE");
	center_text(max_y / 2 - 1, "What would you like to do next?");
	center_text(max_y / 2 + 1, "[1] Play Level 2");
	center_text(max_y / 2 + 2, "[M] Return to Main Menu");
	center_text(max_y / 2 + 3, "[Q] Quit Game");

	refresh();

	int ch;

	while (1) {
		ch = getch();
		if (ch == '1') {
			play_level2();
			break;
		} else if (ch == 'm' || ch == 'M') {
			break;
		} else if (ch == 'q' || ch == 'Q') {
			endwin();
			exit(0);
		}

	}
	

}

static void pause_quit_menu(void) {
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);

	clear();
	box(stdscr,0,0);
	center_text(max_y / 2 - 1, "Game is paused.");
	center_text(max_y / 2, "Do you want to quit? (Q = Quit, P =  Continue)");

	int ch;
	while (1) {
		ch = getch();
		if (ch == 'q' || ch == 'Q') {
			endwin();
			exit(0);
		}
		if (ch == 'p' || ch == 'P') {
			break;
		}

	}
}

static void game_end_screen(const char *line1, const char *line2) {
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);

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



static char level1_get_tile(int y, int x) {
	 if (y < 0 || y >= LEVEL1_HEIGHT || x < 0 || x >= LEVEL1_WIDTH) {
		return '#';
	}
	return level1_layout[y][x];

}

static char level1_can_move_to(int new_y, int new_x) {
	char tile = level1_get_tile(new_y, new_x);
	return tile != '#';

}

static void level1_render_map(int offset_y, int offset_x) {
	for (int y = 0; y < LEVEL1_HEIGHT; y++) {
		for (int x = 0; x < LEVEL1_WIDTH; x++) {
			char tile = level1_get_tile(y, x);
			mvaddch(offset_y + y, offset_x + x, tile);
		}

	}

}

static void level1_render_entities(const Level1State *st, int offset_y, int offset_x) {
		mvaddch(offset_y + st->npc_y, offset_x + st->npc_x, 'N');

		if (st->key_on_ground && !st->has_key) {
			mvaddch(offset_y + st->key_y, offset_x + st->key_x, 'K');

		}
		// PLAYER

		mvaddch(offset_y + st->player_y, offset_x + st->player_x, '0');
}


static void level1_render_hud(const Level2State *st) {
		int max_y, max_x;
		getmaxyx(stdscr, max_y, max_x);
		
		mvprintw(max_y - 3, 2, "LEVEL 1- EASY MAZE");
		mvprintw(max_y - 2, 2, 
			"Key: %s | T = Talk to npc | X = Pick up key | P/Q = Pause and Quit", st->has_key ? "YES" : (st->key_on_ground ? "ON GROUND" : "NO"));
}


static void level1_render(const Level2State *st) {
		int max_y, max_x;
		getmaxyx(stdscr, max_y, max_x);
		clear();
		box(stdscr, 0,0);
		
		int offset_y = 1;
		int offset_x = (max_x - LEVEL1_WIDTH) / 2;	
		level1_render_map(offset_y, offset_x);
		level1_render_entities(st, offset_y, offset_x);
		level1_render_hud(st);

		refresh();

}


/*NPC */

static void level1_update_npc(Level1State *st) {
	int next_x = st->npc_x + st->npc_dir;

	if (next_x < 0 || next_x >= LEVEL1_WIDTH) {
		st->npc_dir *= -1;
		return;
	}
	if (level1_get_tile(st->npc_y, next_x) == '#') {
		st->npc_dir *= -1;
		return;
	}
	st->npc_x = next_x;
}




/*main gameplay */


void play_level1(void) {
	Level1State st;

	st.player_y = 1;
	st.player_x = 1;

	st.npc_y = 11;
	st.npc_x = 21;
	st.npc_dir = 1;

	st.has_key = false;
	st.key_on_ground = false;
	st.key_y = 0;
	st.key_x = 0;

	st.running = true;

	int max_y, max_x;

	while (st.running) {
		level1_render(&st);
		getmaxyx(stdscr, max_y, max_x);
	
		int ch = getch();
		
		if (ch == 'p'|| ch == 'P' || ch == 'q' || ch == 'Q') {
			pause_quit_menu();
			continue;
		}
		// Plater movement
		
		int new_y = st.player_y;
		int new_x = st.player_x;

		if (ch == KEY_UP) new_y--;
		if (ch == KEY_DOWN) new_y++;
		if (ch == KEY_LEFT) new_x--;
		if (ch == KEY_RIGHT) new_x++;

		if (level1_can_move_to(new_y, new_x)) {
			char tile = level1_get_tile(new_y, new_x);

			if (tile == 'D') {
				if(st.has_key) {
					game_end_screen("YOU WIN!", "escaped hard maze using key");
					st.running = false;
					continue;
					
				}else { 
					mvprintw(max_y - 4, 2, "The door is locked. find key..");
					refresh();
					getch();
				}
			}else {
				st.player_y = new_y;
				st.player_x = new_x;
			}
		}
		if (ch == 't' || ch == 'T') {
			int dy = st.player_y - st.npc_y;
			int dx = st.player_x - st.npc_x;
			
			if (abs(dx) + abs(dy) <= 1) {
				if (!st.has_key && !st.key_on_ground) {
					int drop_y = st.npc_y + 1;
					int drop_x = st.npc_x;
			
					if (level1_get_tile(drop_y, drop_x) == '#') {
						drop_y = st.npc_y -1;
					}
					if (level1_get_tile(drop_y, drop_x) == '#') {
						drop_y = st.npc_y;
						drop_x = st.npc_x + 1;
					}
					if (level1_get_tile(drop_y, drop_x) == '#') {
						drop_x = st.npc_x - 1;
					}

					st.key_y = drop_y;
					st.key_x = drop_x;
					st.key_on_ground = true;
			
					mvprintw(max_y - 4, 2, "NPC: Dropped a key, pick it up with X!");
					refresh();
					getch();
				} else if (st.has_key) {
					mvprintw(max_y - 4, 2, "NPC: You picked it up already!");
					refresh();
					getch();
				} else {
					mvprintw(max_y - 4, 2, "NPC: The keys on the ground still..");
					refresh();
					getch();
				}
			}
		}
		if ((ch == 'x' || ch == 'X') && st.key_on_ground && st.player_y == st.key_y && st.player_x == st.key_x) {
			st.key_on_ground = false;
			st.has_key = true;
			
			mvprintw(max_y - 4, 2, "You picked up the key!");
			refresh();
			getch();
		}
		level1_update_npc(&st);
	}
}
