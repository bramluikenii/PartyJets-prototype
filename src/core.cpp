#include <math.h>

#include "core.h"

float Vector::getLength(){
	float length = sqrt(
		pow(x,2) + 
		pow(y,2) + 
		pow(z,2)
	);
	return length;
}
void Vector::normalize(){
	float length = getLength();
	x = x/length;
	y = y/length;
	z = z/length;
}

void Vector::toEuler() //not used
{
	Vector angles;
	angles.z = tan(x/y);
	
	angles.x=0;
	angles.y=0;
	
	//return angles;
}
