#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;


class Projectile
{
public:
	Projectile();
	~Projectile();
	CircleShape getProjectile();
	Vector2f getMomentum();
	void setMomentum(Vector2f); 
	void setPosition(Vector2f);
	void updateMomentum(Vector2f); 
	void updatePosition();
private:
	CircleShape shell;
	Vector2f momentum;
};

