#include "header.h"
#include "Psyfb2TileManager.h"



Psyfb2TileManager::Psyfb2TileManager(int tileWidth, int tileHeight, int numberOfXTiles, int numberOfYTiles)
	: TileManager(tileWidth, tileHeight, numberOfXTiles, numberOfYTiles)
{

}


Psyfb2TileManager::~Psyfb2TileManager()
{
}

/* Draw each tile as triangle */
void Psyfb2TileManager::virtDrawTileAt(BaseEngine * pEngine, DrawingSurface * pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const
{
	unsigned int color = getMapValue(iMapX, iMapY) % 256;
	pSurface->drawTriangle(iStartPositionScreenX, iStartPositionScreenY + (getTileHeight() - 1), iStartPositionScreenX + (getTileWidth() - 1) / 2, iStartPositionScreenY, iStartPositionScreenX + getTileWidth() - 1, iStartPositionScreenY + getTileHeight() - 1, color);
}
