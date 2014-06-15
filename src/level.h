#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <string>

#include "core.h"
#include "projectile.h"
#include "colprim.h"

class Level
{
	public:
		//GameObjective* gameType;
		ColPrim* levelCollision[];
		//NavNode* navigation[];
		//Mesh mesh;

		void load(std::string levelFile);
		bool checkCollision(Vector toCheck);
};

#endif
