/*
@name Christian tran
@name Brice Matias
@file level1.h
@brief function prototype for running Level 1 (easy level)

Level 1 contains:
	- Maze navigation with arrow keys to move
	- Interactable NPC using T
	- Drop key element after talking to NPC using X
	- Locked exit door requiring the key
	- Pause/Quit using P and Q

Module is responsible for only managing the gameplay loop for level 1
all rendering and logic is implemented inside of level1.c itself.

*/


#ifndef LEVEL1_H
#define LEVEL1_H

/*
@brief Starts level 1 gameplay.

this function intiializes all level 1 state variables
enters the input/update/render loop and only ends if the player
either completes or quit the level

*/

void play_level1(void);

#endif
