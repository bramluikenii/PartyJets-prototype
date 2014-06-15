#include "bullet.h"

#include <math.h>

#include "core.h"

#define SHIP_RADIUS 200

void Bullet::draw(float pitch, float roll, float yaw, float length){
	glLoadIdentity();
	SetCamera();
	glTranslatef(position.x,position.y,position.z);
	glRotatef(-yaw,   0, 1, 0);
	glRotatef(-pitch, 1, 0, 0);
	glRotatef(-roll,  0, 0, 1);
	glScalef(1, 1, length);
	glBindTexture(GL_TEXTURE_2D, bmp_bullet.textureid);
	mesh_bullet.Draw();
}

float Bullet::CheckMaxDist(){
	Vector colLoc;
	float dist;
	
	colLoc = position;
	dist = 0;
	
	do {
		colLoc.x += direction.x;
		colLoc.y += direction.y;
		colLoc.z += direction.z;
		dist++;
	} while (dist<5000 && checkWorldCollision(colLoc)==false);
	
	maxDist = dist;
}

HitReturn Bullet::CheckShipCollision(Vector start, float pitch, float yaw, float roll, Vector col) 
{
	HitReturn toReturn;
	
	Vector checkPosition;
	Vector relCol;
	Vector relShip;
	float missedBy, colDist;
	
	//set ray, position and direction vectors	
	position = start;
	
	direction.x =  sin(pi /180 *yaw);
	direction.z = -cos(pi /180 *yaw);
	direction.y = -sin(pi /180 *pitch);
	
	//get relative position of ship(col)
	relCol.x = col.x - position.x;
	relCol.y = col.y - position.y;
	relCol.z = col.z - position.z;
	
	//get missedByance to ship
	colDist = relCol.getLength();
	
	//set check position on ray on distance to ship (this is also relative)
	checkPosition.x = direction.x *colDist;
	checkPosition.y = direction.y *colDist;
	checkPosition.z = direction.z *colDist;
	
	//calculate relative vector between check position and ship
	relShip.x = checkPosition.x - relCol.x;
	relShip.y = checkPosition.y - relCol.y;
	relShip.z = checkPosition.z - relCol.z;
	
	//calculate distance between check position and ship
	missedBy = relShip.getLength();
	printf(" missedBy: %d", missedBy);
	
	//return whether ship was hit
	toReturn.shipHit = false;
	if(colDist<maxDist){
		printf(" ship(%f) < wall(%f)", colDist, maxDist);
		if (missedBy < SHIP_RADIUS){
			toReturn.shipHit = true;
			toReturn.impactDistance = colDist; //hit ship
			printf(" VICTORY!");
		}else toReturn.impactDistance = maxDist; // ship missed, hit wall
	}else toReturn.impactDistance = maxDist; //ship behind wall, hit wall
	
	return toReturn;
}

