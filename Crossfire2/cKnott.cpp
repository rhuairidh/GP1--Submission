/*
=================
cKnott.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cKnott.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cKnott::cKnott() : cSprite()
{
	this->knottVelocity = { 0.0, 0.0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cKnott::update(double deltaTime)
{
	
	

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->getSpriteTranslation().x * deltaTime;
	currentSpritePos.y += this->getSpriteTranslation().y * deltaTime;

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	if (this->getSpritePos().y < 600)
	{
		cout << "Knotts position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	}
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
  Sets the velocity for the Knott
=================================================================
*/
void cKnott::setKnottVelocity(SDL_Point KnottVel)
{
	knottVelocity = KnottVel;
}
/*
=================================================================
  Gets the Knott velocity
=================================================================
*/
SDL_Point cKnott::getKnottVelocity()
{
	return knottVelocity;
}
