# MACMAN - CMPT201 Assignment 2 
# Author(s): Christian Tran, Brice Matias 
# Course: CMPT 201 

# Project Overview

MACMAN is a terminal based - Maze game written in C using n curses
the game includes the following:
	- Splash Screen
	- Level selection Menu
	- Level 1: Easy maze with NPC interaction, key pick up and door unlocking
	- Level 2: Harder maze, same mechanics
	- Player movement with arrow keys
	- Quit (Q)
	- Pause Menu (P)
	- Modular deisng 
	- Doxygen Documentation
	- Build automation completed in Makefile

# Directory Structure
src contains all .c and .h files including the make file.

bin contains the executable file

doc contains the HTML and LATEX

Doxyfile and README.MD are inside the A2 directory.

# Global controls

Arrow keys to move
P to pause
Q to quit (with confirmation)
Enter to select

# Interaction controls
T to talk to NPC
X to pick up dropped key
M return to main menu win screen

# Level descriptions

Level 1 
	1. NPC drops a key when spoken to (T)
	2. Player has to pick it up (X)
	3. Door (D) only opens if player has the key
The win screen offers
	- Continue to level 2 
	- Return to main menu
	- Quit

Level 2
	1. Harder made
	2. Same mechanics
	3. Win screen

# How to run
Inside the src directory run make
