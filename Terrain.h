#include <SFML/Graphics.hpp>
#pragma once

using namespace sf;


class Terrain
{
public:
	Terrain();
	~Terrain();
	void setTerrain(bool, int); //Give it top(true) or bottom(false), and the map(1-9)
	float getHeight(double); //Give the x coordinate
	VertexArray getVertexArray();

private:
	VertexArray heightMap;
	int terDif = 1; //the map(1 - 9)
};

