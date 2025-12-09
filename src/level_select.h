/*
@name CHRISTIAN TRAN
@brief header for the level selection menu. 

Selection menu allows the player to choose either
level 1 (easy)
level 2(hard)

Module handles menu navigations with arrow keys and enter selection, returning an integer
to indicate which level or action was chosen
*/

#ifndef LEVEL_SELECT_H
#define LEVEL_SELECT_H


/*
 @brief displays the level selection menu and waits for input from player

Controls:
	- Arrow keys: move to highlight
	- ENTER: select option
	- Q: quit game

@return int
	1 -> level 1 
	2 -> level 2 selected
	0 -> go back to splash screen
	-1 -> Quit the program
*/

int level_select_menu(void);

#endif
