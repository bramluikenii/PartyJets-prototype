#include <math.h>
#include <gl/gl.h>
#include <cgl/cgl.h>
#include "../c_mesh.h"

#include "core.h"
//#include "c_player.h"

#define MAXDIST	100000

extern c_mesh mesh_bullet;
extern s_bitmap bmp_bullet;
extern int gameactive;
extern bool checkWorldCollision(Vector toCheck);
extern void SetCamera();

struct HitReturn {
	float impactDistance;
	bool shipHit;
};

class Bullet {
	public:
	Vector position;
	Vector direction;
	float maxDist;
		
	void draw(float pitch,float roll,float yaw,float length);
	float CheckMaxDist();
	HitReturn CheckShipCollision(Vector start, float pitch, float yaw, float roll, Vector col);
};
