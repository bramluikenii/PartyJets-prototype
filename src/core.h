#ifndef CORE_H
#define CORE_H

class Vector
{
	public:
	float x,y,z;
	
	float getLength();
	void normalize();
	void toEuler();
};

class Rotation //Not used or finished due to deadline
{
	public:
	float x,y,z,w;
	
	void reset(); //sets identity (localZ = 0,0,1f)
	void rotate(Vector axis, double angle);
	void addRotation(Rotation toAdd);
	Vector getRotationXYZ(); // gets axis of rotation past identity (0,0,1)
	float getRotationA(); // gets angle of rotation past identity
	Vector getRotationPYR(); // gets rotation in Euler angles [ P = Pitch = X | Y = Yaw = Y | R = Roll = Z ]
	//slerp here
};

class Node //to be inherited
{
	Vector positionition;
};

#endif
