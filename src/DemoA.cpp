#include "header.h"
#include "DemoA.h"
#include "ImageManager.h"
#include "MyObjectA.h"


DemoA::DemoA()
{
}


DemoA::~DemoA()
{
}


// Override parent method to draw background
void DemoA::virtSetupBackgroundBuffer()
{
	fillBackground(0x000000);
	for (int i = 0; i < getWindowWidth(); i++) {
		for (int j = 0; j < getWindowHeight(); j++) {
			switch (rand() % 100) {
			case 0: setBackgroundPixel(i, j, 0xff0000); break;
			case 1: setBackgroundPixel(i, j, 0x00ff00); break;
			case 2: setBackgroundPixel(i, j, 0x0000ff); break;
			case 3: setBackgroundPixel(i, j, 0x0f0f0f); break;
			case 4: setBackgroundPixel(i, j, 0xef0d40); break;
			}
		}
	}

	for (int i = 0; i < tm.getMapWidth(); i++) {
		for (int j = 0; j < tm.getMapHeight(); j++) {
			tm.setMapValue(i, j, rand());
		}
	}
	tm.setTopLeftPositionOnScreen(50, 50);
	tm.drawAllTiles(this, getBackgroundSurface());

	// load an image
	SimpleImage image = ImageManager::loadImage("demo.png", true);
	image.renderImage(getBackgroundSurface(), 0, 0, 10, 10, image.getWidth(), image.getHeight());
}


// Identify mouse clicks
void DemoA::virtMouseDown(int button, int x, int y)
{
	printf("Mouse clicked at %d, %d\n", x, y);

	if (tm.isValidTilePosition(x, y) && button == SDL_BUTTON_LEFT) {
		int mapX = tm.getMapXForScreenX(x);
		int mapY = tm.getMapYForScreenY(y);
		int color = tm.getMapValue(mapX, mapY);
		tm.setAndRedrawMapValueAt(mapX, mapY, color + 1, this, getBackgroundSurface());
		redrawDisplay();
	}
	
}


// Called after a keypress
void DemoA::virtKeyDown(int keyCode)
{
	switch (keyCode) {
	case ' ': 
		virtSetupBackgroundBuffer();
		redrawDisplay();
		break;
	case 'b':
		virtMouseDown(SDL_BUTTON_LEFT, rand() % (getWindowWidth() - 10), rand() % (getWindowHeight() - 10));
		redrawDisplay();
		break;
	}
}


int DemoA::virtInitialiseObjects()
{
	// tell framework to reload array of objects
	drawableObjectsChanged();

	// destroy all existing objects
	destroyOldObjects(true);

	createObjectArray(1);

	storeObjectInArray(0, new MyObjectA(this));

	setAllObjectsVisible(true);
	return 0;
}
