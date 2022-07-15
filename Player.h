#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;


class Player
{
public:
	Player();
	~Player();

	Vector2f getPos();
	double getHealth();
	RectangleShape getPlayerBody();
	RectangleShape getPlayerBarrel();
	Vector2f getMome();
	void setPos(Vector2f); // give the x coordinate
	void setHealth();
	void setBtmLRCoords(); //set bottom left right coordinates
	void setMome(Vector2f); //set momentum
	void updatePos(); 
	void updateBarrelAngle(double);
	void updateMome(Vector2f); //give a momentum vector
	void updateBarrelPos();

private:
	Clock clock;
	RectangleShape body, barrel;
	Vector2f mome, btmlPos, btmrPos; //momentum, bottom left and right of the body rectangle in global coordinates, for ground intersection
	double health = 0;
};

