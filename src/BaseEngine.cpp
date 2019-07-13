#include "header.h"

#if defined(_MSC_VER)
#include <SDL_syswm.h>
#endif

#include <math.h>	

#include "BaseEngine.h"
#include "DisplayableObject.h"

#include "templates.h"





/*
Constructor.
*/
BaseEngine::BaseEngine()
: m_pKeyStatus(NULL)
, m_iExitWithCode(-1)
, m_iTick(0)
, m_iNextTickToActOn(0)
, m_pDefaultFont(NULL)
, m_iMouseXClickedDown(0)
, m_iMouseYClickedDown(0)
, m_iMouseXClickedUp(0)
, m_iMouseYClickedUp(0)
, m_iCurrentMouseX(0)
, m_iCurrentMouseY(0)
, m_iCurrentButtonStates(0)
, m_iTimeOffset(0)
, m_oBackgroundSurface(this)
, m_oForegroundSurface(this)
, m_iPaused(-1)
, m_bNotifyObjectsAboutKeys(false)
, m_bNotifyObjectsAboutMouse(false)
, m_bNotifyTopObjectAboutMouse(false)
, m_bNotifyBottomObjectAboutMouse(false)
, m_bBlockWaitingForEvents(false) // don't wait for events before continuing each loop
, m_bNeedsRedraw(true)
, m_bTotalRedraw(true)
, m_iRedrawVirtualLeft(INT_MAX / 2)
, m_iRedrawVirtualTop(INT_MAX / 2)
, m_iRedrawVirtualRight(-INT_MAX / 2)
, m_iRedrawVirtualBottom(-INT_MAX / 2)
, m_iRedrawRealLeft(INT_MAX / 2)
, m_iRedrawRealTop(INT_MAX / 2)
, m_iRedrawRealRight(-INT_MAX / 2)
, m_iRedrawRealBottom(-INT_MAX / 2)
{
}


/*
Destructor - destroys moving objects
*/
BaseEngine::~BaseEngine(void)
{
}



extern "C" void mydebug()
{
	puts("Ended normally");
}

/*
Initialise SDL and create a window of the correct size.
*/ 
int BaseEngine::initialise(const char* strCaption, int iWindowWidth, int iWindowHeight, const char* szFontName, int iFontSize)
{
	m_iWindowWidth = iWindowWidth;
	m_iWindowHeight = iWindowHeight;

	// Initialize SDL's subsystems
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}

	atexit(mydebug);

	// Register SDL_Quit to be called at exit; makes sure things are
	// cleaned up when we quit.
	atexit(SDL_Quit);

	// True type font stuff
	TTF_Init();
	atexit(TTF_Quit);

	m_oFontManager.loadFonts();

	// Load the default font - used whenever NULL is passed
	if ( ( szFontName != NULL ) && ( strlen(szFontName) > 0 ) )
		m_pDefaultFont = m_oFontManager.getFont( szFontName, iFontSize );

	// SDL2 replacement for the SDL1.2 stuff, to make the surfaces still work with software rendering, so it is still a bitmapped display
	// WARNING: This is definitely not the fastest performance way to do this by any means, but it means that the bitmapped display will
	// still work and all of the older SDL1.2 drawing code that I had still works without changes.
	m_pSDL2Window = SDL_CreateWindow( strCaption, 
			10,40, /* If you hardcode these, allow room for the window border and title bar */
			//SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // Use this to let it position the window for you.
			m_iWindowWidth, m_iWindowHeight,
			0/*SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL*/ );
	m_pSDL2Renderer = SDL_CreateRenderer( m_pSDL2Window,-1,0 );
	m_pSDL2Texture = SDL_CreateTexture( m_pSDL2Renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,m_iWindowWidth, m_iWindowHeight);

	// Foreground surface, for the moving objects on top of background. This is basically the window content.
	m_oForegroundSurface.createSurface(m_iWindowWidth, m_iWindowHeight);
	// Background surface, for the static background stuff. This is the same size as the window and is used for writing over the window contents to undraw objects.
	m_oBackgroundSurface.createSurface(m_iWindowWidth, m_iWindowHeight);


	// Initialise key status
	m_pKeyStatus = new bool[1 + SDL12_SDLK_LAST];
	memset( m_pKeyStatus,0,sizeof( bool )*(1 + SDL12_SDLK_LAST) );

	// Call method to allow custom sub-class initialiation.
	virtInitialise();

	return 0; // success
}


/*
Initialise game/program-specific things.
Base class just calls InitialiseObjects() to create the moving objects
*/
int BaseEngine::virtInitialise()
{
	// Create the moving objects
	virtInitialiseObjects();
	// Set up the initial background
	virtSetupBackgroundBuffer();

	return 0; // success
}


/*
In here you need to create any movable objects that you wish to use.
Sub-classes need to implement this function.
You can use the CreateObjectArray() and StoreObjectInArray() functions to create it.
*/
int BaseEngine::virtInitialiseObjects()
{
	// Destroy any existing objects
	destroyOldObjects(true);

	// Create an array one element larger than the number of objects that you want.
	// e.g.: m_ppDisplayableObjects = new DisplayableObject*[MAXNUMBER+1];
	// or CreateObjectArray(MAXNUMBER);

	// You MUST set the array entry after the last one that you create to NULL, so that the system knows when to stop.
	//e.g. m_ppDisplayableObjects[0] = new MyDisplayableObject( this, m_iPlayer1StartX, m_iPlayer1StartY);
	//e.g. m_ppDisplayableObjects[1] = new MyDisplayableObject( this, m_iPlayer2StartX, m_iPlayer2StartY);
	// or StoreObjectInArray( 0, new MyDisplayableObject( this, m_iPlayer1StartX, m_iPlayer1StartY);
	// or StoreObjectInArray( 1, new MyDisplayableObject( this, m_iPlayer2StartX, m_iPlayer2StartY);

	// i.e. The LAST entry has to be NULL. The fact that it is NULL is used in order to work out where the end of the array is.
	//e.g. m_ppDisplayableObjects[2] = NULL;
	// Note that CreateObjectArray will have zero'ed the last element (NULL) but you can set it if you wish

	// You should return the number of objects which you created.
	// e.g. return 2;

	return 0;
}








// Handle an SDL event
bool BaseEngine::handleEvent(SDL_Event* pEvent)
{
	switch (pEvent->type)
	{
	case SDL_KEYDOWN:
		//printf("Key down: %d, '%c'\n", SymbolToKeycode(pEvent->key.keysym.sym), pEvent->key.keysym.sym);
		m_pKeyStatus[symbolToKeycode(pEvent->key.keysym.sym)] = true;
		virtKeyDown(pEvent->key.keysym.sym);
		if (m_bNotifyObjectsAboutKeys)
			std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=](DisplayableObject* pdo) { if (pdo == nullptr) return; pdo->virtKeyDown(pEvent->key.keysym.sym); });
		break;

	case SDL_KEYUP:
		//printf("Key up: %d, '%c'\n", SymbolToKeycode(pEvent->key.keysym.sym), pEvent->key.keysym.sym);
		m_pKeyStatus[symbolToKeycode(pEvent->key.keysym.sym)] = false;
		virtKeyUp(pEvent->key.keysym.sym);
		if (m_bNotifyObjectsAboutKeys)
			std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=](DisplayableObject* pdo) { if (pdo == nullptr) return; pdo->virtKeyUp(pEvent->key.keysym.sym); });
		break;

	case SDL_MOUSEMOTION:
		// Note: If you are not using a filter this just assigns the motion.x to the m_iCurrentMouseX. If using a filter it may convert the point. See filter at top of DrawingSurface if you really need to know what this is doing.
		m_iCurrentMouseX = m_oForegroundSurface.convertXClickRealToVirtual(pEvent->motion.x);
		m_iCurrentMouseY = m_oForegroundSurface.convertYClickRealToVirtual(pEvent->motion.y);
		virtMouseMoved(m_iCurrentMouseX, m_iCurrentMouseY);
		if (m_bNotifyObjectsAboutMouse)
			std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=](DisplayableObject* pdo) { if (pdo == nullptr) return; pdo->virtMouseMoved(m_iCurrentMouseX, m_iCurrentMouseY); });
		break;

	case SDL_MOUSEBUTTONDOWN:
		// Note: If you are not using a filter this just assigns the motion.x to the m_iCurrentMouseX. If using a filter it may convert the point. See filter at top of DrawingSurface if you really need to know what this is doing.
		m_iMouseXClickedDown = m_oForegroundSurface.convertXClickRealToVirtual(pEvent->button.x);
		m_iMouseYClickedDown = m_oForegroundSurface.convertYClickRealToVirtual(pEvent->button.y);
		virtMouseDown(pEvent->button.button, m_iMouseXClickedDown, m_iMouseYClickedDown);

		if (m_bNotifyObjectsAboutMouse)
			std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=](DisplayableObject* pdo) { if (pdo == nullptr) return; pdo->virtMouseDown(pEvent->button.button, m_iMouseXClickedDown, m_iMouseYClickedDown); });

		if (m_bNotifyBottomObjectAboutMouse)
		{
			for (int i = 0; i < m_vecDisplayableObjects.size(); i++)
				if (m_vecDisplayableObjects[i] != nullptr
					&& m_vecDisplayableObjects[i]->virtSingleItemMouseDown(pEvent->button.button, m_iMouseXClickedDown, m_iMouseYClickedDown))
					break; // from for loop - this one handled it, so stop now
		}

		if (m_bNotifyTopObjectAboutMouse)
		{
			for (int i = m_vecDisplayableObjects.size() - 1; i >= 0; i--)
				if (m_vecDisplayableObjects[i] != nullptr
					&& m_vecDisplayableObjects[i]->virtSingleItemMouseDown(pEvent->button.button, m_iMouseXClickedDown, m_iMouseYClickedDown))
					break; // from for loop - this one handled it, so stop now
		}
		//redrawDisplay();
		break;

	case SDL_MOUSEBUTTONUP:
		// Note: If you are not using a filter this just assigns the motion.x to the m_iCurrentMouseX. If using a filter it may convert the point. See filter at top of DrawingSurface if you really need to know what this is doing.
		m_iMouseXClickedUp = m_oForegroundSurface.convertXClickRealToVirtual(pEvent->button.x);
		m_iMouseYClickedUp = m_oForegroundSurface.convertYClickRealToVirtual(pEvent->button.y);
		virtMouseUp(pEvent->button.button, m_iMouseXClickedUp, m_iMouseYClickedUp);
		if (m_bNotifyObjectsAboutMouse)
			std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=](DisplayableObject* pdo) { if (pdo == nullptr) return; pdo->virtMouseUp(pEvent->button.button, m_iMouseXClickedUp, m_iMouseYClickedUp); });
		//redrawDisplay();
		break;

	case SDL_MOUSEWHEEL:
		virtMouseWheel(pEvent->wheel.x, pEvent->wheel.y, pEvent->wheel.which, pEvent->wheel.timestamp);
		break;

	case SDL_FINGERMOTION:
	case SDL_FINGERDOWN:
	case SDL_FINGERUP:
		virtTouchEvent(pEvent->tfinger.type, pEvent->tfinger.timestamp, pEvent->tfinger.touchId, pEvent->tfinger.fingerId, pEvent->tfinger.x, pEvent->tfinger.y, pEvent->tfinger.dx, pEvent->tfinger.dy, pEvent->tfinger.pressure);
		break;

	case SDL_QUIT:
		return false;
	}
	return true;
}

/*
Main loop.
1) Handles key presses and events
2) Calls virtUpdate(), which subclasses can change. If it returns true then it asks all objects to update
	By default virtUpdate() just checks whether it is time to do an update yet.
3) Calls virtRenderScreen() to draw the screen
	If redrawDisplay() was called, this will copy all of the background onto the screen.
	Otherwise, if redrawRectangle() can called, it will copy that rectangle of pixels from the background
	It will then draw strings underneath, then ask objects to draw themselves, then draw strings on top, to draw the screen.
*/
int BaseEngine::mainLoop(void)
{
    SDL_Event event;
	int iKeyCode;

	// Main loop: loop until told not to
	while (m_iExitWithCode==-1)
	{
		// Hook to allow subclasses to do something here
		virtMainLoopStartIteration();

		if (m_bBlockWaitingForEvents) // Only poll for events rather than waiting for one to happen - default behaviour
		{ // Block waiting for the next event - i.e. don't do anything while there are no UI events - so no AI movement etc.
			// Wait for an event to happen - keeps cpu usage low but don't use it if you need things to move without user interactions
			if (SDL_WaitEvent(&event))
				if (!handleEvent(&event))
					return 0;
		}

		// Keep polling for subsequent events until they have all been handled, rather than handling one at a time per virtual funciton call.
		while (SDL_PollEvent(&event))
			if (!handleEvent(&event))
				return 0;

		// Hook to allow subclasses to do something here
		virtMainLoopPreUpdate();

		// Render stuff
		m_iTick = SDL_GetTicks();

		// Called to allow sub-classes to easily add behaviour
		if (virtUpdate())
		{
			// Do any updates for movements
			updateAllObjects(getModifiedTime());
		}

		// Hook to allow subclasses to do something here
		virtMainLoopPostUpdate();

		// Render the screen
		virtRenderScreen();
	}
	return 0;
}


// Change behaviour in subclass if needed.
// If this returns false then objects will not be told to move - allowing you to freeze them all if you wish
bool BaseEngine::virtUpdate(void)
{
	if (this->isTimeToAct())
	{
		this->setTimeToAct(10); // Max 1 per 10ms
		return true;
	}
	else
	{
		return false; // Don't move anything
	}
}

/*
Overridable function, if necessary, for doing all of the drawing.
*/
void BaseEngine::virtRenderScreen(void)
{
	if ( !m_bNeedsRedraw )
		return;

	// Note: the redraw flags must only be set early in this method, not at the end, since the drawing/moving of the moving objects may
	// potentially flag a win/lose condition and a state change, which will set the redraw flag again to force a full draw after the 
	// state changes.

	// Lock foreground surface if necessary
	m_oForegroundSurface.mySDLLockSurface();

	// First clear the screen by redrawing the background
	//std::cout << "Redraw background..." << std::endl;
	if (m_bTotalRedraw)
		copyAllBackgroundBuffer();
	else // Partial redraw so only clear the part of screen we are redrawing.
	{ // Copy background pixels from the redraw region only
		//this->copyBackgroundPixels(m_iRedrawRealLeft -1, m_iRedrawRealTop -1, m_iRedrawRealRight- m_iRedrawRealLeft +2, m_iRedrawRealBottom - m_iRedrawRealTop +2);
		this->copyBackgroundPixels(m_iRedrawRealLeft, m_iRedrawRealTop, m_iRedrawRealRight - m_iRedrawRealLeft, m_iRedrawRealBottom - m_iRedrawRealTop);
		//std::cout << "Clear background, real l " << std::to_string(m_iRedrawRealLeft) << ",  r " << std::to_string(m_iRedrawRealRight)
		//	<< " : t " << std::to_string(m_iRedrawRealTop) << ",  b " << std::to_string(m_iRedrawRealBottom) << std::endl;
		//std::cout << "Clear background, virtual l " << std::to_string(m_iRedrawVirtualLeft) << ",  r " << std::to_string(m_iRedrawVirtualRight)
		//	<< " : t " << std::to_string(m_iRedrawVirtualTop) << ",  b " << std::to_string(m_iRedrawVirtualBottom) << std::endl;
	}

	// Draw the text for the user
	//std::cout << "virtDrawStringsUnderneath..." << std::endl;
	virtDrawStringsUnderneath();	// Draw the string underneath the other objects here
	// Then draw the changing objects
	//std::cout << "drawAllObjects..." << std::endl;
	drawAllObjects();
	// Another option for where to put the draw strings code - if you want it on top of the moving objects
	//std::cout << "virtDrawStringsOnTop..." << std::endl;
	virtDrawStringsOnTop();

	// Lock foreground surface if necessary
	m_oForegroundSurface.mySDLUnlockSurface();
	//std::cout << "Finished draw..." << std::endl;

	// Copy surface to a texture, which is then rendered to the screen.
	m_oForegroundSurface.mySDLUpdateTexture(m_pSDL2Texture);
	SDL_RenderCopy( m_pSDL2Renderer,m_pSDL2Texture,NULL,NULL );
	SDL_RenderPresent( m_pSDL2Renderer );

	// Just drawn so no more redraw needed
	m_bNeedsRedraw = false;
	m_bTotalRedraw = false;
	m_iRedrawVirtualLeft = INT_MAX / 2;
	m_iRedrawRealLeft = INT_MAX / 2;
	m_iRedrawVirtualTop = INT_MAX / 2;
	m_iRedrawRealTop = INT_MAX / 2;
	m_iRedrawVirtualRight = -INT_MAX / 2;
	m_iRedrawRealRight = -INT_MAX / 2;
	m_iRedrawVirtualBottom = -INT_MAX / 2;
	m_iRedrawRealBottom = -INT_MAX / 2;
}












/* Deinitialise everything - delete all resources */
void BaseEngine::deinitialise(void)
{
	// Call method to allow custom sub-class cleanup
	virtCleanUp();

	// Free memory for key status record
	delete[] m_pKeyStatus;
}


/*
Overridable function, for adding custom clean-up in sub-classes.
*/
void BaseEngine::virtCleanUp(void)
{
}





// Determine whether a specific key is currently pressed
bool BaseEngine::isKeyPressed(int iKeyCode) const
{
	return m_pKeyStatus[symbolToKeycode(iKeyCode)];
}


/* Override this to handle a key being released.
Note that the fact that the key has been released will have already been stored.
*/
void BaseEngine::virtKeyUp(int iKeyCode)
{
}

/* Override this to handle a key being pressed
Note that the fact that the key has been pressed will have already been stored.
*/
void BaseEngine::virtKeyDown( int iKeyCode )
{
}

/* Override this to handle the fact that a mouse has moved
	Note: these coordinates are automatically stored for you and there are methods to retrieve them if you wish
*/
void BaseEngine::virtMouseMoved( int iX, int iY )
{
	//printf( "BaseEngine::MouseMoved %d, %d\n", iX, iY );
	//DrawForegroundThickLine( m_iMouseXClickedDown, m_iMouseYClickedDown, iX, iY, 0xff0000, 1 );
}

/* Override this to handle the fact that a mouse button has been pressed
Note: these coordinates are automatically stored for you and there are methods to retrieve them if you wish
*/
void BaseEngine::virtMouseDown( int iButton, int iX, int iY )
{
	// Test code if you want it?
	// printf( "BaseEngine::MouseDown %d, %d, %d\n", iButton, iX, iY );
}

/*
Override this to handle the fact that a mouse button has been released
Note: these coordinates are automatically stored for you and there are methods to retrieve them if you wish
*/
void BaseEngine::virtMouseUp( int iButton, int iX, int iY )
{
	// Test code if you want it?
	// printf( "BaseEngine::MouseUp %d, %d, %d\n", iButton, iX, iY );
}



















/* Fill the background - should be overridden to actually draw the correct thing. */
void BaseEngine::virtSetupBackgroundBuffer()
{
	// Put any special code to call different render functions for different states here
	fillBackground( 0xffff00 );
}






/* You probably need to override these to draw text onto the screen.
The key question is whether you want the text to appear on top of or underneath the moving objects.
i.e. can objects move over the text, or underneath it if they are in the same position.
*/



/* Draw the string that moving objects should be drawn on top of.
You can either do everything for strings in this function if you wish, ignoring UnDrawStrings() and DrawStringsOnTop(),
or you can put the background removal in UnDrawStrings(), then use this to draw strings behind moving objects and 
the three below, or you can split the contents.
*/
void BaseEngine::virtDrawStringsUnderneath()
{
	// Example Version 1: If using only this function:
	//		First clear the area where they were originally drawn - i.e. undraw previous text
	//		e.g. CopyBackgroundBuffer()
	//		Then draw the new labels/strings
	//		e.g. m_pFont->DrawString(m_pForegroundSurface, 100, 180, "The DrawStringsUnderneath method needs to be overridden", 0xff00ff);
	//		or DrawForegroundString( 150,10,"Example text",0xffffff,NULL );
	// OR Example Version 2 - assuming the string removal is in UnDrawStrings() above
	//		Just draw any strings you want to draw...
	//		e.g. DrawForegroundString( 150,10,"Example text",0xffffff,NULL );
	// For efficiency I recommend version 2, so that the undrawing is only done when needed, rather than every time the strings are drawn,
	// however, that may be harder for some of you to understand hence the option to put it all in here if you wish.
}

/* Draw any string which should appear on top of moving objects - i.e. objects move behind these.
There are two differences between this and the Underneath() function above:
1: These strings appear on top of the moving objects (i.e. they are drawn AFTER the moving objects)
2: Because of when this function is called, you cannot remove the strings in it. i.e. you need to implement UnDrawStrings()
*/
void BaseEngine::virtDrawStringsOnTop()
{
	// Example:
	// DrawForegroundString( 150,60,"Example text",0xffffff,NULL );
}












/*
Draws draws the displayable objects in their new positions.
*/
void BaseEngine::drawAllObjects()
{
	for (auto it = m_vecDisplayableObjects.begin(); it != m_vecDisplayableObjects.end(); it++)
		if (*it) 
			(*it)->setHasActed(false);

	bool finished = false;
	while (!finished)
	{
		//std::cout << "starting draw loop..." << std::endl;
		m_bDrawableObjectsChanged = false; // Moved to inside loop to avoid infinite loop
		finished = true;
		for (auto it = m_vecDisplayableObjects.begin(); it != m_vecDisplayableObjects.end(); it++)
		{
			if (*it)
			{
				if (!(*it)->getHasActed())
				{
					//std::cout << "draw " << (int)(*it) << std::endl;
					(*it)->setHasActed(true);
					(*it)->virtDraw();
				}
				if (m_bDrawableObjectsChanged)
				{
					finished = false;
					break; // from the for statement - will repeat loop again
				}
			}
		}
	}
}


/* Tell all displayable objects to update themselves. Calls virtDoUpdate on each displayable object. */
void BaseEngine::updateAllObjects(int iCurrentTime)
{
	for (auto it = m_vecDisplayableObjects.begin(); it != m_vecDisplayableObjects.end(); it++)
		if (*it) 
			(*it)->setHasActed(false);

	bool finished = false;
	while (!finished)
	{
		m_bDrawableObjectsChanged = false; // Moved to inside loop to avoid infinite loop
		finished = true;
		for (auto it = m_vecDisplayableObjects.begin(); it != m_vecDisplayableObjects.end(); it++)
		{
			if (*it)
			{
				if (!(*it)->getHasActed())
				{
					(*it)->setHasActed(true);
					(*it)->virtDoUpdate(iCurrentTime);
				}
				if (m_bDrawableObjectsChanged)
				{
					finished = false;
					break; // from the for statement - will repeat loop again
				}
			}
		}
	}
}






/*
Get a colour code for a specific index
Used to avoid creating arbitrary colours and instead have a set of useful values.
*/
unsigned int BaseEngine::getColour(int iColourIndex) const
{
	switch ( iColourIndex )
	{
	case 0: return 0x000000;
	case 1: return 0xff0000;
	case 2: return 0x00ff00;
	case 3: return 0x0000ff;
	case 4: return 0xff00ff;
	case 5: return 0xffff00;
	case 6: return 0x00ffff;
	case 7: return 0xffffff;
	case 8:	return 0xffc0c0;
	case 9: return 0xc0ffc0;
	case 10: return 0xc0c0ff;
	case 11: return 0xc00000;
	case 12: return 0x00c000;
	case 13: return 0x0000c0;
	case 14: return 0xc000c0;
	case 15: return 0xc0c000;
	case 16: return 0x00c0c0;
	case 17: return 0xc0c0c0;
	case 18: return 0xff8080;
	case 19: return 0x80ff80;
	case 20: return 0x8080ff;
	case 21: return 0x800000;
	case 22: return 0x008000;
	case 23: return 0x000080;
	case 24: return 0x800080;
	case 25: return 0x808000;
	case 26: return 0x008080;
	case 27: return 0x808080;
	case 28: return 0xc08080;
	case 29: return 0x80c080;
	case 30: return 0x8080c0;
	case 31: return 0x400000;
	case 32: return 0x004000;
	case 33: return 0x000040;
	case 34: return 0x400040;
	case 35: return 0x404000;
	case 36: return 0x004040;
	case 37: return 0x404040;
	case 38: return 0xc04040;
	case 39: return 0x40c040;
	case 40: return 0x4040c0;
	}
	return 0xcccccc; // Anything else is grey
}





