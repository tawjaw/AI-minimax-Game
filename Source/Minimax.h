/*
  ==============================================================================

    Minimax.h
    Created: 11 Nov 2015 3:26:09am
    Author:  Tawfiq Jawhar

  ==============================================================================
*/

#ifndef MINIMAX_H_INCLUDED
#define MINIMAX_H_INCLUDED
#include "Game.h"


struct Game;

struct Minimax
{

//	ofstream myfile;

	Minimax(int maxlevel);

	~Minimax();

	int getBestNextMove(const Game game);

	inline void DeleteNodes();

	inline void CreateNodes(const Game &game);

	Game* getParent(int node);

	Game* getParent(__int64 node, int LevelofNode);

	__int64 getParentIndex(__int64 node, int LevelofNode);
	/*
	*	this function calculates the maximum number of nodes the minimax will have based on the level of the minimax
	*	if it is the larva's turn then there is 4 possible moves maximum (UpRight, UpLeft, DownRight, and DownLeft)
	*	if it is the Birds turn then there is 8 possible moves maximum, 2 for each bird (UpRight and UpLeft)
	*
	*	this function also saves the number of nodes in each level to the nodesInLevel array
	*	and the index of the beginning node of every level in the levelBegins array
	*
	*	so to get all the nodes in level x, you go to the index of levelBegins[x] in the array nodes
	*	and you get the nodes from that index to (index + nodesInLevel[x])
	*/
	void CalculateNumberofNodes();


	Game** nodes[2];
	int MaxLevel;
	__int64* levelBegins[2];
	__int64* nodesInLevel[2];
	__int64 NumberOfNodes[2];

	int NumberOfPossibleMoves[2];
	bool isMax;
	int bestvalue = 0;
	int bestmovevalue = -1;
};

#endif  // MINIMAX_H_INCLUDED
