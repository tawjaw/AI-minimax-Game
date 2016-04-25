/*
  ==============================================================================

    Game.cpp
    Created: 16 Nov 2015 8:46:40pm
    Author:  Tawfiq Jawhar

  ==============================================================================
*/

#include "Game.h"


Game::Game(PLAYERSTATE larvastate, PLAYERSTATE birdstate)
  {
	  //set players states	Human or AI
	  heuristic = nullptr;
	  LarvaState = larvastate; BirdState = birdstate;
	  gameisValid = true;
	  GameOver = false;
	  //set players images
	  if (LarvaState == LarvaHuman)
		  players[PlayerLarva].image = ImageCache::getFromMemory(BinaryData::larva_png, BinaryData::larva_pngSize);
	  else if (LarvaState == LarvaAI)
		  players[PlayerLarva].image = ImageCache::getFromMemory(BinaryData::larvaAI_png, BinaryData::larvaAI_pngSize);
	  else	assert(false);	//wrong state of larva

	  if (BirdState == BirdHuman)
	  {
		  players[PlayerBird1].image = ImageCache::getFromMemory(BinaryData::bird1_png, BinaryData::bird1_pngSize);
		  players[PlayerBird2].image = ImageCache::getFromMemory(BinaryData::bird2_png, BinaryData::bird2_pngSize);
		  players[PlayerBird3].image = ImageCache::getFromMemory(BinaryData::bird3_png, BinaryData::bird3_pngSize);
		  players[PlayerBird4].image = ImageCache::getFromMemory(BinaryData::bird4_png, BinaryData::bird4_pngSize);
	  }
	  else if (BirdState == BirdAI)
	  {
		  players[PlayerBird1].image = ImageCache::getFromMemory(BinaryData::bird1AI_png, BinaryData::bird1AI_pngSize);
		  players[PlayerBird2].image = ImageCache::getFromMemory(BinaryData::bird2AI_png, BinaryData::bird2AI_pngSize);
		  players[PlayerBird3].image = ImageCache::getFromMemory(BinaryData::bird3AI_png, BinaryData::bird3AI_pngSize);
		  players[PlayerBird4].image = ImageCache::getFromMemory(BinaryData::bird4AI_png, BinaryData::bird4AI_pngSize);
	  }
	  else	assert(false);	//wrong state of bird

							//make sure images are valid
	  assert(players[PlayerLarva].image.isValid());
	  assert(players[PlayerBird1].image.isValid());
	  assert(players[PlayerBird2].image.isValid());
	  assert(players[PlayerBird3].image.isValid());
	  assert(players[PlayerBird4].image.isValid());

	  state = MAINMENU;
	  //initialize game
	  Initialize();



  }

 Game::Game(const Game & other, int player, int move, bool setgameover)
{
	heuristic = nullptr;
	
	if (&other == nullptr || other.GameOver)
		gameisValid = false;
	else if (other.gameisValid)
	{
		players[PlayerLarva] = other.players[PlayerLarva];
		players[PlayerBird1] = other.players[PlayerBird1];
		players[PlayerBird2] = other.players[PlayerBird2];
		players[PlayerBird3] = other.players[PlayerBird3];
		players[PlayerBird4] = other.players[PlayerBird4];
		MovePlayer(player, move);
		if(setgameover)
			if (isGameOver() || isWon()) { GameOver = true; CalculateHeuristic(); }

	}
	else
		gameisValid = false;
}


 void Game::Initialize()
 {
	 //set initial positions of players

	 players[PlayerLarva].PosY = 7; players[PlayerLarva].PosX = 4;
	 players[PlayerBird1].PosY = 8; players[PlayerBird1].PosX = 1;
	 players[PlayerBird2].PosY = 8; players[PlayerBird2].PosX = 3;
	 players[PlayerBird3].PosY = 8; players[PlayerBird3].PosX = 5;
	 players[PlayerBird4].PosY = 8; players[PlayerBird4].PosX = 7;

	 //larva stats the game
	 turn = LARVA;
	 //no bird is selected
	 BirdSelected = NULL;

 }

 bool Game::MouseClick(int mouseX, int mouseY, Minimax& minimax)
 {
	 bool repaint = false;
	 if (mouseX >= 0 && mouseX < 9 && mouseY >= 0 && mouseY < 9)
	 {

		 if (turn == LARVA)
		 {
			 if (LarvaState == LarvaHuman)
			 {
				 bool goodLarvaMove = false;
				 if ((mouseX == players[PlayerLarva].PosX - 1 && mouseY == players[PlayerLarva].PosY - 1)
					 || (mouseX == players[PlayerLarva].PosX + 1 && mouseY == players[PlayerLarva].PosY - 1)
					 || (mouseX == players[PlayerLarva].PosX - 1 && mouseY == players[PlayerLarva].PosY + 1)
					 || (mouseX == players[PlayerLarva].PosX + 1 && mouseY == players[PlayerLarva].PosY + 1))
					 goodLarvaMove = true;

				 if (goodLarvaMove)
				 {
					 if (!Collision(mouseX, mouseY))
					 {
						 players[PlayerLarva].PosX = mouseX;
						 players[PlayerLarva].PosY = mouseY;
						 turn = BIRD;
						 repaint = true;
					 }
				 }
			 }
			 else if (LarvaState == LarvaAI)
			 {
				 //TODO let the AI handle this
				 int bestmove = minimax.getBestNextMove(*this);
				 assert(bestmove > -1);
				 MovePlayer(PlayerLarva, bestmove + 1);
				// repaint = true;
				 turn = BIRD;
			 }
			 else
				 assert(false);	//player1 can either be human or AI larva
		 }
		 else
		 {
			 if (BirdState == BirdHuman)
			 {
				 if (Collision(mouseX, mouseY))
				 {
					 BirdSelected = findBird(mouseX, mouseY);
					 repaint = true;
				 }
				 else
				 {
					 if (BirdSelected != nullptr)
					 {
						 if (MovePlayer(BirdSelected, mouseX, mouseY))
						 {
							 turn = LARVA;
							 BirdSelected = nullptr;
							 repaint = true;
						 }
					 }
				 }
			 }
			 else if (BirdState == BirdAI)
			 {
				 //TODO let AI handle this 
				 int bestmove = minimax.getBestNextMove(*this);
				// assert(bestmove > -1);
				 MovePlayer(bestmove/2 +1, bestmove%2 + 1);
				// repaint = true;
				 turn = LARVA;
			 }
			 else
				 assert(false);	//player 2 should only be Bird
		 }

	 }
	 return repaint;
 }

 //move bird if no collision with other players

 bool Game::MovePlayer(Player * bird, int mouseX, int mouseY)
 {
	 bool goodBirdMove = false;
	 if ((mouseX == bird->PosX - 1 && mouseY == bird->PosY - 1)
		 || (mouseX == bird->PosX + 1 && mouseY == bird->PosY - 1))
		 goodBirdMove = true;

	 if (goodBirdMove)
	 {
		 if (!Collision(mouseX, mouseY))
		 {
			 bird->PosX = mouseX;
			 bird->PosY = mouseY;
			 return true;
		 }
	 }
	 return false;
 }

 void Game::MovePlayer(int player, int move)
 {
	 assert(player < 5);
	 gameisValid = true;
	 GameOver = false;
	 switch (move)
	 {
	 case UpRight:
		 if (!Collision(players[player].PosX + 1, players[player].PosY - 1))
		 {
			 players[player].PosX++;
			 players[player].PosY--;
		 }
		 else
			 gameisValid = false;
		 break;
	 case UpLeft:
		 if (!Collision(players[player].PosX - 1, players[player].PosY - 1))
		 {
			 players[player].PosX--;
			 players[player].PosY--;
		 }
		 else
			 gameisValid = false;
		 break;
	 case DownRight:
		 if (!Collision(players[player].PosX + 1, players[player].PosY + 1))
		 {
			 players[player].PosX++;
			 players[player].PosY++;
		 }
		 else
			 gameisValid = false;
		 break;
	 case DownLeft:
		 if (!Collision(players[player].PosX - 1, players[player].PosY + 1))
		 {
			 players[player].PosX--;
			 players[player].PosY++;
		 }
		 else
			 gameisValid = false;
		 break;
	 default:
		 assert(false);	//wrong value
	 }
 }
 //find the bird selected

 Player * Game::findBird(int mouseX, int mouseY)
 {
	 if (players[PlayerBird1].PosX == mouseX && players[PlayerBird1].PosY == mouseY)
		 return &players[PlayerBird1];
	 else if (players[PlayerBird2].PosX == mouseX && players[PlayerBird2].PosY == mouseY)
		 return &players[PlayerBird2];
	 else if (players[PlayerBird3].PosX == mouseX && players[PlayerBird3].PosY == mouseY)
		 return &players[PlayerBird3];
	 else if (players[PlayerBird4].PosX == mouseX && players[PlayerBird4].PosY == mouseY)
		 return &players[PlayerBird4];
	 else
		 return nullptr;
 }

 bool Game::isGameOver()
 {

	 if (Collision(players[PlayerLarva].PosX - 1, players[PlayerLarva].PosY - 1)
		 && Collision(players[PlayerLarva].PosX + 1, players[PlayerLarva].PosY - 1)
		 && Collision(players[PlayerLarva].PosX - 1, players[PlayerLarva].PosY + 1)
		 && Collision(players[PlayerLarva].PosX + 1, players[PlayerLarva].PosY + 1))
		 return true;
	 else return false;
 }

 bool Game::isWon()
 {
	 bool iswon = false;
	 Game** games = new Game*[8];
	 games[0] = new Game(*this, PlayerBird1, UpRight, false);
	 games[1] = new Game(*this, PlayerBird1, UpLeft, false);
	 games[2] = new Game(*this, PlayerBird2, UpRight, false);
	 games[3] = new Game(*this, PlayerBird2, UpLeft, false);
	 games[4] = new Game(*this, PlayerBird3, UpRight, false);
	 games[5] = new Game(*this, PlayerBird3, UpLeft, false);
	 games[6] = new Game(*this, PlayerBird4, UpRight, false);
	 games[7] = new Game(*this, PlayerBird4, UpLeft, false);
	 int counterbird = 0;
	 for (int i = 0; i < 8; i++)
	 {
		 if (!games[i]->gameisValid)
			 counterbird++;
	 }
	 if (counterbird == 8) iswon = true;

	 for (int i = 0; i < 8; i++)
		 delete games[i];
	 delete games;

	 
	 return (iswon || (players[PlayerLarva].PosY == 8) ? true : false);
 }

 //returns true if the next movement collides with any of the players

 bool Game::Collision(int mouseX, int mouseY)
 {
	 if (mouseX > 0 && mouseX < 9 && mouseY > 0 && mouseY < 9)
	 {
		 if ((mouseX == players[PlayerLarva].PosX && mouseY == players[PlayerLarva].PosY)
			 || (mouseX == players[PlayerBird1].PosX && mouseY == players[PlayerBird1].PosY)
			 || (mouseX == players[PlayerBird2].PosX && mouseY == players[PlayerBird2].PosY)
			 || (mouseX == players[PlayerBird3].PosX && mouseY == players[PlayerBird3].PosY)
			 || (mouseX == players[PlayerBird4].PosX && mouseY == players[PlayerBird4].PosY))
			 return true;
		 else return false;
	 }
	 else return true;
 }
 int Game::getPosition(int player)
 {
	 return (players[player].PosY * 8 - 8 + players[player].PosX);
 }
 int Game::collisionLarvawithBird(int bird)
 {
	 if (players[bird].PosY == players[PlayerLarva].PosY - 1 && (players[bird].PosX == players[PlayerLarva].PosX - 1 || players[bird].PosX == players[PlayerLarva].PosY + 1))
		 return 1;
	 if (players[bird].PosY == players[PlayerLarva].PosY + 1 && (players[bird].PosX == players[PlayerLarva].PosX - 1 || players[bird].PosX == players[PlayerLarva].PosY + 1))
		 return 2;
 }
 
 void Game::CalculateHeuristic()
 {
	 //TODO
	 //calculate the hruristic
	 int temp = 0;
	// temp -=  * 1000;
	// temp = blocks*0.1;
	 temp += (1 + players[PlayerLarva].PosY)*(8 - players[PlayerLarva].PosY + 1);
	 int countAboveLarva = 0;
	 int countBirdsSameLine = 0;
	 for (int i = 1; i < 5; i++)
	 {
		 if (players[i].PosY > players[PlayerLarva].PosY)
		 {
			 temp += -(1 + players[i].PosY)*(8 - players[i].PosY + 1);//* 10;
			 countAboveLarva++;
		 }
		 for (int j = 1; j < 5 && j != i; j++)
			 if (players[i].PosX == players[j].PosX && players[i].PosY != 8)
				 countBirdsSameLine++;

	 }
	 if (countAboveLarva >= 3)
		 temp += - 100;
	 else if (countAboveLarva <=2)
		 temp += 100;
	 
	 if (countBirdsSameLine >= 9)
		 temp += -100;
	// else
		// temp += 100;
	

	 if (countAboveLarva == 4 && countBirdsSameLine == 12) 		 
		 temp += -200;
	

//	 temp += (players[PlayerLarva].PosY + 1)*100;
		// temp += 500;
	// temp -= countBirdsSameLine*10;
//	 temp += -(1 + players[PlayerBird1].PosY)*(8 - players[PlayerBird1].PosY + 1) - (1 + players[PlayerBird2].PosY)*(8 - players[PlayerBird2].PosY + 1) - (1 + players[PlayerBird3].PosY)*(8 - players[PlayerBird3].PosY + 1) - (1 + players[PlayerBird4].PosY)*(8 - players[PlayerBird4].PosY + 1)) * 10;

	 //temp += players[PlayerLarva].PosY;
	 /*if (isWon() && turn == LARVA)
		 temp = temp + 10000;
	 else if(isWon() && turn == BIRD)
		 temp = temp + 1000;
	 else if (isGameOver() && turn == BIRD)
		 temp = temp - 1000;
	 else if (isGameOver() && turn == LARVA)
		 temp = temp - 10000;
	 else
		 temp = 0;//rand() % 1000;*/
	 if (isWon())
		 temp = 100000;
	 if (isGameOver())
		 temp = -100000;
	 
	 //temp += (2+players[PlayerLarva].PosY)*100;
	/* for (int i = 1; i < 5; i++)
	 {
		 temp += (players[i].PosY+1)*(8-players[i].PosY+1) + (players[PlayerLarva].PosY+1)*(8+players[PlayerLarva].PosY+1);
	 }*/
	 heuristic = new int(temp);
	 //cout << temp << endl;
 }
