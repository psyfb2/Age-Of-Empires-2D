#pragma once


#include "BaseEngine.h"
#include "DragableObject.h"
#include "DrawingFilters.h" 

/*
This demo includes:
Playing cards
Tokens
Token containers
Images
Dragging
Decks of cards / card containers.
*/
class PlayingCardsDemo :
	public BaseEngine
{
public:
	PlayingCardsDemo()
		: m_filterScaling(0, 0, this), m_filterTranslation(0, 0, &m_filterScaling)
	{
		this->notifyTopObjectAboutMouseClick(true);
		m_bBlockWaitingForEvents = true;
	}

	~PlayingCardsDemo()
	{}

	// Initialise things for my specific game/program
	int virtInitialise();

	// Do any setup of back buffer prior to locking the screen buffer
	// Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
	virtual void virtSetupBackgroundBuffer() override;

	// Create any moving objects
	int virtInitialiseObjects() override;

	// Handle pressing of a mouse button
	void virtMouseDown(int iButton, int iX, int iY) override;

	// Handle pressing of a key
	virtual void virtKeyDown(int iKeyCode) override;
	virtual void virtKeyUp(int iKeyCode) override;

	// Handle mouse wheel
	virtual void virtMouseWheel(int x, int y, int which, int timestamp) override;

protected:
	// Note that static members are only initialised once, so this only creates the one object of each type, and keeps a pointer to it
	FilterPointsScaling m_filterScaling;
	FilterPointsTranslation m_filterTranslation;

};
