#include "header.h"

#include "ImageManager.h"
#include "DrawingSurface.h"

#include "BaseEngine.h"


// Singleton imagemanager if we want one rather than creating and maintaining one
ImageManager* ImageManager::g_pManager = nullptr;







// Default constructor - image does not exist
SimpleImage::SimpleImage()
	: theData(nullptr)
{
}

// Standard constructor taking a pointer to the image which is being used
SimpleImage::SimpleImage(std::shared_ptr<RawImageData> theData)
	: theData(theData)
{
}

// Determine whether the image was actually loaded correctly and exists for use
bool SimpleImage::exists() const { return theData != nullptr; }







void SimpleImage::renderImage( DrawingSurface* pTarget, 
							int iXSource, int iYSource, 
							int iXTarget, int iYTarget, 
							int iWidth, int iHeight ) const
{
	if ( iXTarget < 0 )
	{ // Then trim
		iWidth += iXTarget;
		iXSource -= iXTarget;
		iXTarget = 0;
	}

	if ( (iXTarget+iWidth) >= pTarget->getSurfaceWidth() )
	{ // Then trim
		iWidth = pTarget->getSurfaceWidth() - iXTarget;
	}

	if ( iYTarget < 0 )
	{ // Then trim
		iHeight += iYTarget; // Decrease height
		iYSource -= iYTarget; // Increase start point
		iYTarget = 0;
	}

	if ( (iYTarget+iHeight) >= pTarget->getSurfaceHeight())
	{ // Then trim
		iHeight = pTarget->getSurfaceHeight() - iYTarget;
	}

	int iXS, iYS = iYSource, iXT, iYT = iYTarget;
	//int iIntsPerScreenRow = pTarget->pitch/sizeof(unsigned int);

	for ( int iYOffset = 0 ; iYOffset < iHeight ; iYOffset++ )
	{
		iXS = iXSource; 
		iXT = iXTarget;
		for ( int iXOffset = 0 ; iXOffset < iWidth ; iXOffset++ )
		{
			pTarget->setPixel(iXT, iYT, theData->getRawPixelColour(iXS, iYS));
			//((unsigned int *)pTarget->pixels)[iXT + iYT * iIntsPerScreenRow] = theData->getPixelColour( iXS, iYS );
			iXS++;
			iXT++;
		}
		iYS++; 
		iYT++;
	}
}

void SimpleImage::renderImageWithMask(DrawingSurface* pTarget,
	int iXSource, int iYSource,
	int iXTarget, int iYTarget,
	int iWidth, int iHeight) const
{
	int iMaskColor = theData->getRawPixelColour(theData->getWidth() - 1, theData->getHeight() - 1);
	renderImageWithMask(pTarget, iXSource, iYSource, iXTarget, iYTarget, iWidth, iHeight, iMaskColor);
}

// Draw the image to the specified surface using a specified colour as the transparency colour.
void SimpleImage::renderImageWithMask(DrawingSurface* pTarget,
	int iXSource, int iYSource,
	int iXTarget, int iYTarget,
	int iWidth, int iHeight,
	int iMaskColor) const
{
	if ( iXTarget < 0 )
	{ // Then trim
		iWidth += iXTarget;
		iXSource -= iXTarget;
		iXTarget = 0;
	}

	if ( (iXTarget+iWidth) >= pTarget->getSurfaceWidth())
	{ // Then trim
		iWidth = pTarget->getSurfaceWidth() - iXTarget;
	}

	if ( iYTarget < 0 )
	{ // Then trim
		iHeight += iYTarget; // Decrease height
		iYSource -= iYTarget; // Increase start point
		iYTarget = 0;
	}

	if ( (iYTarget+iHeight) >= pTarget->getSurfaceHeight())
	{ // Then trim
		iHeight = pTarget->getSurfaceHeight() - iYTarget;
	}


	int iXS, iYS = iYSource, iXT, iYT = iYTarget;
	//int iIntsPerScreenRow = pTarget->pitch/sizeof(unsigned int);

	for ( int iYOffset = 0 ; iYOffset < iHeight ; iYOffset++ )
	{
		iXS = iXSource; 
		iXT = iXTarget;
		for ( int iXOffset = 0 ; iXOffset < iWidth ; iXOffset++ )
		{
			int iPixel = theData->getRawPixelColour(iXS, iYS);
			if ( iPixel != iMaskColor)
				pTarget->setPixel(iXT, iYT, iPixel);
			//((unsigned int *)pTarget->pixels)[iXT + iYT * iIntsPerScreenRow] = iPixel;
			iXS++;
			iXT++;
		}
		iYS++; 
		iYT++;
	}
}




/* Draw an image to the screen, with a few other options which I needed for a program I was working on so added to this file too:
If you set the iAlternatePixelColour to other than -1 then every alternate pixel will be coloured in this colour.
If you set the iEveryThirdPixelColour to other than -1 then every third pixel will be coloured in this colour.
If you set the iEveryFourthPixelColour to other than -1 then every fourth pixel will be coloured in this colour.

If you set iMergeColour to other than -1 then any remaining pixel colours will be averaged with this one - e.g. to make it more grey?

If you set iBrightnessPercentage to other than 100 you can make the image darker or brighter (reduces or increases each of the RGB values). Note that each will be saturated at 255 though so going too bright can appear to change the colour.

Setting iRotate90Degrees to 1 will rotate 90 degrees clockwise, to 2 will rotate 180 degrees and to 3 will rotate 270 degrees. Only values 0-3 have distinct meanings.
*/
void SimpleImage::flexibleRenderImageWithMask(
	DrawingSurface* pTarget, // Target surface to render to
	int iXSource,int iYSource, // Coordinates in source to start draw from
	int iXTarget,int iYTarget, // Coordinates in dest to start draw to
	int iWidth,int iHeight, // Size of the area to copy - in the SOURCE not destination - since it may be swapped in destination.
	int iRotate90Degrees, // Number of 90 degree rotations to apply
	int iTransparencyPixelX,int iTransparencyPixelY, // Position of the transparency pixel
	int iBrightnessPercentage, // Percentage brightness to use
	int iAlternatePixelColour, // An alternate pixel colour - set to -1 to not use otherwise you will get a crosshatch pattern
	int iThirdPixelColour, // Pixel colour for each third pixel - set to -1 to not use
	int iFourthPixelColour, // Pixel colour for each fourth pixel - set to -1 to not use
	int iMergeColour // Merge colour - set to -1 to not use, otherwise this will average with current pixels
	) const
{
	const int XOFFSET1[] = { 1, 0,-1, 0};
	const int YOFFSET1[] = { 0, 1, 0,-1};
	const int XOFFSET2[] = { 0,-1, 0, 1};
	const int YOFFSET2[] = { 1, 0,-1, 0};

	// Get the colour of the mask to apply, if any
	int iMask = -1; // None
	if ((iTransparencyPixelX > -1) && (iTransparencyPixelY > -1))
		iMask = theData->getRawPixelColour(iTransparencyPixelX, iTransparencyPixelY);

	int iXS,iYS = iYSource,iXT,iYT = iYTarget;
	//int iIntsPerScreenRow = pTarget->pitch / sizeof( unsigned int );
	int iPixel;
	double dR,dG,dB; 
	bool fColourSet = false;

	for ( int iYOffset = 0; iYOffset < iHeight; iYOffset++ )
	{
		iXS = iXSource;
		for ( int iXOffset = 0; iXOffset < iWidth; iXOffset++ )
		{
			// Start the other side of the area
			iXT = iXTarget + iWidth / 2 - iWidth * (XOFFSET1[iRotate90Degrees % 4] + XOFFSET2[iRotate90Degrees % 4]) / 2;
			iYT = iYTarget + iHeight / 2 - iHeight * (YOFFSET1[iRotate90Degrees % 4] + YOFFSET2[iRotate90Degrees % 4]) / 2;

			iXT += iXOffset * XOFFSET1[iRotate90Degrees % 4] + iYOffset * XOFFSET2[iRotate90Degrees % 4];
			iYT += iXOffset * YOFFSET1[iRotate90Degrees % 4] + iYOffset * YOFFSET2[iRotate90Degrees % 4];

			if ( iXT < 0 ) iXT = pTarget->getSurfaceWidth() - 1;
			if ( iXT >= pTarget->getSurfaceWidth() ) iXT = 0;
			if ( iYT < 0 ) iYT = pTarget->getSurfaceHeight() - 1;
			if ( iYT >= pTarget->getSurfaceHeight()) iYT = 0;

			iPixel = theData->getRawPixelColour(iXS, iYS); 
			if ( iPixel != iMask )
			{
				fColourSet = false; 

				if ( (iAlternatePixelColour != -1) && ((iXOffset + iYOffset) & 1) )
				{
					iPixel = iAlternatePixelColour;
					fColourSet = true;
				}
				
				if ( (iThirdPixelColour != -1) && ((iXOffset + iYOffset)%3 == 1) )
				{
					iPixel = iThirdPixelColour;
					fColourSet = true;
				}
				
				if ( (iFourthPixelColour != -1) && ((iXOffset + iYOffset) % 4 == 1) )
				{
					iPixel = iFourthPixelColour;
					fColourSet = true;
				}

				if ( !fColourSet) // Not an alternating pixel colour etc, so no colour set
				{
					if ( (iBrightnessPercentage != 100) || (iMergeColour != -1 ) )
					{
						// Alter brightness of RGB separately by the specified amount
						dR = ((0xFF & (iPixel >> 16)) * iBrightnessPercentage) / 100.0;
						dG = ((0xFF & (iPixel >> 8)) * iBrightnessPercentage) / 100.0;
						dB = ((0xFF & iPixel) * iBrightnessPercentage) / 100.0;

						if ( iMergeColour != -1 )
						{
							dR = (dR + (double)(iMergeColour>>16 & 0xff)) / 2.0;
							dG = (dG + (double)(iMergeColour>>8 & 0xff)) / 2.0;
							dB = (dB + (double)(iMergeColour & 0xff)) / 2.0;
						}

						// Catch saturation
						if ( dR > 255 ) dR = 255;
						if ( dG > 255 ) dG = 255;
						if ( dB > 255 ) dB = 255;

						iPixel = ((((int)(dR + 0.5)) & 0xFF) << 16)
							+ ((((int)(dG + 0.5)) & 0xFF) << 8)
							+ ((((int)(dB + 0.5)) & 0xFF));
					}
				}

				pTarget->setPixel(iXT,iYT,iPixel);
					//pixels)[iXT + iYT * iIntsPerScreenRow] = iPixel;
			}
			iXS++;
		}
		iYS++;
	}
}





/*
Simple render the image, no complex behaviour such as rotation etc
Needs to know: the base engine and drawing surface
The x and y location on the surface to draw to
The part of the image to draw - x and y of top left, and width and height of bit to draw
Any size reduction multiplier - e.g. 2 means treat it as half size, so skip alternate pixels
*/
void SimpleImage::simpleRender(BaseEngine* pEngine, DrawingSurface* pTarget, int iXDrawLocation, int iYDrawLocation,
	int iLeftInImage, int iTopInImage, int iWidthInImage, int iHeightInImage, int iSizeReduction,
	int iTransparencyColour ) const
{
	int xSrc, ySrc, iPixel;

	int iCurrentDrawWidth = iWidthInImage / iSizeReduction;
	int iCurrentDrawHeight = iHeightInImage / iSizeReduction;

	int iX0 = pEngine->convertRealToVirtualXPosition(0);
	int iY0 = pEngine->convertRealToVirtualYPosition(0);
	int iX1 = pEngine->convertRealToVirtualXPosition(1);
	int iY1 = pEngine->convertRealToVirtualYPosition(1);
	int iXMax = pEngine->convertRealToVirtualXPosition(pEngine->getWindowWidth());
	int iYMax = pEngine->convertRealToVirtualYPosition(pEngine->getWindowHeight());

	// Note: this method can also handle partial redraw of screen, by reducing the size of the draw area:
	if (!pEngine->getRedrawAllScreen())
	{
		// Not redrawing entire screen...
		int iRedrawPos = pEngine->getRedrawRectVirtualLeft();
		if (iRedrawPos > iX0)
			iX0 = iRedrawPos;
		iRedrawPos = pEngine->getRedrawRectVirtualTop();
		if (iRedrawPos > iY0)
			iY0 = iRedrawPos;
		iRedrawPos = pEngine->getRedrawRectVirtualRight();
		if (iRedrawPos < iXMax)
			iXMax = iRedrawPos;
		iRedrawPos = pEngine->getRedrawRectVirtualBottom();
		if (iRedrawPos < iYMax)
			iYMax = iRedrawPos;
	}

	//std::cout << "Render image: " << std::to_string(iX0) << "," << std::to_string(iY0) << " to "
	//	<< std::to_string(iXMax) << "," << std::to_string(iYMax) << "," << std::endl;

	// Trim to image to only draw the specified range of pixels
	int xMinOffset = 0;
	if (iXDrawLocation < iX0)
		xMinOffset = iX0 - iXDrawLocation; // Skip some pixels on left
										   // Do the same for top of screen
	int yMinOffset = 0;
	if (iYDrawLocation < iY0)
		yMinOffset = iY0 - iYDrawLocation; // Skip some pixels at the top

										   // If right hand side is off the end, then trim that side too.
	int xMaxOffset = iCurrentDrawWidth;
	if ((iXDrawLocation + xMaxOffset)/*RHS*/ > iXMax)
		xMaxOffset = iXMax - iXDrawLocation;
	// Do the same for bottom
	int yMaxOffset = iCurrentDrawHeight;
	if ((iYDrawLocation + yMaxOffset)/*RHS*/ > iYMax)
		yMaxOffset = iYMax - iYDrawLocation;

	// If the zoom level means multiple image pixels per screen pixel then skip the extra ones now by calculating the increment
	// Note: It's fine (but slower) if this is too low a number (>=1 though) but a problem if it is too high
	int xIncrement = iX1 - iX0; if (xIncrement < 1) xIncrement = 1;
	int yIncrement = iY1 - iY0; if (yIncrement < 1) yIncrement = 1;
	//int xIncrement = 1;
	//int yIncrement = 1;

	//std::cout << "ImageRenderer::simpleRender : draw  x " << std::to_string(xMinOffset) << " to " << std::to_string(xMaxOffset) << " , y " << std::to_string(yMinOffset) << " to " << std::to_string(yMaxOffset) << std::endl;
	//std::cout << "ImageRenderer::simpleRender : scale " << std::to_string(xIncrement) << " , " << std::to_string(yIncrement) << " , size reduction " << std::to_string(iSizeReduction) << std::endl;

	if ((xMinOffset >= xMaxOffset) || (yMinOffset >= yMaxOffset))
	{
		//std::cout << "** Nothing to do due to coordinates" << std::endl;
		return; // Nothing to do as nothing to draw
	}

	if (((iTopInImage + yMinOffset) % yIncrement) != 0)
		yMinOffset -= ((iTopInImage + yMinOffset) % yIncrement);
	if (((iLeftInImage + xMinOffset) % xIncrement) != 0)
		xMinOffset -= ((iLeftInImage + xMinOffset) % xIncrement);

	for (int iYOffset = yMinOffset; iYOffset < yMaxOffset; iYOffset += yIncrement)
	{
		for (int iXOffset = xMinOffset; iXOffset < xMaxOffset; iXOffset += xIncrement)
		{
			iPixel = 0xffffff & theData->getRawPixelColour(iLeftInImage + iXOffset * iSizeReduction, iTopInImage + iYOffset * iSizeReduction);
			if ((iTransparencyColour == -1) || (iPixel != iTransparencyColour))
			{
				pTarget->setPixel(iXDrawLocation + iXOffset, iYDrawLocation + iYOffset, iPixel);
			}
		}
	}
}


