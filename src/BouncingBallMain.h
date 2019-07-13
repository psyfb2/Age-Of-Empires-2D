#ifndef BouncingBallMain_H
#define BouncingBallMain_H

#include "BaseEngine.h"

// Forward declarations of classes
// Says that these are classes, but not what the classes look like
// Doesn't matter, since I only use pointers to them, so compiler only
// cares when I de-reference the pointer.
class BouncingBall;
class BouncingBall1;
class BouncingBall2;

// This includes the class definition for the TileManager class,
// which I use
#include "TileManager.h"

// Class definition for BouncingBallMain
// Says that it subclasses a class called BaseEngine
// so we inherit a lot of functionality for this class
class BouncingBallMain :
	public BaseEngine
{
protected:
	// Three member variables, to store pointers to the three balls
	// Only the pointers are created with the main object - not anything that they may point to
	// These have to be initialised in the initialisation list
	BouncingBall1* m_pBall1;
	BouncingBall2* m_pBall2;
	BouncingBall2* m_pBall3;

	// A member object. Object is created when the BouncingBallMain is created
	TileManager obTileManager;

public:

	/**
	Constructor
	The : here refers to an initialisation list
	*/
	BouncingBallMain()
	: m_pBall1(NULL)  // Initialise pointers to NULL
	, m_pBall2(NULL)
	, m_pBall3(NULL)
	, obTileManager(25,25) // Tile manager - set the tile size.
	{}

	// Do any setup of back buffer prior to locking the screen buffer
	// Do the drawing of the background in here and 
	// it'll be copied to the screen for you as needed
	virtual void virtSetupBackgroundBuffer() override;

	// Create the bouncing ball(s)
	int virtInitialiseObjects() override;

	// Draw any strings that you need on the screen
	void virtDrawStringsUnderneath() override;

	// Mouse-down override
	void virtMouseDown( int iButton, int iX, int iY ) override;

	// Handle key presses if necessary
	virtual void virtKeyDown(int iKeyCode) override;

	// Test code to test the various message hooks in the base engine:
	virtual void virtMainLoopStartIteration() { std::cout << "virtMainLoopStartIteration" << std::endl; }
	virtual void virtMainLoopPreUpdate() { std::cout << "\tvirtMainLoopPreUpdate" << std::endl; }
	virtual void virtMainLoopPostUpdate() { std::cout << "\tvirtMainLoopPostUpdate" << std::endl; }

};

#endif
