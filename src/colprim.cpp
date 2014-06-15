#include "colprim.h"

bool ColPrim::checkCollision(Vector toCheck){
	return false;
}

bool ColPrimBox::checkCollision(Vector toCheck){
	if
	(
		toCheck.x - positionition.x < width/2 && toCheck.x - positionition.x > -width/2 &&
		toCheck.y - positionition.y < height/2 && toCheck.y - positionition.y > -height/2 &&
		toCheck.z - positionition.z < depth/2 && toCheck.z - positionition.z > -depth/2
	)
	{
		return true;
	}
	else
	{
		return false;	
	}
	
}
