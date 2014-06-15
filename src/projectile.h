#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "core.h"
#include "colprim.h"
#include "../c_mesh.h"

class Projectile
{
	Vector direction;
	double speed;
	int team;
	c_mesh mesh;
	int id;
	ColPrim col;
	
	void update();
};

#endif
