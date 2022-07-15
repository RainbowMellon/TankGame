#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace sf;


Player::Player()
{
	//random color set up
	Time time1 = clock.getElapsedTime();
	srand(time1.asMicroseconds()/2);
	Color color(rand() % 255, rand() % 255, rand() % 255);
	
	//body set up
	body.setSize(Vector2f(30, 10));
	body.setOrigin(15, 5);
	body.setFillColor(color);
	body.setOutlineThickness(1.f);
	body.setOutlineColor(Color::Black);

	//barrel set up
	barrel.setSize(Vector2f(25, 4));
	barrel.setOrigin(25, 2);
	barrel.setPosition(body.getPosition());
	barrel.setRotation(90);
	barrel.setFillColor(color);
	barrel.setOutlineThickness(1.f);
	barrel.setOutlineColor(Color::Black);

	setBtmLRCoords();
}


Player::~Player()
{

}

Vector2f Player::getPos()
{
	return body.getPosition();
}


double Player::getHealth()
{
	return 0;
}


RectangleShape Player::getPlayerBody()
{
	return body;
}


RectangleShape Player::getPlayerBarrel()
{
	return barrel;
}


Vector2f Player::getMome()
{
	return mome;
}


void Player::setPos(Vector2f newPos)
{
	body.setPosition(newPos);
}


void Player::setHealth()
{

}


void Player::setBtmLRCoords()
{
	btmlPos.x = body.getPosition().x - body.getSize().x / 2;
	btmlPos.y = body.getPosition().y + body.getSize().y / 2;

	btmrPos.x = body.getPosition().x + body.getSize().x / 2;
	btmrPos.y = body.getPosition().y + body.getSize().y / 2;
}


void Player::setMome(Vector2f newMome)
{
	mome = newMome;
}


void Player::updatePos()
{
	body.move(mome);
}


void Player::updateBarrelAngle(double newAngle)
{
	barrel.setRotation(newAngle);
}


void Player::updateMome(Vector2f newMome)
{
	mome += newMome;
}


//move the barrel to the tank
void Player::updateBarrelPos()
{
	barrel.setPosition(body.getPosition());
}