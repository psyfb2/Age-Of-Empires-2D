#include "header.h"
#include "BallBouncer.h"
#include "CollisionDetection.h"

BallBouncer::BallBouncer(BaseEngine* engine, int centreX, int centreY, int radius, unsigned int color, Psyfb2TileManager* triangleTM, int* otherBallIndexes, int otherBallIndexesSize)
	: SpecialAbilityDeployer(engine, centreX, centreY, radius, color, "B", 0, 0, 1280), triangleTM(triangleTM), otherBallIndexes(otherBallIndexes), otherBallIndexesSize(otherBallIndexesSize)
{
	xSpeed = 4;
	ySpeed = 3;
}


BallBouncer::~BallBouncer()
{
}

// Change the appearance of this object
void BallBouncer::virtDraw()
{
	if (!isVisible()) {
		return;
	}

	unsigned int textColor = 0xff * (0x010101 - color);

	// m_iStartDrawPos is the offest (negative radius / 2)
	int topLeftX = m_iCurrentScreenX + m_iStartDrawPosX;
	int topLeftY = m_iCurrentScreenY + m_iStartDrawPosY;
	getEngine()->drawForegroundOval(topLeftX, topLeftY, topLeftX + m_iDrawWidth, topLeftY + m_iDrawHeight, color);
	// pick three random points on the circumfrence of the circle
	int xPoints[3];
	int yPoints[3];
	for (int i = 0; i < 3; i++) {
		generateRandomCoordOnCircumfrence(&xPoints[i], &yPoints[i]);
	}
	getEngine()->drawForegroundTriangle(xPoints[0], yPoints[0], xPoints[1], yPoints[1], xPoints[2], yPoints[2], 0xffffff);

	getEngine()->drawForegroundString(m_iCurrentScreenX + textOffset, m_iCurrentScreenY + textOffset, text, textColor);
}

void BallBouncer::generateRandomCoordOnCircumfrence(int *x, int *y) {
	float angle = ((float)rand() / RAND_MAX) * M_PI * 2;
	*x = cos(angle)*(m_iDrawWidth / 2) + m_iCurrentScreenX;
	*y = sin(angle)*(m_iDrawWidth / 2) + m_iCurrentScreenY;
}

void BallBouncer::virtDoUpdate(int currentTime) {
	if (getEngine()->isPaused() || !isVisible()) {
		return;
	}

	if (m_iCurrentScreenX < -100 || m_iCurrentScreenX > 1400 || m_iCurrentScreenY < -100 || m_iCurrentScreenY > 900) {
		m_iCurrentScreenX = 400;
		m_iCurrentScreenY = 400;
	}

	// make the ball bounce
	m_iCurrentScreenX += xSpeed;
	m_iCurrentScreenY += ySpeed;
	
	if (!isInBorders()) { // checks x borders
		m_iCurrentScreenX -= 3 * xSpeed;
		m_iCurrentScreenY -= ySpeed;
		xSpeed = -xSpeed;
		if (rand() % 2 && xSpeed < 8) {
			xSpeed = xSpeed * 2;
		}
		else if (xSpeed > 2){
			xSpeed = xSpeed / 2;
		}
	}
	if (m_iCurrentScreenY + m_iStartDrawPosY < 200 || m_iCurrentScreenY - m_iStartDrawPosY > 720) { // check y borders
		m_iCurrentScreenX -= xSpeed;
		m_iCurrentScreenY -= 3 * ySpeed;
		ySpeed = -ySpeed;
		if (rand() % 2 && ySpeed < 8) {
			ySpeed = ySpeed * 2;
		}
		else if (ySpeed > 2) {
			ySpeed = ySpeed / 2;
		}
	}

	// check for collisions with the other ball bouncer
	for (int i = 0; i < otherBallIndexesSize; i++) {
		BallBouncer* b = dynamic_cast<BallBouncer *>( getEngine()->getDisplayableObject(otherBallIndexes[i]) );
		if (CollisionDetection::checkCircles(m_iCurrentScreenX, m_iCurrentScreenY, b->getXCentre(), b->getYCentre(), (m_iDrawWidth / 2) + (b->getDrawWidth() / 2))) {
			// there is a collision between these two circles so reverse them both
			m_iCurrentScreenX -= 3 * xSpeed + 1;
			m_iCurrentScreenY -= 3 * ySpeed + 1;
			xSpeed = -xSpeed;
			ySpeed = -ySpeed;
		}

	}

	if (triangleTM != nullptr) {
		// if any point on this ball bouncers circumfrence touches tile manager then change colour of tile
		for (double i = 0; i < M_PI * 2; i += 0.5) {
			int x = cos(i)*(m_iDrawWidth / 2) + m_iCurrentScreenX;
			int y = sin(i)*(m_iDrawWidth / 2) + m_iCurrentScreenY;
			if (triangleTM->isValidTilePosition(x, y)) {
				int tileX = triangleTM->getMapXForScreenX(x);
				int tileY = triangleTM->getMapYForScreenY(y);
				int color =   0xff * (0x010101 - triangleTM->getMapValue(tileX, tileY)); // Change colour of the tile to opposite colour
				triangleTM->setAndRedrawMapValueAt(tileX, tileY, color, getEngine(), getEngine()->getBackgroundSurface());
			}
		}
	}
	redrawDisplay();
}

void BallBouncer::setXSpeed(double xSpeed)
{
	if (xSpeed <= 10 && xSpeed >= -10) {
		this->xSpeed = xSpeed;
	}
}

void BallBouncer::setYSpeed(double ySpeed)
{
	if (ySpeed <= 10 && ySpeed >= -10) {
		this->ySpeed = ySpeed;
	}
}

double BallBouncer::getXSpeed()
{
	return xSpeed;
}

double BallBouncer::getYSpeed()
{
	return ySpeed;
}

