#include "header.h"

#include "ImageManager.h"
#include "DrawingSurface.h"

#include "BaseEngine.h"

/*
#ifdef WIN32
static void *jpeglib, *pnglib;
#endif

void InitImage()
{
#ifdef WIN32
	// PERF_IMPROVEMENT: Avoid SDL_Image to load/unload shared libs on every image
	// This also seems to fix a double load of the same file.
	jpeglib = SDL_LoadObject("jpeg.dll");
	pnglib = SDL_LoadObject("libpng12-0.dll");
#endif
}

void DeinitImage()
{
#ifdef WIN32
	SDL_UnloadObject(pnglib);
	SDL_UnloadObject(jpeglib);
#endif
}
*/




// Constructor
// Create an image by shrinking another one by a factor of iReductionMultiplier - pixel will average the reduced ones
RawImageData::RawImageData(const std::shared_ptr<RawImageData>& sourceImage, int iReductionMultiplier)
{
	m_iWidth = (sourceImage->m_iWidth) / iReductionMultiplier;
	m_iHeight = (sourceImage->m_iHeight) / iReductionMultiplier;
	m_aiPixels = new unsigned int[m_iWidth * m_iHeight];

	for (int x = 0; x < m_iWidth; x++)
		for (int y = 0; y < m_iHeight; y++)
			m_aiPixels[y * m_iWidth + x] = sourceImage->m_aiPixels[y * iReductionMultiplier * sourceImage->m_iWidth + x * iReductionMultiplier];
}


// Constructor
// Simple method to create an image by shrinking another one by a factor of three
// Copies from an existing raw image
// Specifies new width and height
// If sampling one pixel then 
RawImageData::RawImageData(int iNewWidth, int iNewHeight, const std::shared_ptr<RawImageData>& sourceImage)
{
	m_iWidth = iNewWidth;
	m_iHeight = iNewHeight;
	m_aiPixels = new unsigned int[m_iWidth * m_iHeight];

	int iSourceX, iSourceY;

	for (int y = 0; y < m_iHeight; y++)
		for (int x = 0; x < m_iWidth; x++)
		{
			iSourceX = (x * sourceImage->getWidth()) / m_iWidth;
			iSourceY = (y * sourceImage->getHeight()) / m_iHeight;
			m_aiPixels[y * m_iWidth + x] = sourceImage->m_aiPixels[iSourceY * sourceImage->getWidth() + iSourceX];
		}
}

