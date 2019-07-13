#include "header.h"

#include "BaseEngine.h"

#include "BouncingBall.h"

#include "BouncingBallMain.h"

#include "ImageManager.h"

#include "TileManager.h"

/*
Draw the background of the screen.
This fills the background with black
Then puts some random colour on it
Then demos various ways of drawing shapes, images, etc
*/
void BouncingBallMain::virtSetupBackgroundBuffer()
{
	// #include "DrawingFilters.h" at the top to use these
	//static FilterPoints* filterMask = new FilterPointsRectangularMask(10, 10, 500, 500);
	// static FilterPoints* filterMask = new FilterPointsOnScreen(); // create a new one
	//static FilterPoints* filterMask = this->getForegroundSurface()->getDrawPointsFilter(); // get the default one - which is already a mask for screen size unless we clear it
	//static FilterPoints* filterStretch = new FilterPointsScaling(0, 0, filterMask);
	//this->getForegroundSurface()->setDrawPointsFilter(filterStretch);
	//this->getBackgroundSurface()->setDrawPointsFilter(filterStretch);

	fillBackground( 0 );

	for ( int iX = 0 ; iX < getWindowWidth() ; iX++ )
		for ( int iY = 0 ; iY < this->getWindowHeight() ; iY++ )
			switch( rand()%100 )
			{
			case 0: setBackgroundPixel( iX, iY, 0xFF0000 ); break;
			case 1: setBackgroundPixel( iX, iY, 0x00FF00 ); break;
			case 2: setBackgroundPixel( iX, iY, 0x0000FF ); break;
			case 3: setBackgroundPixel( iX, iY, 0xFFFF00 ); break;
			case 4: setBackgroundPixel( iX, iY, 0x00FFFF ); break;
			case 5: setBackgroundPixel( iX, iY, 0xFF00FF ); break;
		}

	drawBackgroundVerticalSidedRegion( 650, 750, // X
		100, 50, // Top line
		150, 180, // Bottom line
		0xff0000 );

	int iLineThickness = 2;

	// Draw a triangle where all three points are specified.
	drawBackgroundTriangle( 300,300, 325,275, 350,350, 0x00ffff );
	drawBackgroundTriangle( 425,375, 450,450, 400,375, 0x00ffff );
	drawBackgroundTriangle( 550,550, 525,475, 450,500, 0x00ffff );

	// Draw various polygons
//	drawBackgroundPolygon( 100, 100, 150, 100, 250, 200, 150, 200, 0xff0000 );
//	drawBackgroundPolygon( 100, 300, 200, 275, 300, 325, 250, 400, 150, 375, 0x00ff00 );
//	drawBackgroundPolygon( 400, 100, 500, 150, 600, 100, 550, 200, 450, 200, 400, 250, 0x0000ff );

	// Draw a polygon and some lines joining the points.
	drawBackgroundPolygon( 100, 100, 150, 100, 
			250, 200, 150, 200, 
			100, 300, 200, 275, 
			300, 325, 50, 400, 0xff00ff );
	drawBackgroundLine( 100, 100, 150, 100, 0xffffff );
	drawBackgroundLine( 150, 100, 250, 200, 0xffffff );
	drawBackgroundLine( 250, 200, 150, 200, 0xffffff );
	drawBackgroundLine( 150, 200, 100, 300, 0xffffff );
	drawBackgroundLine( 100, 300, 200, 275, 0xffffff );
	drawBackgroundLine( 200, 275, 300, 325, 0xffffff );
	drawBackgroundLine( 300, 325,  50, 400, 0xffffff );
	drawBackgroundLine(  50, 400, 100, 100, 0xffffff );

	drawBackgroundPolygon( 350, 100, 500, 150, 
			600, 100, 550, 200, 
			450, 200, 400, 250, 
			410, 180, 0x00ff00 );
	drawBackgroundLine( 350, 100, 500, 150, 0xffffff );
	drawBackgroundLine( 500, 150, 600, 100, 0xffffff );
	drawBackgroundLine( 600, 100, 550, 200, 0xffffff );
	drawBackgroundLine( 550, 200, 450, 200, 0xffffff );
	drawBackgroundLine( 450, 200, 400, 250, 0xffffff );
	drawBackgroundLine( 400, 250, 410, 180, 0xffffff );
	drawBackgroundLine( 410, 180, 350, 100, 0xffffff );

	// Well done if you are reading this since it means that you bothered to go through the source code. 
	// This code will create three images, which shows you how to use basic images
	// It creates them as local variables, so would recreate these objects each time this function is called! (may not be good for many functions)
#if 0
	// Draw an image loaded from a file.
	//ImageData im, im2;
	// Load the image file into an image object - at the normal size
	SimpleImage im1 = ImageManager::get()->getImagebyURL("demo.png", true/*Load*/, true/*Keep it loaded*/);
	SimpleImage im2 = ImageManager::get()->resizeTo(im1, 96, 60); // Original is 192x120
	SimpleImage im3 = ImageManager::get()->shrinkBy(im1, 2);

	// Note: image loaded only once, above, and now we will draw it nine times
	// Also note: don't try to use the 'WithMask' version with a jpg - the mask needs a solid colour
	for (int i = 0; i < 3; i++)
	{
		im1.renderImageWithMask(getBackgroundSurface(),
			0, 0,
			100, i * 200,
			im1.getWidth(), im1.getHeight());
		im2.renderImageWithMask(getBackgroundSurface(),
			0, 0,
			300, i * 200,
			im2.getWidth(), im2.getHeight());
		im3.renderImageWithMask(getBackgroundSurface(),
			0, 0,
			500, i * 200,
			im3.getWidth(), im3.getHeight());
	}
#endif


#if 0
	// This uses an image renderer - which is a powerful version of imagedata
	// The only problem is that it's a bigger object - it maintains more state inside it
	ImageRenderer im1 = ImageManager::get()->getImageRendererbyURL("demo.png", true, true);
	im1.setTransparencyColour(im1.getPixelColour(0, 0));
	ImageRenderer im2 = ImageManager::get()->getImageRendererPartbyURL("demo.png", 1/*Left*/, 1/*top*/, 100/*Width*/, 100/*height*/, true, true);
	im2.setTransparencyColour(im2.getPixelColour(0, 0));
	ImageRenderer im3 = ImageManager::get()->getImageRendererPartbyURL("demo.png", 30/*Left*/, 1/*top*/, 100/*Width*/, 100/*height*/, true, true);
	im3.setTransparencyColour(im3.getPixelColour(0, 0));
	im3.setDrawingSize(200, 200);
	for (int i = 0; i < 3; i++)
	{
		im1.render(getBackgroundSurface(), i * 200, 000 );
		im2.render(getBackgroundSurface(), i * 200, 200 );
		im3.render(getBackgroundSurface(), i * 200, 350 );
	}
#endif


	// Draw some tiles using the tile manager
	// Specify how many tiles wide and high
	obTileManager.setMapSize( 10, 10 ); 
	// Specify the screen x,y of top left corner
	obTileManager.setTopLeftPositionOnScreen( 450,300 );
	// Tell it to draw tiles from x1,y1 to x2,y2 in tile array, to the background of this screen
	obTileManager.drawAllTiles( this, getBackgroundSurface() ); // Draw to background!
}


/*
In here you need to create any movable objects that you wish to use.
Sub-classes need to implement this function.
*/
int BouncingBallMain::virtInitialiseObjects()
{
	// I put a mouse eevent handler into one of the balls, so we need to ensure that objects are told about mouse events.
	notifyObjectsAboutMouse(true);

	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	drawableObjectsChanged();

	// Destroy any existing objects
	destroyOldObjects(true);

	// Create a new object of type BouncingBall1
	m_pBall1 = new BouncingBall1( this, 
		0/*Id*/, 1/*Type*/, 
		100/*Size*/, 
		1/*Colour*/,
		"A"/*Label*/, 
		-10/*XLabelOffset*/,
		-15/*YLabelOffset*/, 
		&obTileManager );

	// Bouncing ball 1 has a movement object, tells it to move from
	// position a to position b, over a specific period of time.
	// The update for the ball will reverse the move at the end of this time
	m_pBall1->setMovement( getModifiedTime(), getModifiedTime()+1000, getModifiedTime(), 100, 100, 700, 500 );

	// Create another, different, type of ball
	m_pBall2 = new BouncingBall2( this, 
		0/*Id*/, 1/*Type*/, 
		100/*Size*/, 
		2/*Colour*/,
		"B"/*Label*/, 
		-10/*XLabelOffset*/,
		-15/*YLabelOffset*/ );
	m_pBall2->setPosition( 100,100 );
	m_pBall2->setSpeed( 1.1,1.0 );

	// And a third ball
	m_pBall3 = new BouncingBall2( this, 
		0/*Id*/, 1/*Type*/, 
		100/*Size*/, 
		3/*Colour*/,
		"C"/*Label*/, 
		-10/*XLabelOffset*/,
		-15/*YLabelOffset*/ );
	m_pBall3->setPosition( 300,250 );
	m_pBall3->setSpeed( 0.8,1.2 );

	// Creates an array one element larger than the number of objects that you want.
	createObjectArray(3);

	// You MUST set the array entry after the last one that you create to NULL, so that the system knows when to stop.
	storeObjectInArray(0, m_pBall1 );
	storeObjectInArray(1, m_pBall2 );
	storeObjectInArray(2, m_pBall3 );

	// NOTE: We also need to destroy the objects, but the method at the 
	// top of this function will destroy all objects pointed at by the 
	// array elements so we can ignore that here.

	return 0;
}







/* Draw text labels */
void BouncingBallMain::virtDrawStringsUnderneath()
{
	drawForegroundString( 150, 10, "Example text", 0xffffff, getFont( "Cornerstone Regular.ttf", 48) );
}


// Override to add a node at specified point
void BouncingBallMain::virtMouseDown( int iButton, int iX, int iY )
{
	m_pBall2->setPosition( iX, iY );
}

/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is pressed
*/
void BouncingBallMain::virtKeyDown(int iKeyCode)
{
	switch ( iKeyCode )
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		setExitWithCode( 0 );
		break;
	case SDLK_SPACE: // SPACE Pauses
		break;
	}
}

