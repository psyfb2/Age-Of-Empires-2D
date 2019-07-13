SDL_DIR = ../SDL/include

CC = g++

LIBS = -I$(SDL_DIR) -D_REENTRANT -lSDL2 -lSDL2_ttf -lSDL2_image

DEPS = BaseEngine.cpp BouncingBallMain.cpp BouncingBall.cpp DisplayableObject.cpp DraggingDemo.cpp DrawingSurface.cpp FontManager.cpp ImageObjectDemo.cpp Images.cpp JasonsDemoA.cpp JasonsObjectA.cpp JasonsTileManager.cpp MazeDemoMain.cpp MazeDemoObject.cpp MazeDemoTileManager.cpp PlayingCardsDemo.cpp RawImage.cpp SimpleDemo.cpp TileManager.cpp ZoomingDemo.cpp

OUTPUT = ./a.out

build:
	$(CC) mainfunction.cpp $(DEPS) $(LIBS) -o $(OUTPUT) -std=c++11

