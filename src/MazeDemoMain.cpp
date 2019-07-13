#include "header.h"

#include "BaseEngine.h"

#include "MazeDemoObject.h"

#include "MazeDemoMain.h"

#include "ImageManager.h"

#include "TileManager.h"


MazeDemoMain::MazeDemoMain(void)
: m_state(stateInit) // NEW
{
}

MazeDemoMain::~MazeDemoMain(void)
{
}

void MazeDemoMain::virtSetupBackgroundBuffer()
{
	// Switches on state to implement some state-based behaviour - a 
	switch( m_state )
	{
	case stateInit: // Reload the level data
		fillBackground( 0xffff00 );
		{
			const char* data[] = {
				"bbbbbbbbbbbbbbb",
				"baeaeadadaeaeab",
				"babbbcbcbcbbbeb",
				"badadgdadhdadhb",
				"bgbcbbbcbbbcbeb",
				"babadadadadabab",
				"bfbcbbbcbbbcbeb",
				"bahadadhdadadab",
				"bfbbbcbibcbbbeb",
				"badadadadadadab",
				"bbbbbbbbbbbbbbb" };

			// Specify how many tiles wide and high
			m_oTiles.setMapSize( 15, 11 ); 
			// Set up the tiles
			for ( int x = 0 ; x < 15 ; x++ )
				for ( int y = 0 ; y < 11 ; y++ )
					m_oTiles.setMapValue( x, y, data[y][x]-'a' );

			for ( int y = 0 ; y < 11 ; y++ )
			{
				for ( int x = 0 ; x < 15 ; x++ )
					std::cout << ("%d ", m_oTiles.getMapValue(x,y) );
				std::cout << std::endl;
			}

			// Specify the screen x,y of top left corner
			m_oTiles.setTopLeftPositionOnScreen( 25, 40 );
		}
		return;

	case stateMain:
		fillBackground( 0 );
		// Tell it to draw tiles from x1,y1 to x2,y2 in tile array,
		// to the background of this screen
		m_oTiles.drawAllTiles( this, this->getBackgroundSurface() );
		break; // Drop out to the complicated stuff
	case statePaused:
		fillBackground( 0 );
		m_oTiles.drawAllTiles(this, this->getBackgroundSurface() );
		break;
	} // End switch
}



int MazeDemoMain::virtInitialiseObjects()
{
	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	drawableObjectsChanged();

	// Destroy any existing objects
	destroyOldObjects(true);

	// Creates an array one element larger than the number of objects that you want.
	createObjectArray(5);

	// You MUST set the array entry after the last one that you create to NULL, so that the system knows when to stop.
	storeObjectInArray( 0, new MazeDemoObject(this, 1, 1) );
	storeObjectInArray( 1, new MazeDemoObject(this, 9, 9) );
	storeObjectInArray( 2, new MazeDemoObject(this, 13, 9) );
	storeObjectInArray( 3, new MazeDemoObject(this, 9, 5) );
	storeObjectInArray( 4, new MazeDemoObject(this, 13, 5) );

	// NOTE: We also need to destroy the objects, but the method at the 
	// top of this function will destroy all objects pointed at by the 
	// array elements so we can ignore that here.

	setAllObjectsVisible(false);

	return 0;
}







/* Draw text labels */
void MazeDemoMain::virtDrawStringsUnderneath()
{
	// NEW SWITCH
	switch( m_state )
	{
	case stateInit:
		drawForegroundString( 100, 300, "Initialised and waiting for SPACE", 0x0, NULL );
		break;
	case stateMain:
		drawForegroundString( 250, 10, "Running", 0xffffff, NULL );
		break;
	case statePaused:
		drawForegroundString( 200, 300, "Paused. Press SPACE to continue", 0xffffff, NULL );
		break;
	}
}


// Override to add a node at specified point
void MazeDemoMain::virtMouseDown( int iButton, int iX, int iY )
{
}

/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is pressed
*/
void MazeDemoMain::virtKeyDown(int iKeyCode)
{
	// NEW SWITCH on current state
	switch ( iKeyCode )
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		setExitWithCode( 0 );
		break;
	case SDLK_SPACE: // SPACE Pauses
		switch( m_state )
		{
		case stateInit:
			// Go to state main
			m_state = stateMain;
			// Force redraw of background
			virtSetupBackgroundBuffer();
			// Ensure objects become visible now
			setAllObjectsVisible(true);
			// Redraw the whole screen now
			redrawDisplay();
			break;
		case stateMain:
			this->pause();
			// Go to state main
			m_state = statePaused;
			// Force screen redraw
			virtSetupBackgroundBuffer();
			redrawDisplay();
			break;
		case statePaused:
			// Go to state main
			m_state = stateMain;
			this->unpause();
			// Force redraw of background
			virtSetupBackgroundBuffer();
			// Redraw the whole screen now
			redrawDisplay();
			break;
		} // End switch on current state
		break; // End of case SPACE
	}
}



