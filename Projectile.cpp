#include "Projectile.h"
#include <SFML/Graphics.hpp>

using namespace sf;


Projectile::Projectile()
{
	shell.setFillColor(Color::Black);
	shell.setRadius(4);
	shell.setOrigin(4, 4);
	shell.setPosition(-100, 50);
	setMomentum(Vector2f(0, 0));
}


Projectile::~Projectile()
{

}


CircleShape Projectile::getProjectile()
{
	return shell;
}


Vector2f Projectile::getMomentum()
{
	return momentum;
}


void Projectile::setMomentum(Vector2f newMomentum)
{
	momentum = newMomentum;
}


void Projectile::setPosition(Vector2f newPosition)
{
	shell.setPosition(newPosition);
}


void Projectile::updateMomentum(Vector2f force)
{
	momentum += force;
}


void Projectile::updatePosition()
{
	if (shell.getPosition().y < 600 && shell.getPosition().x > -100 && shell.getPosition().x < 1100)
		shell.move(momentum);
}