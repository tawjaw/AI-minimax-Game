/*
	============================================================
		Common.h
		Author: Tawfiq Jawhar
		17-10-2015
		this file holds common definitions like enum and structs
	============================================================
*/


#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>

enum GAMESTATE { MAINMENU, START, END };
enum PLAYERSTATE {LarvaHuman, BirdHuman, LarvaAI, BirdAI};

struct Player
{
	int PosX;
	int PosY;
	int type;
	Image image = Image::null;
};


#define LARVA	true
#define BIRD	false

#define PlayerLarva		0		//every player has an integer defining it
#define PlayerBird1		1		//in an array of Players[5] 
#define PlayerBird2		2		//array[0] = array[PlayerLarva] is Larva player
#define	PlayerBird3		3		//array[1] is Bird 1 player .. etc
#define PlayerBird4		4

#define UpRight			1		//the following definitions define the type of the move
#define UpLeft			2		//the players can only mode diagonally
#define DownRight		3		//UpLeft means move up by one step and to the left by one step
#define DownLeft		4

#define MAX				1		//LARVA = MAX	AND		BIRD  = MIN		
#define MIN				0		//we can use LARVA and BIRD instead of MAX and MIN
								//however it is clearer to understand the minimax code with MAX and MIN

#endif
