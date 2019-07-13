#ifndef DISPLAYABLEOBJECT_H
#define DISPLAYABLEOBJECT_H

class BaseEngine;

class DisplayableObject
{
public:
	// Constructors
	DisplayableObject(BaseEngine* pEngine, int iWidth = 0, int iHeight = 0, bool useTopLeftFor00 = true);
	DisplayableObject(int xStart, int yStart, BaseEngine* pEngine, int iWidth, int iHeight, bool useTopLeftFor00 = true);

	// Create a new object which is a clone of this one. Uses the Java approach to avoid issues of assignment operator
	DisplayableObject* cloneObject() const { return new DisplayableObject(*this); }
	
	// Destructor
	virtual ~DisplayableObject(void);

	// Draw the object - override to implement the actual drawing of the correct object
	virtual void virtDraw();

	// Handle the update action, moving the object and/or handling any game logic
	virtual void virtDoUpdate( int iCurrentTime );

	// Handle a notification by a caller. The SignalNumber will have an application-specific meaning, i.e. you can use whatever you wish. See the NotifyAllObjects... functions in BaseEngine.
	virtual long virtNotify( int iSignalNumber, int x, int y ) { return 0; }

	// Set the width and height of the object - used by some methods to test for drawing etc
	void setSize(int width, int height)
	{
		m_iDrawWidth = width;
		m_iDrawHeight = height;
	}

	// Get the width in pixels that was stored for the object
	int getDrawWidth() const { return m_iDrawWidth;  }

	// Get the height in pixeks that was stored for the object
	int getDrawHeight() const { return m_iDrawHeight; }

	// Get position of left side of shape
	int getDrawingRegionLeft() const { return m_iCurrentScreenX + m_iStartDrawPosX; }

	// Get position of right side of shape
	int getDrawingRegionRight() const { return m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth; }

	// Get position of top of shape
	int getDrawingRegionTop() const { return m_iCurrentScreenY + m_iStartDrawPosY; }

	// Get position of bottom of shape
	int getDrawingRegionBottom() const { return m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight; }

	// Whether to delete the object when it is removed from the object array
	// Return false in the override in sub-class if you want to delete the object manually
	virtual bool deleteOnRemoval() const { return true; }



	// Get pointer to game object
	BaseEngine* getEngine() const { return m_pEngine; }

	/** Redraw the whole screen - background and objects */
	void redrawDisplay();

	/* 
	Call this to specify that only a section of the screen should be redrawn - default to updating the part the object would be drawn to.
	Note: need to call it before and after updating the positions, to redraw old and new position
	*/
	void virtRedrawRectangle();

	/*
	Return true if object is at least partially within the redraw area
	*/
	bool virtIsObjectInRedrawArea();

	/** Get the X centre position of the object */
	int getXCentre() const { return m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth/2; }

	/** Get the Y centre position of the object */
	int getYCentre() const { return m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight / 2; }

	// Call this to make the item visible or invisible
	void setVisible( bool bVisible ) { m_bVisible = bVisible; }

	// Returns true if the object should be visible
	bool isVisible() const { return m_bVisible; }

	// Fix coords if they are off the screen
	void fixPosition();

	// Check whether a set of coordinates are within the specified rectangle for the object - can be overridden to be better in a subclass
	virtual bool virtIsPositionWithinObject( int iX, int iY )
	{
		if (iX < (m_iCurrentScreenX + m_iStartDrawPosX)) return false;
		if (iX >= (m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth)) return false;
		if (iY < (m_iCurrentScreenY + m_iStartDrawPosY)) return false;
		if (iY >= (m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight)) return false;
		return true;
	}

	// Set current position
	void setPosition(int x, int y)
	{
		m_iCurrentScreenX = x - m_iStartDrawPosX;
		m_iCurrentScreenY = y - m_iStartDrawPosY;
	}

	bool getHasActed() const { return m_bHasActed; }
	void setHasActed(bool hasActed) { m_bHasActed = hasActed; }

	int getDefaultColour() const { return m_iColour; }
	void setDefaultColour(int iColour) { m_iColour = iColour; }

	/* Override if necessary - a key was released. You MUST call BaseEngine's notifyObjectsAboutKeys(true) for this to be called */
	virtual void virtKeyUp(int iKeyCode) {}
	/* Override if necessary - a key was pressed down. You MUST call BaseEngine's notifyObjectsAboutKeys(true) for this to be called */
	virtual void virtKeyDown(int iKeyCode) {}

	/* Override if necessary - Mouse event - mouse was moved. Must call BaseEngine::notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseMoved(int iX, int iY) {}
	/* Override if necessary - Mouse event - mouse button pressed. Must call BaseEngine::notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseDown(int iButton, int iX, int iY) {}
	/* Override if necessary - Mouse event - mouse button released. Must call BaseEngine::notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseUp(int iButton, int iX, int iY) {}

	/* 
	This is called for each item if the BaseEngine is told to do so.
	If you handle this event the return true, if not in your bounds then return false.
	BaseEngine stops telling objects as soon as one returns true, so only one is clicked.
	You need to use BaseEngine::notifyTopObjectAboutMouseClick() and notifyBottomObjectAboutMouseClick() to turn behaviour on or off (off by default)
	*/
	virtual bool virtSingleItemMouseDown(int iButton, int iX, int iY) { return false; }



	// These are really provided for the use of sub-classes so we will expose them as protected methods
protected:
	// Pointer to the game object
	BaseEngine* m_pEngine;

	// We expect the sub-class to specify where to draw the object, and where it came from
	// Current position of object on the screen
	int m_iCurrentScreenX;
	int m_iCurrentScreenY;

	// We expect the sub-class to specify the size of the drawable area, and where, within the drawable area, to draw the object
	// Details of where to actually draw this thing
	// Using these offsets allows moving of the drawn object within the drawing region, without altering the calling code.
	int m_iStartDrawPosX;
	int m_iStartDrawPosY;
	// Size of the object to draw
	int m_iDrawWidth;
	int m_iDrawHeight;

	// Default colour to use, in case the subclass wants to use one
	int m_iColour;

	// True if item is visible
	bool m_bVisible;

	// Track whether it has drawn/moved this iteration - used by framework, so ignore it
	bool m_bHasActed;
};




#endif
