#pragma once

/**********************
STOP!!!!
You don't need to understand most of what is in this file.
Instead go and look at the ImageObject class to see how to use it.
If you need more advanced features you can come back and look at SimpleImage.
************************/

#include <memory>

#include "RawImage.h"

class DrawingSurface;
class BaseEngine;




/*
SimpleImage version
SimpleImage requires that you specify the details of the drawing when you call the render functions.
ImageRenderer maintains this information within the object.
*/
class SimpleImage
{
public:
	// Default constructor - image does not exist
	SimpleImage();

	// Standard constructor taking a pointer to the image which is being used
	SimpleImage(std::shared_ptr<RawImageData> theData);

	// Determine whether the image was actually loaded correctly and exists for use
	bool exists() const;

	// Ask for the wisth of the image
	int getWidth() const { return theData->getWidth(); }

	// Ask for the height of the image
	int getHeight() const { return theData->getHeight(); }

	// Get the colour of a specified pixel, by x and y position
	int getPixelColour(int x, int y) const { return theData->getRawPixelColour(x, y); }

	// Draw the image to the specified surface
	void renderImage(DrawingSurface* pTarget,
		int iXSource, int iYSource,
		int iXTarget, int iYTarget,
		int iWidth, int iHeight) const;

	// Draw the image to the specified surface and assume that the bottom right pixel is the transparency colour.
	void renderImageWithMask(DrawingSurface* pTarget,
		int iXSource, int iYSource,
		int iXTarget, int iYTarget,
		int iWidth, int iHeight) const;

	// Draw the image to the specified surface using a specified colour as the transparency colour.
	void renderImageWithMask(DrawingSurface* pTarget,
		int iXSource, int iYSource,
		int iXTarget, int iYTarget,
		int iWidth, int iHeight,
		int iMaskColour ) const;

	/* 
	Draw an image to the screen, with a few other options. I would suggest to use the ImageRenderer class instead if you want to do this, it's more flexible and simpler to use:
	If you set the iAlternatePixelColour to other than -1 then every alternate pixel will be coloured in this colour.
	If you set the iEveryThirdPixelColour to other than -1 then every third pixel will be coloured in this colour.
	If you set the iEveryFourthPixelColour to other than -1 then every fourth pixel will be coloured in this colour.
	If you set iMergeColour to other than -1 then any remaining pixel colours will be averaged with this one - e.g. to make it more grey?
	If you set iBrightnessPercentage to other than 100 you can make the image darker or brighter (reduces or increases each of the RGB values). Note that each will be saturated at 255 though so going too bright can appear to change the colour.
	Setting iRotate90Degrees to 1 will rotate 90 degrees clockwise, to 2 will rotate 180 degrees and to 3 will rotate 270 degrees. Only values 0-3 have distinct meanings.
	e.g. ... 
		flexibleRenderImageWithMask( this->getBackgroundSurface(),
				0,0, // x and y source
				i * 100,j * 100 + 300, // x and y dest
				im.getWidth(),im.getHeight(), // Height and width to copy
				i + j, // Rotation value
				0, 0, // Transparency pixel - which pixel gives transparency colour, or use -1,-1 for no transparency
				100+(i-j)*20, // Brightness percentage
				-1, // 0x00ff00, // Alternate pixels
				-1, // 0x0000ff, // Every third pixel
				-1, // 0xffffff, // Every fourth pixel
				i == 0 ? -1 : 0x404040 * i // Average with this colour
				);
	*/
	void flexibleRenderImageWithMask(
		DrawingSurface* pTarget, // Target surface to render to
		int iXSource, int iYSource, // Coordinates in source to start draw from
		int iXTarget, int iYTarget, // Coordinates in dest to start draw to
		int iWidth, int iHeight, // Size of the area to copy - in the SOURCE not destination - since it may be swapped in destination.
		int iRotate90Degrees = 0, // Number of 90 degree rotations to apply
		int iTransparencyPixelX = -1, int iTransparencyPixelY = -1, // Position of the transparency pixel
		int iBrightnessPercentage = 100, // Percentage brightness to use
		int iColourNthPixel = -1, // An alternate pixel colour - set to -1 to not use otherwise you will get a crosshatch pattern
		int iThirdPixelColour = -1, // Pixel colour for each third pixel - set to -1 to not use
		int iFourthPixelColour = -1, // Pixel colour for each fourth pixel - set to -1 to not use
		int iMergeColour = -1 // Merge colour - set to -1 to not use, otherwise this will average with current pixels
	) const;

	/*
	This is a simpler rendering, to a specified size reduction rather than working out the pixels to draw.
	*/
	void simpleRender(BaseEngine* pEngine, DrawingSurface* pTarget, int iXTarget, int iYTarget,
		int iLeftInImage, int iTopInImage, int iWidthInImage, int iHeightInImage, int iSizeReduction = 1,
		int iTransparencyColour = -1) const;


	// Create a shrunk copy of another image - DOES NOT CACHE THE IMAGE FOR REUSE!!! See ImageManager for caching version
	SimpleImage shrinkBy(int iReductionMultiplier)
	{
		std::shared_ptr<RawImageData> newData( new RawImageData( theData, iReductionMultiplier) );
		return SimpleImage(newData);
	}

	// Create a re-sized copy of another image
	SimpleImage resizeTo(int iNewWidth, int iNewHeight)
	{
		std::shared_ptr<RawImageData> newData(new RawImageData(iNewWidth, iNewHeight, theData));
		return SimpleImage(newData);
	}

	// Should not need this. Provide a pointer to new raw image data to be stored in here.
	void setRawImageData(std::shared_ptr<RawImageData> data)
	{
		theData = data;
	}

	// Should not need this. Retrieve a pointer to the raw image data stored in here.
	std::shared_ptr<RawImageData> getRawImageData()
	{
		return theData;
	}

	// Get the raw data shared pointer - do not use unless you know what you are doing!
	const std::shared_ptr<RawImageData>& getTheData() { return theData; }

	// Get URL for current image
	const std::string getImageURL() { if (theData == nullptr) return ""; else return theData->getImageURL(); }

protected:
	// Shared pointer to the actual image data
	std::shared_ptr<RawImageData> theData;

};



