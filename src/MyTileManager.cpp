#include "header.h"
#include "MyTileManager.h"



MyTileManager::MyTileManager()
	: TileManager(20, 20, 15, 15) // 20x20 tile size and 15x15 tiles
{

}

void MyTileManager::virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int mapX, int mapY,
	int startPositionScreenX, int startPositionSceenY) const
{
	int mapValue = getMapValue(mapX, mapY);
	unsigned int color = (0x100000 * (mapX & 1)
		+ 0x001000 * (mapX & 2)
		+ 0x000010 * (mapX & 4)
		+ 0x010000 * (mapY & 1)
		+ 0x000100 * (mapY & 2)
		+ 0x000001 * (mapX & 4)) * (mapValue % 16);
	pSurface->drawOval(
		startPositionScreenX,
		startPositionSceenY,
		startPositionScreenX + getTileWidth() - 1,
		startPositionSceenY + getTileHeight() - 1,
		color);

}

MyTileManager::~MyTileManager()
{
}
