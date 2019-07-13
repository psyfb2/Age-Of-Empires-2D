#include "header.h"
#include "SpriteSheetAnimation.h"



SpriteSheetAnimation::SpriteSheetAnimation(BaseEngine* engine, SpriteSheet* spriteSheet, int topY, int numOfFrames, int frameDelay, Direction moveDirection, bool loop) :
	engine(engine), spriteSheet(spriteSheet), topY(topY), numOfFrames(numOfFrames), frameDelay(frameDelay), moveDirection(moveDirection), loop(loop)
{
	frame = 0;
	initialTime = 0;
	timePassed = 0;

	done = false;
	firstDrawCall = false;
}

SpriteSheetAnimation::~SpriteSheetAnimation()
{
}

void SpriteSheetAnimation::draw(int currentMilliSeconds, int x, int y)
{
	if (!firstDrawCall) {
		firstDrawCall = true;
		initialTime = currentMilliSeconds;
	}
	timePassed = currentMilliSeconds - initialTime;

	if (!done && timePassed >= frameDelay) {
		// move to the next frame
		firstDrawCall = false;
		frame += 1;

		if (frame == numOfFrames) {
			if (loop) {
				frame = 0;
			}
			else {
				done = true;
				return;
			}
		}
	}

	if (moveDirection == Right) {
		spriteSheet->getImage().renderImageWithMask(engine->getForegroundSurface(),
			spriteSheet->getWidth() * frame, topY, x, y, spriteSheet->getWidth(), spriteSheet->getHeight());
	}
	else {
		spriteSheet->getImage().renderImageWithMask(engine->getForegroundSurface(),
			spriteSheet->getImage().getWidth() - (spriteSheet->getWidth() * (frame + 1)), topY, x, y, spriteSheet->getWidth(), spriteSheet->getHeight());
	}
	
}

void SpriteSheetAnimation::reset()
{
	initialTime = 0;
	timePassed = 0;

	done = false;
	firstDrawCall = false;

	frame = 0;
}

bool SpriteSheetAnimation::getDone() {
	return done;
}
