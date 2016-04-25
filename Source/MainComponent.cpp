/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
	: gameComponent ("gameBoard")
{
    setSize (1000, 700);
	setBoundsToFit(0, 0, 1000, 700, Justification::centred, true);
	
	addAndMakeVisible(gameComponent);
	
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
}

void MainContentComponent::resized()
{
	gameComponent.setBounds(getLocalBounds());
}
