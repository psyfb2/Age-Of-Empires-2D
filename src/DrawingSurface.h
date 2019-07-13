#pragma once


class BaseEngine;
class Font;

#include <assert.h>
#include "FilterPoints.h"

/*
All drawing is done by setting pixels on a surface.
The program would run faster if I put the surface on the graphics card, but I wanted to allow you the chance to understand how it is working, so I left it in main memory.
When drawing, the entire surface is copied to a texture and then to the screen.
*/
class DrawingSurface
{
public:
	// Constructor
	DrawingSurface(BaseEngine* m_pCreatorEngine);

	// Destructor
	~DrawingSurface() {}

	// This is called to create the surface to use, telling it how big to make it
	void createSurface(int iWidth, int iHeight)
	{
		m_pSDLSurface = SDL_CreateRGBSurface(0, iWidth, iHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	}
	
	/* Conversion operator - return the surface which is hidden inside - so we can pass a DrawingSurface to anything expecting an SDL_Surface and implicit conversion will take place */
	operator SDL_Surface*() { return m_pSDLSurface; }

	/* Set a pixel on a specific surface - the surface is a bitmapped display with one int per pixel so we just have to set the correct int to the colout to use. May look more complex than it is. */
	inline void rawSetPixel(int iX, int iY, unsigned int uiColour)
	{
		if (iX < 0 || iX >= getSurfaceWidth() || iY < 0 || iY >= getSurfaceHeight())
		{
			std::cout << "Error in setting pixel- should never happen!" << std::endl;
			int i = 0; // For a break point
		}

		// Set the value of the pixel
		((unsigned int *)m_pSDLSurface->pixels)[iX + iY * getIntsPerWindowRow()] = uiColour;
	}

	/*
	Set a pixel on a specific surface.
	This could just do the rawSetPixel but it gives the DisplaySurface a chance to apply any filter first.
	You can freely ignore filters, but they may be worth investigating if you want to scroll the screen or zoom in/out.
	Hint: the way that the objects is used is a bit like functional composition in Haskell. OO uses objects and polymorphism where Haskell uses functions.
	*/
	inline void setPixel(int iX, int iY, unsigned int uiColour)
	{
		if (m_pFilter != nullptr)
			if (!m_pFilter->filter(this, iX, iY, uiColour))
				return; // Don't do anything
		// Note: filter may have changed the x and y if it returned true
		rawSetPixel(iX, iY, uiColour);
	}

	/* Set a pixel on a specific surface */
	inline unsigned int rawGetPixel(int iX, int iY) const
	{
		// Validate that the position is within the window
		if ((iX < 0) || (iX >= getSurfaceWidth()) || (iY < 0) || (iY >= getSurfaceHeight()))
			return 0;

		// Get the value of the pixel
		return ((unsigned int *)m_pSDLSurface->pixels)[iX + iY * getIntsPerWindowRow()];
	}


	// Draw a line
	void drawLine(double fX1, double fY1, double fX2, double fY2, unsigned int uiColour);

	// Draw a thick line
	void drawThickLine(double fX1, double fY1, double fX2, double fY2, unsigned int uiColour, int iThickness);

	// Draw a region which has vertical sides - one side may be a point, making a triangle
	void drawVerticalSidedRegion(
		double fX1, double fX2,
		double fY1a, double fY2a,
		double fY1b, double fY2b,
		unsigned int uiColour);

	// Draw a rectangle
	void drawRectangle(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour);

	// Draw an oval 
	void drawOval(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour);

	// Draw a hollow oval - i.e. just the edge
	void drawHollowOval(int iX1, int iY1, int iX2, int iY2,
		int iX3, int iY3, int iX4, int iY4,
		unsigned int uiColour);

	// Draw a polygon on the supplied surface
	void drawPolygon(int iPoints, double* pXArray, double* pYArray, unsigned int uiColour);

	/* Draw a string to the supplied surface - This is faster than the scalable string version */
	void drawFastString(int iX, int iY, const char* pText, unsigned int uiColour, Font* pFont);
	/* Draw a string to the supplied surface - this goes through the scaling functions so works even if display is rescaled */
	void drawScalableString(int iX, int iY, const char* pText, unsigned int uiColour, Font* pFont);

	/* Draw an arrow between two points, which can be shortened at either end by a number of pixels - was used to draw arcs between nodes in the past */
	void drawShortenedArrow(int iX1, int iY1, int iX2, int iY2, int iShortenedStart, int iShortenedEnd, unsigned int uiColour, int iThickness, int iHeadSize);

	void drawShortenedLine(int iX1, int iY1, int iX2, int iY2, int iShortenedStart, int iShortenedEnd, unsigned int uiColour, int iThickness);

	// Draw a triangle where all three points are specified.
	void drawTriangle(
		double fX1, double fY1,
		double fX2, double fY2,
		double fX3, double fY3,
		unsigned int uiColour);





	/* Number of ints per row of the window. */
	int getIntsPerWindowRow() const
	{
		return m_pSDLSurface->pitch / sizeof(unsigned int);
	}

	// Get the width of the surface, in pixels
	int getSurfaceWidth() const
	{
		return m_pSDLSurface->w;
	}

	// Get the height of the surface, in pixels
	int getSurfaceHeight() const
	{
		return m_pSDLSurface->h;
	}

	// Copy all pixels from one surface to this one
	void copyEntireSurface(DrawingSurface* pFrom);

	// Copy rectangle of pixels from a surface to this one
	void copyRectangleFrom(DrawingSurface* pFrom, int iX, int iY, int iWidth, int iHeight);

	// Constant for PI - used in GetAngle below.
	static const double MY_PI;

	/*
	Determine the angle (in radians)  of point 2 from point 1.
	Note that it also checks the quadrant, so you get a result from 0 to 2PI.
	Implemented as a template so you can use with ints, doubles, etc
	*/
	static double getAngle(double tX1, double tY1, double tX2, double tY2);


	/* Fill the surface with a specific colour */
	void fillSurface(unsigned int uiPixelColour)
	{
		SDL_Rect rect; rect.x = 0; rect.y = 0; rect.w = getSurfaceWidth(); rect.h = getSurfaceHeight();
		::SDL_FillRect(m_pSDLSurface, &rect, uiPixelColour);
	}

	//
	// Filter for points...
	// A filter can be applied to the surface to ensure that drawing is limited to various regions, or even to stretch or translate coordinates
	// Students who find C++ easy will find this to be powerful.
	// Students who are struggling with this should just ignore filters - they are only useful for advanced features anyway
	//

	// Set a filter
	void setDrawPointsFilter(FilterPoints* pFilter) { m_pFilter = pFilter; }

	// Retrieve current filter
	FilterPoints* getDrawPointsFilter() const { return m_pFilter; }

	/*
	Ask the display to convert an x click from a point on screen to point in the display coordinates. Does nothing if no filters are applied.
	For example, used to work out what pixel on the surface was clicked on when scaling is applied, or what pixel the top left of bottom right is.	
	*/
	int convertXClickRealToVirtual(int x)
	{
		if (m_pFilter)
			return m_pFilter->convertXClickRealToVirtual(x);
		else
			return x;
	}

	/*
	Ask the display to convert an x click from a point on screen to point in the display coordinates. Does nothing if no filters are applied.
	For example, used to work out what pixel on the surface was clicked on when scaling is applied, or what pixel the top left of bottom right is.
	*/
	int convertYClickRealToVirtual(int y)
	{
		if (m_pFilter)
			return m_pFilter->convertYClickRealToVirtual(y);
		else
			return y;
	}

	/*
	Ask the display to find out what position on the window a pixel would be shown at
	For example, used to work out where the top left of an image will appear
	*/
	int findXClickPositionFromPixel(int x)
	{
		if (m_pFilter)
			return m_pFilter->convertXPixelVirtualToReal(x);
		else
			return x;
	}

	/*
	Ask the display to find out what position on the window a pixel would be shown at
	For example, used to work out where the top left of an image will appear
	*/
	int findYClickPositionFromPixel(int y)
	{
		if (m_pFilter)
			return m_pFilter->convertYPixelVirtualToReal(y);
		else
			return y;
	}


private:
	// A filter can be applied to the surface to ensure that drawing is limited to various regions, or even to stretch or translate coordinates
	FilterPoints * m_pFilter;

public:
	//
	// SDL stuff - students can ignore these functions!
	//

	void mySDLLockSurface()
	{
		if (SDL_MUSTLOCK(m_pSDLSurface))
		{
			mySDLSurfaceLockedCount++;
			if (mySDLSurfaceLockedCount == 1)
				SDL_LockSurface(m_pSDLSurface);
		}
	}

	void mySDLUnlockSurface()
	{
		if (SDL_MUSTLOCK(m_pSDLSurface))
		{
			mySDLSurfaceLockedCount--;
			if (mySDLSurfaceLockedCount == 0)
				SDL_UnlockSurface(m_pSDLSurface);
		}
	}

	// Sometimes we need to unlock the surface in order to do stuff, such as a bitblit. This will unlock it if it was locked.
	void mySDLTempUnlockSurface()
	{
		if (SDL_MUSTLOCK(m_pSDLSurface))
		{
			if (mySDLSurfaceLockedCount > 0) // i.e. it is locked!
				SDL_UnlockSurface(m_pSDLSurface);
		}
	}

	// This looks at whether the surface should have been locked and if so relocks it now. 
	void mySDLTempRelockSurface()
	{
		if (SDL_MUSTLOCK(m_pSDLSurface))
		{
			if (mySDLSurfaceLockedCount > 0) // i.e. it is locked!
				SDL_LockSurface(m_pSDLSurface);
		}
	}

	// Copy the surface contents onto a texture, which will then get drawn to screen
	void mySDLUpdateTexture(SDL_Texture* pSDL2Texture)
	{
		SDL_UpdateTexture(pSDL2Texture, NULL, m_pSDLSurface->pixels, m_pSDLSurface->pitch);
	}

	// Remove the checks on bounds for deciding whether to draw shapes or not
	void removeBoundsCheck()
	{
		checkBoundsForDrawings = false;
	}

	// Add checks on bounds for deciding whether to draw shapes or not
	void addBoundsCheck(int top, int bottom, int left, int right)
	{
		checkBoundsForDrawings = false;
		m_iBoundsTop = top;
		m_iBoundsBottom = bottom;
		m_iBoundsLeft = left;
		m_iBoundsRight = right;
	}

private:
	int mySDLSurfaceLockedCount;
	SDL_Surface *m_pSDLSurface;
	BaseEngine* m_pCreatorEngine;

private:
	bool checkBoundsForDrawings;
	int m_iBoundsTop;
	int m_iBoundsBottom;
	int m_iBoundsLeft;
	int m_iBoundsRight;
};


