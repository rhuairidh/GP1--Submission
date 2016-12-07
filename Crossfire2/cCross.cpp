/*
=================
cCross.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cCross.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cCross::cCross() : cSprite()
{
	this->crossVelocity = { 0.0f, 0.0f };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cCross::update(double deltaTime)
{

	FPoint direction = { 0.0f, 0.0f };
	direction.X = (sin(this->getSpriteRotAngle()*PI / 180));
	direction.Y = -(cos(this->getSpriteRotAngle()*PI / 180));
	
	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->crossVelocity.x * deltaTime;
	currentSpritePos.y += this->crossVelocity.y * deltaTime;

	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y  });
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
Sets the velocity for the cross
=================================================================
*/
void cCross::setCrossVelocity(SDL_Point crossVel)
{
	crossVelocity = crossVel;
}
/*
=================================================================
Gets the cross velocity
=================================================================
*/
SDL_Point cCross::getCrossVelocity()
{
	return crossVelocity;
}


