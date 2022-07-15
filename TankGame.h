#include "Player.h"
#include "Terrain.h"
#include "Projectile.h"
#include <SFML/Graphics.hpp>
#pragma once


class TankGame
{
public:
	TankGame();
	~TankGame();
	void setLevel(int);
	VertexArray getTerrain();
	RectangleShape getPlayerBody(int);
	RectangleShape getPlayerBarrel(int);
	VertexArray getDebugLine(int);
	void updateDebugLines(int);
	CircleShape getProjectile();
	Vector2f getWind();
	void setMousePosition(Vector2f);
	void applyNature();
	void applyMomentum(); 
	void updateBarrels(int);
	void fire(int);
	bool isLanded();
	bool isPlayerHit(int); 
	Vector2f changeCordOrigin(Vector2f, int, bool);
	Vector2f getProjectileMomentum();
	void resetProjectile(Vector2f);
	bool onSegment(Vector2f, Vector2f, Vector2f);
	int orientation(Vector2f, Vector2f, Vector2f);
	bool doIntersect(Vector2f, Vector2f, Vector2f, Vector2f);
	Vector2f explosionHit(int, Vector2f, Vector2f);

private:
	Vector2f gravity, mousePosition, wind;
	VertexArray lineToMouse, projectileMomentumLine, lineToPlayer;
	Player player1, player2;
	Terrain terrainBottom;
	Projectile shot;
};

