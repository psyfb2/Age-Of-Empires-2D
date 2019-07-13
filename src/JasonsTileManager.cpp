#include "header.h"

// This is Jason's version of the MyTileManager class from Demo A.
// It may be a useful comparison for your own class?

#include "JasonsTileManager.h"



JasonsTileManager::JasonsTileManager()
	: TileManager( 20, 20, 15, 15 )
{
}


JasonsTileManager::~JasonsTileManager()
{
}

void JasonsTileManager::virtDrawTileAt(
	BaseEngine* pEngine, // We don't need this but maybe a student will so it is here to use if needed
	DrawingSurface* pSurface,
	int iMapX, int iMapY,
	int iStartPositionScreenX, int iStartPositionScreenY) const
{
	int iMapValue = getMapValue(iMapX, iMapY);
	unsigned int iColour = (0x100000 * (iMapX & 1)
		+ 0x001000 * (iMapX & 2)
		+ 0x000010 * (iMapX & 4)
		+ 0x010000 * (iMapY & 1)
		+ 0x000100 * (iMapY & 2)
		+ 0x000001 * (iMapX & 4)) * (iMapValue % 16);
	pSurface->drawOval (
		iStartPositionScreenX,	// Left
		iStartPositionScreenY,	// Top
		iStartPositionScreenX + getTileWidth() - 1,		// Right
		iStartPositionScreenY + getTileHeight() - 1,	// Bottom
		iColour);
}

