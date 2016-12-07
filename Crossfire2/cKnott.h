/*
=================
cKnott.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CKNOTT_H
#define _CKNOTT_H
#include "cSprite.h"

class cKnott : public cSprite
{
private:
	SDL_Point knottVelocity;

public:
	cKnott();
	void update(double deltaTime);		// Rocket update method
	void setKnottVelocity(SDL_Point KnottVel);   // Sets the velocity for the knott
	SDL_Point getKnottVelocity();				 // Gets the knott velocity
};
#endif