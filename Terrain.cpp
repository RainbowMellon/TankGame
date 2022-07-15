#include <SFML/Graphics.hpp>
#include <math.h>
#include "Terrain.h"

using namespace sf;


Terrain::Terrain()
{
	heightMap.setPrimitiveType(TriangleStrip);
	heightMap.resize(86);
}


Terrain::~Terrain()
{

}


void Terrain::setTerrain(bool top, int map)
{
	Color terTone1(145, 122, 68), terTone2(56, 40, 4);
	terDif = map*100;

	if (top == true)
	{
		heightMap;
	}
	else
	{
		//This will set the surface of the ground
		for (int i = 0; i < 43; i++)
		{
			heightMap[i * 2] = Vector2f(i * 25, getHeight(i * 25));
			heightMap[i * 2].color = terTone1;
		}

		//This will put the bottom on the bottom of the screen
		int count = 0; 
		for (int j = 1; j < 86; j += 2)
		{
			heightMap[j] = Vector2f(count * 25, 500); //to get the spacing, 25 pixles apart
			heightMap[j].color = terTone2;
			count++;
		}
	}
}


//this uses cosine and sine to make a terrain height map, and is this function: https://www.desmos.com/calculator/0vjli9bpld
float Terrain::getHeight(double xPos)
{ 
	return static_cast<__int64>(25*(0.5*cos((xPos + terDif)/50)+cos((0.6*xPos+terDif)/50)+(1/2)* sin((xPos + terDif)/50) + sin((xPos)/50) / 2 + cos((4 * xPos)/50) / 5)+400);
}


//This is the bottom step in drawing the terrain on the screen
VertexArray Terrain::getVertexArray()
{
	return heightMap;
}