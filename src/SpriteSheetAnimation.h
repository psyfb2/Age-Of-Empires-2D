#pragma once
#include "BaseEngine.h"
#include "SpriteSheet.h"
#include "SpecialAbilityDeployerAI.h"

/* Animates a single row animation within a sprite sheet */
class SpriteSheetAnimation
{
public:
	SpriteSheetAnimation(BaseEngine* engine, SpriteSheet* spriteSheet, int topY, int numOfFrames, int frameDelay, Direction moveDirection, bool loop);
	~SpriteSheetAnimation();

	/* Draws an image on the spritesheet depending on the current time and topY coordinate given */
	void draw(int currentMilliSeconds, int x, int y);

	/* Resets animation to frame 1 */
	void reset();

	bool getDone();

private:
	BaseEngine* engine;
	SpriteSheet* spriteSheet;

	Direction moveDirection;

	int initialTime; // time at first draw call
	int timePassed;  // time since the initial time

	int topY;
	int frame;
	int numOfFrames;
	int frameDelay;

	bool loop;
	bool done;
	bool firstDrawCall;
};

