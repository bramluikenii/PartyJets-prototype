#include <math.h>
#include <gl/gl.h>
#include <cgl/cgl.h>
#include "c_mesh.h"

#include "src/core.h"
#include "src/bullet.h"

#define PLAYER 0
#define ENEMY 1

extern c_mesh mesh_chopper;
extern s_bitmap bmp_chopper;
extern int gameactive;
extern Vector levelnavigation[4];

extern void SetCamera();
extern bool checkWorldCollision(Vector toCheck);


class c_player {

	public:
	
	float pitch, yaw, roll;
	float angle,speed;  
	Vector position;
	float gravity;
	float bladeangle;
	int index;
	int type;
	int aipoint;
	bool dir;
	Bullet bullet;
	float laserCharge;
	float health;
	
	void Init(int myIndex);
	void input();
	void Reset();
	void Draw();
	bool shoot(bool justCheck);
	void Handle();
	void aidrive();
};

extern c_player enemies[10];

