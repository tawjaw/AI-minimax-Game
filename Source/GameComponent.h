/*
	============================================================
		GameComponent.h
		Author: Tawfiq Jawhar
		17-10-2015
		this class handles the GUI and controls the game
	============================================================
*/


#ifndef GameComponent_H_INCLUDED
#define GameComponent_H_INCLUDED
#include <iostream>
#include "../JuceLibraryCode/JuceHeader.h"
#include "Common.h"
#include "Game.h"
#include <iostream>

using namespace std;


class GameComponent  : public Component, public ButtonListener
{
public:
	GameComponent(String name = "gameBoard")
        : Component (name)
    {
		
		
		//create button and add listener
		StartButton = new TextButton("START");
		StartButton->setBounds(800, 200, 100, 100);
		StartButton->setColour(0x1000100, Colours::brown);
		StartButton->addListener(this);
		setMainMenuGraphics();
		addAndMakeVisible(StartButton);
		
		game = nullptr;
		larvaSelected = nullptr;
		birdSelected = nullptr;
		
    }
	
	~GameComponent()
	{
		deleteAllChildren();
		delete game;
	}
	void setMainMenuGraphics()
	{
		//set background images	
		grass = ImageCache::getFromMemory(BinaryData::grass_jpg, BinaryData::grass_jpgSize);
		wallpaper_Start = ImageCache::getFromMemory(BinaryData::wallpaper2_jpg, BinaryData::wallpaper2_jpgSize);

		assert(grass.isValid());
		assert(wallpaper_Start.isValid());
		p_larvaHuman.image = ImageCache::getFromMemory(BinaryData::larva_png, BinaryData::larva_pngSize);
		p_larvaAI.image = ImageCache::getFromMemory(BinaryData::larvaAI_png, BinaryData::larvaAI_pngSize);
		p_BirdHuman.image = ImageCache::getFromMemory(BinaryData::bird1_png, BinaryData::bird1_pngSize);
		p_BirdAI.image = ImageCache::getFromMemory(BinaryData::bird1AI_png, BinaryData::bird1AI_pngSize);
		p_larvaHuman.PosX = 2;
		p_larvaAI.PosX = 2;
		p_BirdHuman.PosX = 6;
		p_BirdAI.PosX = 6;
		p_larvaHuman.PosY = 2;
		p_larvaAI.PosY = 6;
		p_BirdHuman.PosY = 2;
		p_BirdAI.PosY = 6;

	}
    void resized() override {  }

	void paint(Graphics& g)
	{
		
		g.drawImageWithin(wallpaper_Start, 0, 0, 1000, 700, RectanglePlacement(128));
	
		int stepX = 600 / numX;
		int stepY = 600 / numY;
		
		//__________MAIN MENU___________
		if (game == nullptr || game->state == MAINMENU )
		{
			g.drawImageWithin(p_larvaHuman.image, p_larvaHuman.PosX*stepX, p_larvaHuman.PosY*stepY, 100, 100, RectanglePlacement(128));
			g.drawImageWithin(p_larvaAI.image, p_larvaAI.PosX*stepX, p_larvaAI.PosY*stepY, 100, 100, RectanglePlacement(128));
			g.drawImageWithin(p_BirdHuman.image, p_BirdHuman.PosX*stepX, p_BirdHuman.PosY*stepY, 100, 100, RectanglePlacement(128));
			g.drawImageWithin(p_BirdAI.image, p_BirdAI.PosX*stepX, p_BirdAI.PosY*stepY, 100, 100, RectanglePlacement(128));

			if(birdSelected != nullptr)
				g.drawRect(birdSelected->PosX*stepX-25, birdSelected->PosY*stepY-25 , 150, 150, 5);

			if (larvaSelected != nullptr)
				g.drawRect(larvaSelected->PosX*stepX-25 , larvaSelected->PosY*stepY-25 , 150, 150, 5);
			
			StartButton->setButtonText("Start");
			StartButton->setVisible(true);
			if(larvaSelected != nullptr && birdSelected != nullptr)
				StartButton->setEnabled(true);
			else
				StartButton->setEnabled(false);

		}

		//____________START_____________
		else if (game->state == START)
		{
			
			StartButton->setButtonText("RESET");
			
			//draw play grid
			
			g.drawImageWithin(grass, 50, 50, 600, 600, RectanglePlacement(64));
			g.setColour(Colours::darkslategrey);
			g.drawLine(50, 52, 653,  52, 6.0f);
			g.drawLine(45, 652, 653,652, 6.0f);
			g.drawLine(48, 49, 48, 650, 6.0f);
			g.drawLine(650, 50, 650, 650, 6.0f);

			
		
			//draw players
			g.drawImageWithin(game->players[PlayerLarva].image, game->players[PlayerLarva].PosX*stepX - 12, game->players[PlayerLarva].PosY*stepY - 12, 50, 50, RectanglePlacement(128));		
			g.drawImageWithin(game->players[PlayerBird1].image, game->players[PlayerBird1].PosX*stepX - 12, game->players[PlayerBird1].PosY*stepY - 12, 50, 50, RectanglePlacement(128));
			g.drawImageWithin(game->players[PlayerBird2].image, game->players[PlayerBird2].PosX*stepX - 12, game->players[PlayerBird2].PosY*stepY - 12, 50, 50, RectanglePlacement(128));
			g.drawImageWithin(game->players[PlayerBird4].image, game->players[PlayerBird4].PosX*stepX - 12, game->players[PlayerBird4].PosY*stepY - 12, 50, 50, RectanglePlacement(128));
			g.drawImageWithin(game->players[PlayerBird3].image, game->players[PlayerBird3].PosX*stepX - 12, game->players[PlayerBird3].PosY*stepY - 12, 50, 50, RectanglePlacement(128));
			
		
			//draw box around selected bird if player is human
			g.setColour(Colours::black);
			if (game->BirdSelected != nullptr)
			{
				g.drawRect(game->BirdSelected->PosX*stepX - 12, game->BirdSelected->PosY*stepY - 12, 50, 50, 5);

			}
			
			//check if game ends and change state to END
			if (game->isWon())
			{
				g.drawText("WON", 500, 400, 500, 100, Justification::centred, true);
				game->state = END;
				StartButton->setButtonText("Reset");
				//StartButton->setEnabled(true);
				//StartButton->setVisible(true);
			}
			else if (game->isGameOver())
			{
				g.drawText("GAME OVER HAHAH", 500, 400, 500, 100, Justification::centred, true);
				game->state = END;
				StartButton->setButtonText("Reset");
				//StartButton->setEnabled(true);
				//StartButton->setVisible(true);
			}
			else if (game->LarvaState == LarvaAI && game->turn == LARVA)
			{
				game->MouseClick(0, 0, *minimax); repaint();
			}
			else if (game->BirdState == BirdAI && game->turn == BIRD)
			{
				game->MouseClick(0, 0, *minimax); repaint();
			}
			g.drawText(String(minimax->bestvalue) +"  "+ String(minimax->bestmovevalue), 500, 400, 500, 100, Justification::centred, true);
		}

		//_____________END______________
		else if (game->state == END)
		{
			g.drawImageWithin(game->players[PlayerLarva].image, game->players[PlayerLarva].PosX*stepX - 12, game->players[PlayerLarva].PosY*stepY - 12, 50, 50, RectanglePlacement(128));
			g.drawImageWithin(game->players[PlayerBird1].image, game->players[PlayerBird1].PosX*stepX - 12, game->players[PlayerBird1].PosY*stepY - 12, 50, 50, RectanglePlacement(128));
			g.drawImageWithin(game->players[PlayerBird2].image, game->players[PlayerBird2].PosX*stepX - 12, game->players[PlayerBird2].PosY*stepY - 12, 50, 50, RectanglePlacement(128));
			g.drawImageWithin(game->players[PlayerBird4].image, game->players[PlayerBird4].PosX*stepX - 12, game->players[PlayerBird4].PosY*stepY - 12, 50, 50, RectanglePlacement(128));
			g.drawImageWithin(game->players[PlayerBird3].image, game->players[PlayerBird3].PosX*stepX - 12, game->players[PlayerBird3].PosY*stepY - 12, 50, 50, RectanglePlacement(128));

		}
		

	}

	/*
		Mouse listeners
	*/
	void mouseEnter(const MouseEvent&) override
	{
		
	}
	void mouseDown(const MouseEvent& e) override
	{
		lastMousePosition = e.position;
		mouseDrag(e);
		dragging = true;
	}

	void mouseDrag(const MouseEvent& e) override
	{
		dragging = true;

		if (e.position != lastMousePosition)
		{
			// calculate movement vector

			lastMousePosition = e.position;
		}
	}

	void mouseUp(const MouseEvent& e) override
	{
		bool toRepaint = false;
		if (dragging == true)
		{
			if (e.getMouseDownX() > 50 && e.getMouseDownY() > 50)
			{
				mouseX = (e.getMouseDownX() - 50) / 75 +1;
				mouseY = (e.getMouseDownY() - 50) / 75 +1;
			}
			else
			{
				mouseX = -1;
				mouseY = -1;
			}
			if (game == nullptr || game->state == MAINMENU)
			{
				toRepaint = selectPlayer(mouseX, mouseY);
			}
			else if(game->state == START)
				toRepaint = game->MouseClick( mouseX, mouseY,* minimax);

		}
		dragging = false;
		//isOn = true;
		if(toRepaint)
			repaint();
	}

	bool selectPlayer(int mouseX, int mouseY)
	{
		
		bool repaint = true;
		if (mouseX <= 8 && mouseX >= 6 && mouseY <= 8 && mouseY >=6)
			birdSelected = &p_BirdAI;
		else if (mouseX <= 8 && mouseX >= 6 && mouseY <= 4 && mouseY >=2)
			birdSelected = &p_BirdHuman;
		else if (mouseX <= 4 && mouseX >= 2 && mouseY <= 4 && mouseY >=2)
			larvaSelected = &p_larvaHuman;
		else if (mouseX <= 4 && mouseX >= 2 && mouseY <= 8 && mouseY >=6)
			larvaSelected = &p_larvaAI;
		else repaint = false;

		return repaint;
	}
	//button listener 
	void buttonClicked(Button* button)
	{
		if (button == StartButton)
		{
			if (game == nullptr)
			{
				if (larvaSelected != nullptr && birdSelected != nullptr)
				{
					PLAYERSTATE larvastate;
					PLAYERSTATE birdstate;
					if (birdSelected == &p_BirdAI)
						birdstate = BirdAI;
					else
						birdstate = BirdHuman;

					if (larvaSelected == &p_larvaAI)
						larvastate = LarvaAI;
					else
						larvastate = LarvaHuman;

					minimax = new Minimax(8);
					game = new Game(larvastate, birdstate);
					game->state = START;
					repaint();
				}
			}
			else if (game->state == MAINMENU)
			{
				game->state = START;
				repaint();
			}
			else if (game->state == START)
			{
				game->Initialize();
				repaint();
			}
			else if (game->state == END)
			{
				game->Initialize();
				game->state = START;
				repaint();
			}
		}
	}



private:
    // member variables for the Component
    static const int numX = 8;
    static const int numY = 8;
	
	Point<float> lastMousePosition;
	bool dragging;
    //ToggleLightComponent toggleLights [numX * numY];
	
	Image grass;
	Image wallpaper_Start;
	
	int mouseX = 0;
	int mouseY = 0;

	TextButton* StartButton;

	Game* game;
	
	Minimax* minimax;
	
	Player p_larvaHuman;
	Player p_larvaAI;
	Player p_BirdHuman;
	Player p_BirdAI;
	Player* larvaSelected;
	Player* birdSelected;
	//JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameComponent)
};



#endif  // TOGGLELIGHTGRIDCOMPONENT_H_INCLUDED
