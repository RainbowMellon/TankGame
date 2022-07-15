#include "TankGame.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>

using namespace sf;


TankGame::TankGame()
{
	srand(time(NULL));

	player1.setPos(Vector2f(50, 250));
	player2.setPos(Vector2f(950, 250));
	gravity.x = 0;
	gravity.y = 1.225f;
	lineToMouse.setPrimitiveType(Lines);
	lineToMouse.resize(2);
	projectileMomentumLine.setPrimitiveType(Lines);
	projectileMomentumLine.resize(2);
	lineToPlayer.setPrimitiveType(Lines);
	lineToPlayer.resize(2);
	wind = Vector2f((rand() % 5) * 0.1, 0);
	if (rand() % 2 == 1)
		wind.x *= -1;
}


TankGame::~TankGame()
{

}


void TankGame::setLevel(int level)
{
	terrainBottom.setTerrain(false, level);
}

//this is the middle step in drawing the terrain
VertexArray TankGame::getTerrain()
{
	return terrainBottom.getVertexArray();
}


RectangleShape TankGame::getPlayerBody(int player)
{
	Player piq; //player in question
	if (player == 1)
		piq = player1;
	else if (player == 2)
		piq = player2;
	return piq.getPlayerBody();
}


RectangleShape TankGame::getPlayerBarrel(int choice)
{
	Player piq; //player in question
	if (choice == 1)
		piq = player1;
	else if (choice == 2)
		piq = player2;
	return piq.getPlayerBarrel();
}


VertexArray TankGame::getDebugLine(int turn)
{
	lineToPlayer[0].position = getProjectile().getPosition();
	lineToPlayer[1].position = getPlayerBody(turn).getPosition();

	projectileMomentumLine[0].position = getProjectile().getPosition();
	projectileMomentumLine[1].position = getProjectile().getPosition() + shot.getMomentum();
	
	return lineToPlayer;
}


void TankGame::updateDebugLines(int turn)
{
	lineToPlayer[0].position = getProjectile().getPosition();
	lineToPlayer[1].position = getPlayerBody(turn).getPosition();

	projectileMomentumLine[0].position = getProjectile().getPosition();
	projectileMomentumLine[1].position = getProjectile().getPosition() + shot.getMomentum();
}


Vector2f TankGame::getWind()
{
	return wind;
}


void TankGame::setMousePosition(Vector2f newPos)
{
	mousePosition = newPos;
}


CircleShape TankGame::getProjectile()
{
	return shot.getProjectile();
}


//apply a gravity vector, and detect if it will send the player through the ground
void TankGame::applyNature()
{
	//if applying gravity does not put the player in the ground, 
	if (!(player1.getPos().y + player1.getMome().y >= terrainBottom.getHeight(player1.getPos().x)))
		//then add gravity to momentum vector
		player1.updateMome(gravity);

	//do the same for player2
	if (!(player2.getPos().y + player2.getMome().y >= terrainBottom.getHeight(player2.getPos().x)))
		player2.updateMome(gravity);

	//add gravity and wind to projectile
	if (isLanded() == false && !(shot.getProjectile().getPosition().x < -10 || shot.getProjectile().getPosition().x > 1010))
	{
		shot.updateMomentum(gravity);
		shot.updateMomentum(wind);
	}
	else if (shot.getProjectile().getPosition().x < -10 || shot.getProjectile().getPosition().x > 1010)
		shot.setMomentum(Vector2f(0, 0));

	

}


void TankGame::applyMomentum()
{
	//will the momentum send the player through the ground?
	if (player1.getPos().y + player1.getMome().y > terrainBottom.getHeight(player1.getPos().x))
	{
		//set position to the ground height, because instead it would go through the ground
		player1.setMome(Vector2f(player1.getMome().x, 0));
		player1.setPos(Vector2f(player1.getPos().x, terrainBottom.getHeight(player1.getPos().x)));
	}
	else
		player1.updatePos();

	//repeat for player 2
	if (player2.getPos().y + player2.getMome().y > terrainBottom.getHeight(player2.getPos().x))
	{
		player2.setMome(Vector2f(player2.getMome().x, 0));
		player2.setPos(Vector2f(player2.getPos().x, terrainBottom.getHeight(player2.getPos().x)));
	}
	else
		player2.updatePos();

	if (isLanded() == false)
	{
		shot.updatePosition();
	}
	else
		shot.updatePosition();
}


void TankGame::updateBarrels(int playerTurn)
{
	double newAngle = 0, arcTan = 0, arcSin = 0;
	//move barrels to tanks
	player1.updateBarrelPos();
	player2.updateBarrelPos();

	//create line to mouse to create slope for barrel angle
	if (playerTurn == 1)
		lineToMouse[0] = player1.getPlayerBarrel().getPosition();
	else if (playerTurn == 2)
		lineToMouse[0] = player2.getPlayerBarrel().getPosition();
	lineToMouse[1] = mousePosition;

	//copmlacated stuff to turn coordinates into an angle for the update function
	if (playerTurn == 1)
	{
		arcTan = atan((mousePosition.y - player1.getPlayerBarrel().getPosition().y) / (mousePosition.x - player1.getPlayerBarrel().getPosition().x));
		arcSin = asin((mousePosition.y - player1.getPlayerBarrel().getPosition().y) /
			sqrt(pow(mousePosition.x - player1.getPlayerBarrel().getPosition().x, 2) + pow(mousePosition.y - player1.getPlayerBarrel().getPosition().y, 2)));
	}
	else if (playerTurn == 2)
	{
		arcTan = atan((mousePosition.y - player2.getPlayerBarrel().getPosition().y) / (mousePosition.x - player2.getPlayerBarrel().getPosition().x));
		arcSin = asin((mousePosition.y - player2.getPlayerBarrel().getPosition().y) /
			sqrt(pow(mousePosition.x - player2.getPlayerBarrel().getPosition().x, 2) + pow(mousePosition.y - player2.getPlayerBarrel().getPosition().y, 2)));
	}

	if (arcTan > 0 && arcSin > 0)
		newAngle = arcTan;
	if (arcTan < 0 && arcSin > 0)
		newAngle = 3.14159 + arcTan;
	if (arcTan > 0 && arcSin < 0)
		newAngle = 3.14159 + arcTan;
	if (arcTan < 0 && arcSin < 0)
		newAngle = 2 * 3.14159 + arcTan;

	newAngle *= 180 / 3.14159;
	newAngle = -1 * (newAngle - 360);
	newAngle = 90 - newAngle;
	if (newAngle < 0)
		newAngle += 360;
	else if (newAngle > 360)
		newAngle -= 360;
	newAngle += 90;
	//std::cout << mousePosition.x << " " << mousePosition.y << " " << newAngle << std::endl;   debuging

	if (playerTurn == 1)
		player1.updateBarrelAngle(newAngle);
	else if (playerTurn == 2)
		player2.updateBarrelAngle(newAngle);
}


void TankGame::fire(int turn)
{
	//the position of the mouse relative to the player
	Vector2f mouseRelPlayer = changeCordOrigin(Vector2f(lineToMouse[1].position.x, lineToMouse[1].position.y), turn, false);
	
	//magnitude of lineToMouse
	float magOLTM = sqrt(pow(mouseRelPlayer.x, 2) + pow(mouseRelPlayer.y, 2));
	
	//Unit vector math for the lenght of the barrel
	Vector2f unitVector;
	unitVector = Vector2f(mouseRelPlayer.x / magOLTM, mouseRelPlayer.y / magOLTM);
	unitVector.x *= 25; //a unit vector has a magnitude of one, the barrels are 25 long
	unitVector.y *= 25;
	unitVector = changeCordOrigin(unitVector, turn, true);

	shot.setPosition(unitVector);

	shot.setMomentum(Vector2f(mouseRelPlayer.x / 10, mouseRelPlayer.y / 10));
}


bool TankGame::isLanded()
{	
	if (shot.getProjectile().getPosition().y + shot.getMomentum().y > terrainBottom.getHeight(shot.getProjectile().getPosition().x))
		return true;
	else
		return false;
}

Vector2f TankGame::changeCordOrigin(Vector2f originAtO, int turn, bool reverse)
{
	//change the origin of the coordinates
	Vector2f returnVector;
	if (reverse == false)
	{
		if (turn == 1)
			returnVector = originAtO - player1.getPos();
		else if (turn == 2)
			returnVector = originAtO - player2.getPos();
	}
	else if (reverse == true)//change it back
	{
		if (turn == 1)
			returnVector = originAtO + player1.getPos();
		else if (turn == 2)
			returnVector = originAtO + player2.getPos();
	}

	return returnVector;
}


void TankGame::resetProjectile(Vector2f newPoint)
{
	shot.setPosition(newPoint);
}


Vector2f TankGame::getProjectileMomentum()
{
	return shot.getMomentum();
}


bool TankGame::isPlayerHit(int turn)
{
	Vector2f projMomeLine1, projMomeLine2, topLeft, topRight, bottomLeft, bottomRight;
	bool hit = 0;
	projMomeLine1 = Vector2f(projectileMomentumLine[0].position.x, projectileMomentumLine[0].position.y);
	projMomeLine2 = Vector2f(projectileMomentumLine[1].position.x, projectileMomentumLine[1].position.y);
	topLeft = Vector2f(getPlayerBody(turn).getGlobalBounds().left, getPlayerBody(turn).getGlobalBounds().top);
	topRight = Vector2f(getPlayerBody(turn).getGlobalBounds().left + getPlayerBody(turn).getGlobalBounds().width, getPlayerBody(turn).getGlobalBounds().top);
	bottomLeft = Vector2f(getPlayerBody(turn).getGlobalBounds().left, getPlayerBody(turn).getGlobalBounds().top + getPlayerBody(turn).getGlobalBounds().height);
	bottomRight = Vector2f(getPlayerBody(turn).getGlobalBounds().left + getPlayerBody(turn).getGlobalBounds().width, getPlayerBody(turn).getGlobalBounds().top + getPlayerBody(turn).getGlobalBounds().height);

	if (doIntersect(projMomeLine1, projMomeLine2, topLeft, topRight) == 1)
		hit = 1;
	else if (doIntersect(projMomeLine1, projMomeLine2, topLeft, bottomLeft) == 1)
		hit = 1;
	else if (doIntersect(projMomeLine1, projMomeLine2, topRight, bottomRight) == 1)
		hit = 1;
	else
		hit = 0;

	return hit;
}


bool TankGame::onSegment(Vector2f p, Vector2f q, Vector2f r)
{
	// Given three collinear points p, q, r, the function checks if
	// point q lies on line segment 'pr'
	if (q.x <= fmax(p.x, r.x) && q.x >= fmin(p.x, r.x) &&
		q.y <= fmax(p.y, r.y) && q.y >= fmin(p.y, r.y))
		return true;

	return false;
}


int TankGame::orientation(Vector2f p, Vector2f q, Vector2f r)
{
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/
	// for details of below formula.
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0)
		return 0;  // collinear

	return (val > 0) ? 1 : 2; // clock or counterclock wise
}


bool TankGame::doIntersect(Vector2f p1, Vector2f q1, Vector2f p2, Vector2f q2)
{
	// Find the four orientations needed for general and
	// special cases
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases
	// p1, q1 and p2 are collinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and q2 are collinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are collinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are collinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases
}


Vector2f TankGame::explosionHit(int playerTurn, Vector2f explodePosition, Vector2f playerPos)
{
	double newAngle = 0, arcTan = 0, arcSin = 0;
	Vector2f eC = explodePosition, p1 = playerPos, unitVect40;

	//mousePosition == explosion center == explosionRadiusLine[0] == eC
	//player1.getPlayerBarrel().getPosition() == player1 position == explosionRadiusLine[1] == p1

	//coordinates to angle in radians
	arcTan = atan((eC.y - p1.y) / (eC.x - p1.x));
	arcSin = asin((eC.y - p1.y) / sqrt(pow(eC.x - p1.x, 2) + pow(eC.y - p1.y, 2)));
	//put the right andle as newAngle
	if (arcTan > 0 && arcSin > 0)
		newAngle = arcTan;
	if (arcTan < 0 && arcSin > 0)
		newAngle = 3.14159 + arcTan;
	if (arcTan > 0 && arcSin < 0)
		newAngle = 3.14159 + arcTan;
	if (arcTan < 0 && arcSin < 0)
		newAngle = 2 * 3.14159 + arcTan;
	//radians to degrees
	newAngle *= 180 / 3.14159;
	
	//normal angles, to weird angles(bearings)
	newAngle = -1 * (newAngle - 360);
	newAngle = 90 - newAngle;
	if (newAngle < 0)
		newAngle += 360;
	else if (newAngle > 360)
		newAngle -= 360;
	newAngle += 90;

	//turning an angle and a magnitude into a usable vector
	unitVect40 = Vector2f(40 * cos(newAngle), 40 * sin(newAngle));



	return getPlayerBody(playerTurn).getPosition() + unitVect40;
}