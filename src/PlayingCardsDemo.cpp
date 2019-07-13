#include "header.h"


#include "DisplayableObject.h"
#include "BaseEngine.h"

#include "DragableTokenContainer.h"
#include "DragableToken.h"

#include "ImageManager.h"

#include "PlayingCardsDemo.h"
#include "TokenTray.h"

#include "CardOrTile.h"






/*
Initialise things for my specific game/program
*/
int PlayingCardsDemo::virtInitialise()
{
	// These only need to be called once, so I didn't want to put them in functions which are called multiple times, even though I now need to override this method too
	getBackgroundSurface()->setDrawPointsFilter(&m_filterTranslation);
	getForegroundSurface()->setDrawPointsFilter(&m_filterTranslation);

	// Call base class version
	return BaseEngine::virtInitialise();
}


/*
Do any setup of back buffer prior to locking the screen buffer
Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
*/
void PlayingCardsDemo::virtSetupBackgroundBuffer()
{
	fillBackground(0x008000);

	//ImageRenderer r = ImageManager::get()->getImageRendererbyURL("EH\\map_main.jpg");

}


/*
In here you need to create any movable objects that you wish to use.
Sub-classes need to implement this function.
*/
int PlayingCardsDemo::virtInitialiseObjects()
{
	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	drawableObjectsChanged();

	// Destroy any existing objects
	destroyOldObjects(true);

	CardContainer* decks[4];
	decks[0] = new CardContainer(this, 50, 300, "Hearts");
	decks[1] = new CardContainer(this, 800, 300, "Diamonds");
	decks[2] = new CardContainer(this, 50, 600, "Clubs");
	decks[3] = new CardContainer(this, 800, 600, "Spades");

	CardOrTile* cards[52];
	int card = 0;
	for (int x = 0; x < 13; x++)
		for (int y = 0; y < 4; y++)
		{
			cards[card] = new CardOrTile(this, 0, 0, "images\\cards.png", 5 + 147 * x, 44 + 216 * y, 147, 216, "images\\cardback.png", 0, 0, 148, 216, 147, 216, 2/*Scale*/);
			cards[card]->setTransparencyPixel(0, 0);
			decks[y]->addCardToContainer(cards[card]);
			++card;
		}

	// Initially we just need the token trays and card containers (decks)
	createObjectArray(9);

	CardOrTile* backdrop = new CardOrTile(this, 1000, 0, "images\\cards.png");
	backdrop->setTransparencyPixel(0, 0);

	storeObjectInArray(0, backdrop ); // Image of all cards
	storeObjectInArray(1, new TokenTray(this, 400, 300)); 
	storeObjectInArray(2, new TokenTray(this, 400, 600));

	for (int i = 0; i < 4; i++)
		storeObjectInArray(3 + i, decks[i]);
	
	return 0;
}




// Override to handle a mouse press
void PlayingCardsDemo::virtMouseDown(int iButton, int iX, int iY)
{
	switch (iButton)
	{
	case 1: // Left
			//m_filterScaling.compress();
			// Nothing because we are going to click to drag objects anyway
		break;
	case 2: // Middle
			//m_filterScaling.compress();
		break;
	case 3: // Right
			//m_filterScaling.stretch();
		break;
	}
	// Redraw the background
	redrawDisplay(); // Force total redraw
}


void PlayingCardsDemo::virtMouseWheel(int x, int y, int which, int timestamp)
{
	int iOldCentreX = convertRealToVirtualXPosition(this->getWindowWidth() / 2);
	int iOldCentreY = convertRealToVirtualXPosition(this->getWindowHeight() / 2);

	if (y < 0)
		m_filterScaling.compress();
	else if (y > 0)
		m_filterScaling.stretch();

	int iNewCentreX = convertRealToVirtualXPosition(this->getWindowWidth() / 2);
	int iNewCentreY = convertRealToVirtualXPosition(this->getWindowHeight() / 2);

	//float fNewZoomX = m_filterScaling.getZoomX();
	//float fNewZoomY = m_filterScaling.getZoomY();
	//fNewZoomX = fNewZoomY = 1.0f;

	//std::cout << "Zoom from x " << std::to_string(iOldCentreX) << " to " << std::to_string(iNewCentreX)
	//	<< ", y " << std::to_string(iOldCentreY) << " to " << std::to_string(iNewCentreY)
	//	<< ", shift by " << std::to_string(iNewCentreX - iOldCentreX) << "," << std::to_string(iNewCentreY - iOldCentreY)
	//	<< std::endl;

	// Apply a translation to offset so it appears to have zoomed on the centre.
	m_filterTranslation.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);

	// Redraw the background
	redrawDisplay(); // Force total redraw
}

/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is currently pressed
*/
void PlayingCardsDemo::virtKeyDown(int iKeyCode)
{
	switch (iKeyCode)
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		setExitWithCode(0);
		break;
	case SDLK_LEFT:
		m_filterTranslation.changeOffset(10, 0);
		redrawDisplay();
		break;
	case SDLK_RIGHT:
		m_filterTranslation.changeOffset(-10, 0);
		redrawDisplay();
		break;
	case SDLK_UP:
		m_filterTranslation.changeOffset(0, 10);
		redrawDisplay();
		break;
	case SDLK_DOWN:
		m_filterTranslation.changeOffset(0, -10);
		redrawDisplay();
		break;
	case SDLK_SPACE:
		m_filterTranslation.setOffset(0, 0);
		redrawDisplay();
		break;
	default:
		redrawDisplay();
		break;
	}
}

/* Override this to handle a key being released.
Note that the fact that the key has been released will have already been stored.
*/
void PlayingCardsDemo::virtKeyUp(int iKeyCode)
{
	redrawDisplay();
}
