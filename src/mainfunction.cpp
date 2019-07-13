#include "header.h"

#include <ctime>

#include "templates.h"

// Needs one of the following #includes, to include the class definition
#include "SimpleDemo.h"
#include "BouncingBallMain.h"
#include "MazeDemoMain.h"
#include "DraggingDemo.h"
#include "ZoomingDemo.h"
#include "PlayingCardsDemo.h"
#include "ImageObjectDemo.h"

// MY HEADER FILE:
#include "JasonsDemoA.h"
#include "Psyfb2.h"


const int BaseScreenWidth = 1280;
const int BaseScreenHeight = 720;


int main(int argc, char *argv[])
{
	int iResult;

	{ // Put the main object within a block so it gets destroyed before end of main
		// Send random number generator with current time
		::srand(time(0));

		// Needs just one of the following lines - choose which object to create.
		// Note these are from simplest to most complex demo.
		//JasonsDemoA oMain; // Jason's version of MyDemoA, MyObjectA and MyTileManagerA
		//SimpleDemo oMain;
		//ImageObjectDemo oMain; // Shows drawing images to the foreground and background
		//BouncingBallMain oMain; // A ball bouncing around with images and shapes drawn
		//MazeDemoMain oMain; // Demonstrates a tile manager being used and updated
		//DraggingDemo oMain; // Includes dragable images and image objects
		//ZoomingDemo oMain;	// Shows the use of a filter to shift and rescale the display - this is complex and you can ignore it!
		//PlayingCardsDemo oMain; // The most advanced demo - you may want to ignore this one totally!
		Psyfb2 oMain;

		char buf[1024];
		sprintf(buf, "My Demonstration Program : Size %d x %d", BaseScreenWidth, BaseScreenHeight);
		iResult = oMain.initialise(buf, BaseScreenWidth, BaseScreenHeight, "Cornerstone Regular.ttf", 24);
		iResult = oMain.mainLoop();
		oMain.deinitialise();
	} // Main object gets destroyed at this point, so it will free its memory
	
	// Free the cached images by destroying the image manager
	// Ensure that you do this AFTER the main object and any other objects have been destroyed
	ImageManager::destroyImageManager();
	
// Detect memory leaks on windows
#if defined(_MSC_VER)
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
#endif

	return iResult;
}
