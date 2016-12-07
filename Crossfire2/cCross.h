/*
=================
cCross.h
- Header file for class definition - SPECIFICATION
- Header file for the Cross class which is a child of cSprite class
=================
*/
#ifndef _CCROSS_H
#define _CCROSS_H
#include "cSprite.h"

class cCross : public cSprite
{
private:
	SDL_Point crossVelocity;

public:
	cCross();
	void update(double deltaTime);		// Cross update method
	void setCrossVelocity(SDL_Point crossVel);   // Sets the velocity for the Cross
	SDL_Point getCrossVelocity();				 // Gets the Cross velocity
};
#endif