#pragma once

#include <map>
#include <string>
#include <memory>

/**********************
STOP!!!!
You don't need to understand most of what is in this file.
Instead go and look at the ImageObject class to see how to use it.
If you need more advanced features you can then look at SimpleImage.
If you still need more information then you can look here.
************************/




/*
Class which deals with the data for a specified loaded image
You should not need to interact with this class - instead ask the ImageManager to make you a SimpleImage object, or create your own class (or sub-class of SimpleImage) to use this class.
*/
class RawImageData
{
protected:
	int m_iWidth; // Width of the loaded image
	int m_iHeight; // Height of the loaded image
	unsigned int* m_aiPixels;
	const std::string filename;

public:
	RawImageData(std::string filename)
		: m_iWidth(0), m_iHeight(0), m_aiPixels(NULL), filename(filename)
	{}

	~RawImageData()
	{
		if (m_aiPixels != NULL)
			delete[] m_aiPixels;
	}

	// Constructor
	// Create an image by shrinking another one by a factor of iReductionMultiplier - pixel will average the reduced ones
	RawImageData(const std::shared_ptr<RawImageData>& sourceImage, int iReductionMultiplier);

	// Constructor
	// Create an image by shrinking another one by a factor of three
	// Note: deliberate reorder of parameters compared with the one above, to disambiguate
	RawImageData(int iNewWidth, int iNewHeight, const std::shared_ptr<RawImageData>& sourceImage);


	// Ask for the width of the image
	int getWidth() const { return m_iWidth; }

	// Ask for the height of the image
	int getHeight() const { return m_iHeight; }

	// Get the colour of a specified pixel, by x and y position within the image
	int getRawPixelColour(int iX, int iY) const { return 0xffffff & (m_aiPixels[(iY%m_iHeight) * m_iWidth + (iX%m_iWidth)]); }

	// Function to work out the X coordinate to retrieve a point from given a rotation and cropping
	static int getXForRotationAndCrop(int iImageWidth, int iX, int iY, int iRotation, int iCropLeft, int iCropTop)
	{
		switch ((iRotation + 4) % 4)
		{
		default: return iX + iCropLeft;
		case 1: return iY + iCropTop;
		case 2: return iImageWidth - 1 - iX - iCropLeft;
		case 3: return iImageWidth - 1 - iY - iCropTop;
		}
	}

	// Function to work out the Y coordinate to retrieve a point from given a rotation and cropping
	static int getYForRotationAndCrop(int iImageHeight, int iX, int iY, int iRotation, int iCropLeft, int iCropTop)
	{
		switch ((iRotation + 4) % 4)
		{
		default: return iY + iCropTop;
		case 1: return iImageHeight - 1 - iCropLeft - iX;
		case 2: return iImageHeight - 1 - iY - iCropTop;
		case 3: return iX + iCropLeft;
		}
	}

	// Load the image with the specified filename
	bool loadImage(std::string strFileName, int num90degRotations = 0, int cropTop = 0, int cropBottom = 0, int cropLeft = 0, int cropRight = 0)
	{
		const char* szFileName = strFileName.c_str();

		char buf[128];

		// Free any current image data
		if (m_aiPixels != NULL)
		{
			delete[] m_aiPixels;
			m_aiPixels = NULL;
		}

		SDL_Surface* imagesurface = IMG_Load(szFileName);
		if (imagesurface != NULL)
		{
			{ // Make a 32 bit version
				SDL_Surface* newSurface = SDL_CreateRGBSurface(
					SDL_SWSURFACE, imagesurface->w, imagesurface->h, 32,
					0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
				SDL_BlitSurface(imagesurface, NULL, newSurface, NULL);

				SDL_FreeSurface(imagesurface);
				imagesurface = newSurface;
			}

			if ((num90degRotations % 2) == 1)
			{
				m_iWidth = imagesurface->h - cropTop - cropBottom;
				m_iHeight = imagesurface->w - cropLeft - cropRight;
			}
			else
			{
				m_iWidth = imagesurface->w - cropLeft - cropRight;
				m_iHeight = imagesurface->h - cropTop - cropBottom;
			}

			// Allocate memory
			m_aiPixels = new unsigned int[m_iWidth * m_iHeight];

			int iIntsPerScreenRow = imagesurface->pitch / sizeof(unsigned int);

			for (int iY = 0; iY < m_iHeight; iY++)
			{
				for (int iX = 0; iX < m_iWidth; iX++)
				{
					// Calculate which pixel to pull from the image
					m_aiPixels[iY * m_iWidth + iX]
						= (((unsigned int *)imagesurface->pixels)[getXForRotationAndCrop(imagesurface->w, iX, iY, num90degRotations, cropLeft, cropTop)
							+ getYForRotationAndCrop(imagesurface->h, iX, iY, num90degRotations, cropLeft, cropTop) * iIntsPerScreenRow]);
				}
			}
			SDL_FreeSurface(imagesurface);
			return true;
		}
		return false;
	}

	// Has an image been loaded correctly
	bool isLoaded() const { return m_aiPixels != NULL; }

	// Get the filename of the image file which was loaded - or resized!
	const std::string& getImageURL() { return filename; }
};


