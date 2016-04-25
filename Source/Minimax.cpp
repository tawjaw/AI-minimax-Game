/*
  ==============================================================================

    Minimax.cpp
    Created: 16 Nov 2015 8:47:01pm
    Author:  Tawfiq Jawhar

  ==============================================================================
*/

#include "Minimax.h"

Minimax::Minimax(int maxlevel)
  {
	  //level 11 is the maximum number of nodes the system can handle (level 12 on a 64 bit build)
	  assert(maxlevel > 0);
	  assert(maxlevel < 12);
	  MaxLevel = maxlevel;
	  //turn = game.turn;
	  NumberOfPossibleMoves[MAX] = 4;
	  NumberOfPossibleMoves[MIN] = 8;
	  levelBegins[MAX] = new __int64[maxlevel];
	  levelBegins[MIN] = new __int64[maxlevel];
	  nodesInLevel[MAX] = new __int64[maxlevel];
	  nodesInLevel[MIN] = new __int64[maxlevel];
	  CalculateNumberofNodes();
	  nodes[MAX] = new Game*[NumberOfNodes[MAX]];
	  nodes[MIN] = new Game*[NumberOfNodes[MIN]];
	  //myfile.open("example.txt");
	  //	cout << getNumberofNodes() << endl;

  }

Minimax::~Minimax()
{
	if (nodes != nullptr)
	{
		delete nodes[MAX]; delete nodes[MIN];
	}
	delete nodesInLevel[MAX];
	delete nodesInLevel[MIN];
	delete levelBegins[MAX];
	delete levelBegins[MIN];
}

int Minimax::getBestNextMove(const Game game)
{
	//TODO
	//generate the game nodes.. done, needs testing
	//check if the game is won or lost to stop generating nodes after it.. done
	//cout << "started getting best move" << endl;
	isMax = game.turn;
	CreateNodes(game);

	for (int level = MaxLevel - 1; level > 0; level--)
	{
		//cout << "for loop in level " << level << endl;
		//cout << levelBegins[isMax][level] << endl;
		int LevelofNodeisMax = MAX;
		if (((level) % 2) == isMax)		LevelofNodeisMax = MIN;
		for (__int64 child = levelBegins[isMax][level]; child < (levelBegins[isMax][level] + nodesInLevel[isMax][level]); child++)
		{
			if (nodes[isMax][child] != nullptr)
			{
				__int64 parent = getParentIndex(child, level);
				if (nodes[isMax][child]->heuristic == nullptr)
				{
					nodes[isMax][child]->CalculateHeuristic();
				}
				if (nodes[isMax][parent]->heuristic == nullptr)
					nodes[isMax][parent]->setHeuristic(*nodes[isMax][child]->heuristic/*- nodes[isMax][parent]->blocks*10*/);
				else if (!LevelofNodeisMax)
				//else if(isMax)
				{
					if (*nodes[isMax][parent]->heuristic > *nodes[isMax][child]->heuristic)
						nodes[isMax][parent]->setHeuristic(*nodes[isMax][child]->heuristic /*- nodes[isMax][parent]->blocks * 10*/);
				}
				else
				{
					if (*nodes[isMax][parent]->heuristic < *nodes[isMax][child]->heuristic)
						nodes[isMax][parent]->setHeuristic(*nodes[isMax][child]->heuristic /*- nodes[isMax][parent]->blocks * 10*/);
				}
			}

		}
	}

	/*
	for (int i = 0; i < levelBegins[isMax][1]; i++)
	{
	if (nodes[isMax][i] != nullptr)
	cout << "node number " << i << " heuristic: " << *nodes[isMax][i]->heuristic << endl;
	}
	for (int i = levelBegins[isMax][2]; i <  levelBegins[isMax][3]; i++)
	{
	if (nodes[isMax][i] != nullptr)
	cout << *nodes[isMax][i]->heuristic << "   " << *getParent(i)->heuristic << endl;
	}*/

	//get the best move
	int bestHeuristic ;
	int bestMove ;
	for (int i = 0; i < NumberOfPossibleMoves[isMax]; i++)
	{
		if (nodes[isMax][i] != nullptr)
		{
			if (nodes[isMax][i]->heuristic == nullptr)
				nodes[isMax][i]->CalculateHeuristic();
		}
	}
	for (int i = 0; i < NumberOfPossibleMoves[isMax]; i++)
	{
		if (nodes[isMax][i] != nullptr)
		{
			if (nodes[isMax][i]->heuristic == nullptr)
				nodes[isMax][i]->CalculateHeuristic();
			bestHeuristic = *nodes[isMax][i]->heuristic;
			bestMove = i;
			break;
		}
	}
	
	int bestmovetemp = bestMove;
	for (int i = 0; i < NumberOfPossibleMoves[isMax]; i++)
	{
		
		if (nodes[isMax][i] != nullptr)
		{
			assert(nodes[isMax][i]->gameisValid == true);
			if (isMax)
			{
				if (i != bestmovetemp )
				{
					if (*nodes[isMax][i]->heuristic > bestHeuristic)
					{
						bestMove = i;
						bestHeuristic = *nodes[isMax][i]->heuristic;
					}
				}
			}
			else
			{
				if (i != bestmovetemp  )
				{
					if (*nodes[isMax][i]->heuristic < bestHeuristic)
					{
						bestMove = i;
						bestHeuristic = *nodes[isMax][i]->heuristic;
					}
				}
			}
		}
	}
	bestvalue = bestHeuristic;
	bestmovevalue = bestMove;
	DeleteNodes();

	return bestMove;
}

void Minimax::DeleteNodes()
{
	for (int i = 0; i < MaxLevel; i++)
	{
		//myfile << "new level " << i << endl;
		//myfile << levelBegins[isMax][i] << "   " << nodesInLevel[isMax][i] << endl;
		for (int k = levelBegins[isMax][i]; k < levelBegins[isMax][i] + nodesInLevel[isMax][i]; k++)
		{
			if (nodes[isMax][k] != nullptr)
				delete nodes[isMax][k];
		}
	}
	delete nodes[MAX];
	delete nodes[MIN];
	nodes[MAX] = new Game*[NumberOfNodes[MAX]];
	nodes[MIN] = new Game*[NumberOfNodes[MIN]];
}

void Minimax::CreateNodes(const Game & game)
{
	if (isMax)
	{
		//cout << "it is max we are setting the first 4 nodes" << endl;
		nodes[isMax][0] = new Game(game, PlayerLarva, UpRight, true);
		nodes[isMax][1] = new Game(game, PlayerLarva, UpLeft, true);
		nodes[isMax][2] = new Game(game, PlayerLarva, DownRight,true);
		nodes[isMax][3] = new Game(game, PlayerLarva, DownLeft, true);
		//int blocks = 0;
		for (int i = 0; i < 4; i++)
		{
			
			if (!nodes[isMax][i]->gameisValid)
			{
				delete nodes[isMax][i];
				nodes[isMax][i] = nullptr;
				//if (i > 1)
					//blocks++;
			}
			//else
				//nodes[isMax][i]->calculateBlocks(0,0.5);
		}
		/*for (int i = 0; i < 4; i++)
		{
			if (nodes[isMax][i] != nullptr)
				nodes[isMax][i]->blocks = blocks;

		}*/
		
	}
	else
	{
		//cout << "it is min game we are setting the first 8 moves" << endl;
		nodes[isMax][0] = new Game(game, PlayerBird1, UpRight ,true);
		nodes[isMax][1] = new Game(game, PlayerBird1, UpLeft, true);
		nodes[isMax][2] = new Game(game, PlayerBird2, UpRight, true);
		nodes[isMax][3] = new Game(game, PlayerBird2, UpLeft, true);
		nodes[isMax][4] = new Game(game, PlayerBird3, UpRight, true);
		nodes[isMax][5] = new Game(game, PlayerBird3, UpLeft, true);
		nodes[isMax][6] = new Game(game, PlayerBird4, UpRight, true);
		nodes[isMax][7] = new Game(game, PlayerBird4, UpLeft, true);
		for (int i = 0; i < 8; i++)
		{
			if (!nodes[isMax][i]->gameisValid)
			{
				delete nodes[isMax][i];
				nodes[isMax][i] = nullptr;
			}
			
		}
		
	}
	for (int level = 1; level < MaxLevel; level++)
	{
		//cout << "building level " << level << endl;
		int LevelofNodeisMax = MAX;
		if ((level % 2) == isMax)		LevelofNodeisMax = MIN;
		//cout << "level of node is" << (LevelofNodeisMax ? "MAX" : "MIN");
		//for (int i = 0; i<)
		if (LevelofNodeisMax)
		{
			//cout << "level begins at " << levelBegins[isMax][level] << endl;
			for (int i = levelBegins[isMax][level]; i < (levelBegins[isMax][level] + nodesInLevel[isMax][level]); i += 4)
			{
				//cout << "starting at node " << levelBegins[isMax][level] << endl;
				//cout << "i is " << i << endl;
				nodes[isMax][i] = new Game(*getParent(i, level), PlayerLarva, UpRight, true);
				nodes[isMax][i + 1] = new Game(*getParent(i + 1, level), PlayerLarva, UpLeft, true);
				nodes[isMax][i + 2] = new Game(*getParent(i + 2, level), PlayerLarva, DownRight,true);
				nodes[isMax][i + 3] = new Game(*getParent(i + 3, level), PlayerLarva, DownLeft,true);
				//int blocks = 0;
				for (int j = i; j < i + 4; j++)
				{
					if (nodes[isMax][j] != nullptr)
					{
						if (!nodes[isMax][j]->gameisValid)
						{
							/*if (j > i + 2 && nodes[isMax][j]->players[PlayerLarva].PosY != 8)
								blocks++;*/
							delete nodes[isMax][j];
							nodes[isMax][j] = nullptr;
							
						}
						
					}
					
						

				}
				
				
			}
		}
		else
		{
			for (int i = levelBegins[isMax][level]; i < (levelBegins[isMax][level] + nodesInLevel[isMax][level]); i += 8)
			{
				//cout << "level begins at " << levelBegins[isMax][level] << endl;
				nodes[isMax][i] = new Game(*getParent(i, level), PlayerBird1, UpRight, true);
				nodes[isMax][i + 1] = new Game(*getParent(i + 1, level), PlayerBird1, UpLeft, true);
				nodes[isMax][i + 2] = new Game(*getParent(i + 2, level), PlayerBird2, UpRight,true);
				nodes[isMax][i + 3] = new Game(*getParent(i + 3, level), PlayerBird2, UpLeft,true);
				nodes[isMax][i + 4] = new Game(*getParent(i + 4, level), PlayerBird3, UpRight,true);
				nodes[isMax][i + 5] = new Game(*getParent(i + 5, level), PlayerBird3, UpLeft, true);
				nodes[isMax][i + 6] = new Game(*getParent(i + 6, level), PlayerBird4, UpRight, true);
				nodes[isMax][i + 7] = new Game(*getParent(i + 7, level), PlayerBird4, UpLeft,true);

				for (int j = i; j < i + 8; j++)
				{
					if (nodes[isMax][j] != nullptr)
					{
						if (!nodes[isMax][j]->gameisValid)
						{
							delete nodes[isMax][j];
							nodes[isMax][j] = nullptr;
						}
						//else
							//nodes[isMax][j]->calculateBlocks(getParent(j, level)->blocks, level);
					}
				}
			}

		}
	}
}

Game * Minimax::getParent(int node)
{
	assert(node < NumberOfNodes[isMax]);
	//if (node <= nodesInLevel[isMax][0]-1)	return -1;
	int LevelofNode = -1;
	for (int level = MaxLevel - 1; level > -1; level--)
	{
		if (node >= levelBegins[isMax][level]) { LevelofNode = level; break; }
	}
	int LevelofNodeisMax = 1;
	if ((LevelofNode % 2) == isMax)		LevelofNodeisMax = 0;
	//cout << "level of node" << LevelofNode << endl;
	//cout << levelBegins[isMax][LevelofNode] << " + " << " (" << node << " - " << levelBegins[isMax][LevelofNode] << ") / "<< NumberOfPossibleMoves[LevelofNodeisMax] <<" - " << nodesInLevel[isMax][LevelofNode-1] << endl;
	int parent = (levelBegins[isMax][LevelofNode] + (node - levelBegins[isMax][LevelofNode]) / NumberOfPossibleMoves[LevelofNodeisMax] - nodesInLevel[isMax][LevelofNode - 1]);
	return (nodes[isMax][parent] == nullptr) ? nullptr : nodes[isMax][parent];
}

Game * Minimax::getParent(__int64 node, int LevelofNode)
{
	//if (LevelofNode == 0)	return -1;
	int LevelofNodeisMax = 1;
	if ((LevelofNode % 2) == isMax)
		LevelofNodeisMax = 0;
	__int64 parent = (levelBegins[isMax][LevelofNode] + (node - levelBegins[isMax][LevelofNode]) / NumberOfPossibleMoves[LevelofNodeisMax] - nodesInLevel[isMax][LevelofNode - 1]);
	//cout << "parent is " << parent << endl;
	return (nodes[isMax][parent] == nullptr) ? nullptr : nodes[isMax][parent];
}

__int64 Minimax::getParentIndex(__int64 node, int LevelofNode)
{
	int LevelofNodeisMax = 1;
	if ((LevelofNode % 2) == isMax)
		LevelofNodeisMax = 0;
	return (levelBegins[isMax][LevelofNode] + (node - levelBegins[isMax][LevelofNode]) / NumberOfPossibleMoves[LevelofNodeisMax] - nodesInLevel[isMax][LevelofNode - 1]);
}

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

void Minimax::CalculateNumberofNodes()
{
	bool tempTurn = true;
	__int64 sumMax = 0;
	__int64 sumMin = 0;

	levelBegins[MAX][0] = 0;
	levelBegins[MIN][0] = 0;

	sumMax = NumberOfPossibleMoves[MAX];
	sumMin = NumberOfPossibleMoves[MIN];

	nodesInLevel[MAX][0] = sumMax;
	nodesInLevel[MIN][0] = sumMin;
	__int64 levelSumMax = sumMax;
	__int64 levelSumMin = sumMin;


	for (int level = 1; level < MaxLevel; level++)
	{
		levelBegins[MAX][level] = sumMax;
		levelBegins[MIN][level] = sumMin;
		tempTurn = !tempTurn;
		if (tempTurn)
		{
			levelSumMax *= NumberOfPossibleMoves[MAX];
			levelSumMin *= NumberOfPossibleMoves[MIN];
		}
		else
		{
			levelSumMax *= NumberOfPossibleMoves[MIN];
			levelSumMin *= NumberOfPossibleMoves[MAX];
		}
		sumMax += levelSumMax;
		sumMin += levelSumMin;
		nodesInLevel[MAX][level] = levelSumMax;
		nodesInLevel[MIN][level] = levelSumMin;
	}
	NumberOfNodes[MAX] = sumMax;
	NumberOfNodes[MIN] = sumMin;
	//cout << "finished calculating shit" << endl;

}
