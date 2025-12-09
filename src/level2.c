/*
@name Christian Tran 
@name Brice Matias
@file level2.c
@brief Implements Level 2: Hard maze including NPC interaction, dropped key
locked door, movement, pause menu and win conidtion

Level 2 includes:
	- Maze navigated using arrow keys
	- Npc that moves horizontrally and interacts with player with T keybind
	- A Dropped key object placed by the NPC when spoke to
	- Pickup key mechanic using X
	- Door being the win condition
	- Pause/Quit logic with P and Q

This file handles all the gameplay logic, state management, rendering for level2. Maze layout is stored in global array
*/

#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "level2.h"

#define LEVEL2_HEIGHT 18
#define LEVEL2_WIDTH 60


/*
@brief Map layout for the game
# = wall
' ' = path
D = locked exist door
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
	"#  # # ###### ####### # # ######## # ######### #  #####  ###",//9
	"#  # #      # #     # # #        # #         # #      #  # #",//10
	"#  # ###### # ##### # # #######  #  ######## #  ########## #",//11
	"#  #      # #     #           #  #           #        #    #",//12
	"#  ###### # ##### ########### #  ########### ######## #   ##",//13
	"#       # #     #           # #         #  #       #  #    #",//14
	"####### # ##### ########### # ####### # # ### #### # ###   #",//15
	"#     # #     #         # #           #     #    # #       #",//16
	"#     #       #         # #        #    #        #         #",//17
	"############################################################",//18
};


/*
@struct Level2State
@brief Holds all runtime state variables for level 2 

Structure tracks the following:
	1. Player Position
	2. NPC position + patrol direction
	3. Whether the key has been dropped or picked up
	4. Key position
	5. whether the level is still running

*/
typedef struct {
	int player_y, player_x;
	int npc_y, npc_x;
	int npc_dir; // 1 = move right, -1 = move left
		
	bool has_key;
	bool key_on_ground;
	int key_y, key_x;
	bool running;

} Level2State;

/*
@brief Prints text horizontally at the given row
@param y Row coordinate where text prints
@param txt Null terminated string to display
*/

static void center_text(int y, const char *text) {
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	mvprintw(y, (max_x - (int)strlen(text)) / 2, "%s", text);
	
}

/*
@brief displays pause menu when player presses P or Q

text displays to user if they want to quit and how to via Q or P.

Behaviour:
	1. Pressing Q -> exits program
	2. Pressing P -> Resumes gameplay

*/

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
/*
@brief Displays end level of screen when player completes level

The screen allows for:
	1. M -> return to main menu
	2. Q -> Quit program

@param line1 title text
@param line2 description text
*/

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

/*
@#brief Returns the map tile char at (y,x) with boundary checking.

Out of range coords are treated as walls.
@Param y row index into m aze layout array
@param x column index
@return chart tile character from maze.

*/

static char level2_get_tile(int y, int x) {
	 if (y < 0 || y >= LEVEL2_HEIGHT || x < 0 || x >= LEVEL2_WIDTH) {
		return '#';
	}
	return level2_layout[y][x];

}
/*
@brief determines whether the player is allowed to move into a tile

Walls (#) cant be entered. Doors are handled separately "D" in the movement logic

@param new_y for new row
@param new_x for new column
@return true if movement is allowed
@return false if tile is a wall.

*/
static char level2_can_move_to(int new_y, int new_x) {
	char tile = level2_get_tile(new_y, new_x);
	return tile != '#';

}

/*
@brief draws the maze layout (walls, space and doors)
@param offset_y vertical offset from the top left corner of the window
@param offset_x horizontal offset for centering 

*/


static void level2_render_map(int offset_y, int offset_x) {
	for (int y = 0; y < LEVEL2_HEIGHT; y++) {
		for (int x = 0; x < LEVEL2_WIDTH; x++) {
			char tile = level2_get_tile(y, x);
			mvaddch(offset_y + y, offset_x + x, tile);
		}

	}

}

/*
@brief draws dynamic game entities: player,npc, dropped key

Entities:
	O -> player
	N -> NPC
	k -> key (only after interaction)

@param st Pointer to level 2 state
@param offset_y vertical map offset used for drawing
@param offset_x Horizontal map offset used fopr drawing

*/
static void level2_render_entities(const Level2State *st, int offset_y, int offset_x) {
		mvaddch(offset_y + st->npc_y, offset_x + st->npc_x, 'N');

		if (st->key_on_ground && !st->has_key) {
			mvaddch(offset_y + st->key_y, offset_x + st->key_x, 'K');

		}
		// PLAYER

		mvaddch(offset_y + st->player_y, offset_x + st->player_x, '0');
}

/*
@brief draws the status panel HUD showing states and controls
@param st Pointer to level2State containing key info

Displays
	1. If player has key
	2. If key is on ground
	3. Input controls
*/
static void level2_render_hud(const Level2State *st) {
		int max_y, max_x;
		getmaxyx(stdscr, max_y, max_x);
		
		mvprintw(max_y - 3, 2, "LEVEL 2- HARD MAZE");
		mvprintw(max_y - 2, 2, 
			"Key: %s | T = Talk to npc | X = Pick up key | P/Q = Pause and Quit", st->has_key ? "YES" : (st->key_on_ground ? "ON GROUND" : "NO"));
}


/*
@brief renders all level 2 components for the current frame.

Includes 
	1. Window border
	2. Maze layout
	3. Npc, key and player entities
	4. hud text

Called once per game loop.

@param st Pointer to the current game state

*/
static void level2_render(const Level2State *st) {
		int max_y, max_x;
		getmaxyx(stdscr, max_y, max_x);
		clear();
		box(stdscr, 0,0);
		
		int offset_y = 1;
		int offset_x = (max_x - LEVEL2_WIDTH) / 2;	
		level2_render_map(offset_y, offset_x);
		level2_render_entities(st, offset_y, offset_x);
		level2_render_hud(st);

		refresh();

}


/*
@brief Updattes npc movement each frame
Npc Patrols horizontally and reverses direction when it encounters a boundary
@param st pointer to the level 2 game state
*/

static void level2_update_npc(Level2State *st) {
	int next_x = st->npc_x + st->npc_dir;

	if (next_x < 0 || next_x >= LEVEL2_WIDTH) {
		st->npc_dir *= -1;
		return;
	}
	if (level2_get_tile(st->npc_y, next_x) == '#') {
		st->npc_dir *= -1;
		return;
	}
	st->npc_x = next_x;
}




/*main gameplay */


void play_level2(void) {
	Level2State st;

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
		level2_render(&st);
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

		if (level2_can_move_to(new_y, new_x)) {
			char tile = level2_get_tile(new_y, new_x);

			if (tile == 'D') {
				if(st.has_key) {
					game_end_screen("YOU WIN!", "You've escaped the most difficult maze of all time.");
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
			
					if (level2_get_tile(drop_y, drop_x) == '#') {
						drop_y = st.npc_y -1;
					}
					if (level2_get_tile(drop_y, drop_x) == '#') {
						drop_y = st.npc_y;
						drop_x = st.npc_x + 1;
					}
					if (level2_get_tile(drop_y, drop_x) == '#') {
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
		level2_update_npc(&st);
	}
}
