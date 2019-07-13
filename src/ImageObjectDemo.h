#pragma once

#include "BaseEngine.h"

// This is a relatively simple demo but should be useful to see images used
class ImageObjectDemo : public BaseEngine
{
public:

	/**
	Constructor
	*/
	ImageObjectDemo()
	{}

	// Do any setup of back buffer prior to locking the screen buffer
	// Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
	virtual void virtSetupBackgroundBuffer() override;

	// Create any moving objects
	int virtInitialiseObjects() override;

	/** Draw any strings */
	void virtDrawStringsUnderneath() override;
	void virtDrawStringsOnTop() override;

	// Handle pressing of a mouse button
	void virtMouseDown(int iButton, int iX, int iY) override;

	// Handle pressing of a key
	virtual void virtKeyDown(int iKeyCode) override;
};
