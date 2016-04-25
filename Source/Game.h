/*
	============================================================
		Game.h
		Author: Tawfiq Jawhar
		Created: 17-10-2015
		this file holds the game data and the functionality 
		of the game
	============================================================
*/


#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <iostream>
#include "../JuceLibraryCode/JuceHeader.h"
#include "Common.h"
#include <assert.h>
#include "Minimax.h"

struct Minimax; 

struct Game
{
	Game(PLAYERSTATE larvastate, PLAYERSTATE birdstate);

	
	/*
	*	constructor to create a game using the current game but with one player moved
	*	player	is used to choose wich player to move
	*	move	to choose what move to make
	*/

	Game(const Game& other, int player, int move, bool setGameOver);
	~Game() { if (heuristic != nullptr) delete heuristic; }
	void MovePlayer(int player, int move);

	void Initialize();

	bool MouseClick(int mouseX, int mouseY, Minimax& minimax);
	
	//move bird if no collision with other players
	bool MovePlayer(Player* bird, int mouseX, int mouseY);

	//find the bird selected
	Player* findBird(int mouseX, int mouseY);
	
	bool isGameOver();
	bool isWon();

	//returns true if the next movement collides with any of the players
	bool Collision(int mouseX, int mouseY);

	int getPosition(int player);

	int collisionLarvawithBird(int bird);

	void CalculateHeuristic();

	void setHeuristic(int heur) {	heuristic = new int(heur);	}

	Player players[5];
	GAMESTATE state;
	PLAYERSTATE LarvaState;
	PLAYERSTATE BirdState;
	//true for larva false for birds
	bool turn;
	Player* BirdSelected;
	bool GameOver;
	int* heuristic;
	bool gameisValid;


};


#endif