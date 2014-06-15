#ifndef COLPRIM_H
#define COLPRIM_H

#include "core.h"

class ColPrim // collision primitive
{
	public:
	Vector positionition;
	bool solid;
	int index;
	
	bool checkCollision(Vector toCheck);
};

class ColPrimBox : public ColPrim
{
	public:
	double width, height, depth; //xyz
	bool checkCollision(Vector toCheck);
};

#endif
