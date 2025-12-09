/*
@name Christian tran
@name Brice Matias
@file level1.c
@brief Implements Level 1: Hard maze including NPC interaction, dropped key
locked door, movement, pause menu and win conidtion

Level 1 includes:
	- Maze navigated using arrow keys
	- Npc that moves horizontrally and interacts with player with T keybind
	- A Dropped key objected placed by the NPC when spoke to 
	- Pickup key mechanic using X
	- Door being win condition
	- Pause quit logic with P and Q

this file handles all the gameplay logic, state managegement, rendering for level1. Maze layout is stored in global array
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
@brief Map layout for the game
# = wall
' ' = path
D = locked exist door
*/
static const char *level1_layout[LEVEL1_HEIGHT] = {
	"############################################################",//1
	"#          #        #####                    #             #",//2
	"#  ### ### # ######     # ###### # ####### ### ########### #",//3
	"#  #     # #      # #   # #    # # #     #   #           # #",//4
	"#  # ### # ###### # ##### # ## # # # ### ##### ######## ## #",//5
	"#  # # # #      # #     # # #  # # # # #     #      #   #  #",//6
	"#  # # # ###### # ##### # # # ## # # # ####### #### ## #####",//7
	"#  # # #      # #     # # # #    # # #       # #  #  #     #",//8
	"#  # # ###### ####### # # ###### # # ######### #  #####  # #",//9
	"#  # #      # #     # # #        # #         # #      #  # #",//10
	"#  # ###### # ##### # # #######  #  ######## # # ###### ## #",//11
	"#  #      # #     #           #  #           #        #  # #",//12
	"#  ###### # ##### ########### #  ########### ######## #  ###",//13
	"#       # #     #           # #         #  #       #  #    #",//14
	"####### # ##### ########### # ####### # # ### #### # ###   #",//15
	"#     # #     #         # #           #     #    # #  #    #",//16
	"#     #       #         # #        #  # #        #    #   D#",//17
	"############################################################",//18
};


/*
@struct Level1State
@brief holds all runtime state variables for level 1

Structure tracks the following:
	1. Player Position
	2. NPC position + patrol direction
	3. Whether the level is still running
	4. Key position
	5. Whether the level is still running

*/


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

/*
@brief displays pause menu when player press p or q 
text displays to user if they want to quit and how to via Q or P

Behaviour: 
	1. Pressing Q -> Exits program
	2. pRESSING p -> Resumes gameplay
*/

static void center_text(int y, const char *text) {
	int max_y, max_x;
	(void)max_y;
	getmaxyx(stdscr, max_y, max_x);
	mvprintw(y, (max_x - (int)strlen(text)) / 2, "%s", text);
	
}

/*
@brief displays level completion menu when player completes level
Behaviour:
	Asks user if they want to play level 2
	Asks user if they want to return to main menu 
	Asks user if they want to quit
*/

static void level1_win_menu(void) {
	int max_y, max_x;
	(void)max_x;
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
/*
@brief displays game is paused until it receieves user input

behaviour: 
	Asks user if they want to quit
	Asks user if they want to continue

*/

static void pause_quit_menu(void) {
	int max_y, max_x;
	(void)max_x;
	getmaxyx(stdscr, max_y, max_x);

	clear();
	box(stdscr,0,0);
	center_text(max_y / 2 - 1, "Game is paused.");
	center_text(max_y / 2, "Do you want to quit? ([Q] = Quit, [P] = Continue)");

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
@brief returns the map tile char at (y,x) whith boundary checking
Out of range coords are treated as walls
@Param y row index into maze layout array
@return char tile character from maze

*/


static char level1_get_tile(int y, int x) {
	 if (y < 0 || y >= LEVEL1_HEIGHT || x < 0 || x >= LEVEL1_WIDTH) {
		return '#';
	}
	return level1_layout[y][x];

}

/*
@brief determiens where the player is allowed to move into a tile
Walls (#) cant be entered. Doors are handled separetly
@param new_y for new row
@param new_x for new column
@return true if movement is allowed
@return false if tile is a wall

*/

static char level1_can_move_to(int new_y, int new_x) {
	char tile = level1_get_tile(new_y, new_x);
	return tile != '#';

}
/*
@brief draws the maze layout (walls, space and doors)
@param offset_y vertical offset from the top left corner of the window
@param offset_x horizontal offset for centering

*/
static void level1_render_map(int offset_y, int offset_x) {
	for (int y = 0; y < LEVEL1_HEIGHT; y++) {
		for (int x = 0; x < LEVEL1_WIDTH; x++) {
			char tile = level1_get_tile(y, x);
			mvaddch(offset_y + y, offset_x + x, tile);
		}

	}

}
/*
@brief draws dynamioc game entitires: player, npc , dropped key
Entities:
	O -> Player
	N -> NPC
	K -> Key (only after talking to npc)

@param st Pointer to level 1 state
@param offset_y vertical map offset used for drawing
@param offset_x horizontal map offset used for drawing


*/
static void level1_render_entities(const Level1State *st, int offset_y, int offset_x) {
		mvaddch(offset_y + st->npc_y, offset_x + st->npc_x, 'N');

		if (st->key_on_ground && !st->has_key) {
			mvaddch(offset_y + st->key_y, offset_x + st->key_x, 'K');

		}
		// PLAYER

		mvaddch(offset_y + st->player_y, offset_x + st->player_x, '0');
}

/*
@brief draws the status panel HUD showing states and controls
@param st pointer to level1state containing key info

displays:
	1. if player has key
	2. if key is on ground
	3. Input controls

*/
static void level1_render_hud(const Level1State *st) {
		int max_y, max_x;
		(void)max_x;
		getmaxyx(stdscr, max_y, max_x);
		
		mvprintw(max_y - 3, 2, "LEVEL 1- EASY MAZE");
		mvprintw(max_y - 2, 2, 
			"Key: %s | T = Talk to npc | X = Pick up key | P/Q = Pause and Quit", st->has_key ? "YES" : (st->key_on_ground ? "ON GROUND" : "NO"));
}

/*
@brief renders all level 1 components for the current frame

Includes:
	1. window border
	2. maze layout
	3. NPC, KEY and player entities
	4. hud text
Called once per game loop
@param st Pointer to the current game state

*/

static void level1_render(const Level1State *st) {
		int max_y, max_x;
		(void)max_y;
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

/*
@brief updates npc movement each frame
npc patrols horizontally and reverses direction when it encounters a boundary
@param st poi nter to the level 2 game state

*/
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
/*
Responsible for:
	1. intiailize  all state variables
	2. handle input from keyboard
	3. Animate NPC patrol
	4. Handle NPC interaction
	5. Drop key and allow pickup
	6.  Prevent door from opening until key is collecvted
	7. Support pause quit menu
	8. Display win screens when door is opened

Game ends when:
	Player wins
	Player quits
	Player chooses to return
@return void

*/

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
	(void)max_x;	
	while (st.running) {
		level1_render(&st);
		getmaxyx(stdscr,max_y,max_x);
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
					level1_win_menu();
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
