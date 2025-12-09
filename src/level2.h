/*
@name Christian tran
@name Christian tran
@file level2.h
@brief function prototype for running Level 2 (hard level)

Level 2 contains:
	- Maze navigation with arrow keys to move
	- Interactable NPC using T
	- Drop key element after talking to NPC using X
	- Locked exit door requiring the key
	- Pause/Quit using P and Q

Module is responsible for only managing the gameplay loop for level 2
all rendering and logic is implemented inside of level2.c itself.

*/


#ifndef LEVEL2_H
#define LEVEL2_H

/*
@brief Starts level 2 gameplay.

this function intiializes all level 2 state variables
enters the input/update/render loop and only ends if the player
either completes or quit the level

*/

void play_level2(void);

#endif
