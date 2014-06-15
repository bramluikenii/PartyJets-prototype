#include "c_player.h"

#include "src/core.h"

#include <cstdlib> //srand

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

void c_player::Init(int myIndex)

{
	Reset();
	index = myIndex;
}

void c_player::Draw()

{
    // Draw - helicopter
    glLoadIdentity(); 
    SetCamera();
    glTranslatef(position.x,position.y,position.z);
    glRotatef(-yaw,0,1,0);
    glRotatef(-pitch,1,0,0);
    glRotatef(-roll,0,0,1);
    glBindTexture(GL_TEXTURE_2D,bmp_chopper.textureid);
    mesh_chopper.Draw();    
}

void c_player::Reset()
{
	int spawn = rand()%4;
	position = levelnavigation[spawn];
	if(rand() % 2 > 0){
		dir =true;
		aipoint=spawn+1;
	}else{
		dir=false;
		aipoint=spawn-1;
	}
	
	if (aipoint>3) aipoint = 0;
	if (aipoint<0) aipoint = 3;
	
	//same as aidrive()
	Vector desiredDir;
	desiredDir = levelnavigation[aipoint];
	
	desiredDir.x -= position.x;
	desiredDir.y -= position.y;
	desiredDir.z -= position.z;
	
	health = 100;
	
	//desired dir to euler
	yaw = atan2(desiredDir.x, -desiredDir.z) * 180/PI;
	float adj = sqrt(pow(desiredDir.x, 2) + pow(desiredDir.z, 2)); 
	pitch = -(atan2(adj, -desiredDir.y) * 180/PI)+90;
	laserCharge = 0;
}

void c_player::input()
{
	int key,keytrig;
	CGL_GetKeys(&key, &keytrig);
	if (key & CGL_INPUT_KEY_UP) speed=30;
	if (key & CGL_INPUT_KEY_DOWN) speed=5;
	if (key & CGL_INPUT_KEY_LEFT) roll-=2;
	if (key & CGL_INPUT_KEY_RIGHT) roll+=2;
	if (key & CGL_INPUT_KEY_BUTTONA)
	{
	    shoot(false);
	}
	
	int mouseX,mouseY,mouseBut;
	CGL_GetMouse(&mouseX,&mouseY,&mouseBut);
	pitch+=(mouseY-240)*.01;
	yaw+=(mouseX-320)*.01;
	roll=(mouseX-320)/3;
	//if (mouseBut & CGL_INPUT_MOUSEBUTTON_LEFT) printf("mb=%i \n",mouseBut); // DOESN'T WORK, WHY?
	
	
}

void c_player::aidrive()
{
	Vector desiredDir;
	desiredDir = levelnavigation[aipoint];
	
	desiredDir.x -= position.x;
	desiredDir.y -= position.y;
	desiredDir.z -= position.z;
	
	
	//desired dir to euler
	float dyaw = atan2(desiredDir.x, -desiredDir.z) * 180/PI;
	float padj = sqrt(pow(desiredDir.x, 2) + pow(desiredDir.z, 2)); 
	float dpitch = -(atan2(padj, -desiredDir.y) * 180/PI)+90;
	
	while (dyaw - yaw > 180) dyaw -= 360;
	while (dyaw - yaw < -180) dyaw += 360;

	//current dir to euler
	//desired dir in euler relative to current dir in euler
	//ajust dir as if by player's controls.
	
	pitch+=(dpitch - pitch)/15;
	yaw+=(dyaw - yaw)/15;
	roll=(dyaw - yaw) * 1;
	
	if (levelnavigation[aipoint].x - position.x < 20 && levelnavigation[aipoint].x - position.x > -20 &&
		levelnavigation[aipoint].y - position.y < 20 && levelnavigation[aipoint].y - position.y > -20 &&
		levelnavigation[aipoint].z - position.z < 20 && levelnavigation[aipoint].z - position.z > -20
		)
	{
		if(dir==true)aipoint++;
		if(dir==false)aipoint--;
		
		if (aipoint>3) aipoint = 0;
		if (aipoint<0) aipoint = 3;
		
		/*
		if(rand() % 10 > 8)
		{
			if(dir = false)dir =true;
			else dir=false;
		}*/
	}
	
	if(shoot(true))shoot(false);
}

bool c_player::shoot(bool justCheck)
{
	bool anyShipHit = false;
	int closestShipHit;
	float closestHitDist;
	
	if(laserCharge>=0 && health>0 && !justCheck)laserCharge-=3;
	if(laserCharge>0 && health>0 && !justCheck){
		bullet.CheckMaxDist();
		closestHitDist = bullet.maxDist;
		
		// Check point of impact
		for (int i=0; i<10; i++) {
			if (i!=index) {
				printf("\n%d...",i);
				HitReturn currentCheck = bullet.CheckShipCollision(position, pitch, yaw, roll, enemies[i].position);

				printf(" washit:%d",currentCheck.shipHit);
				if (currentCheck.impactDistance < closestHitDist) {
					closestHitDist = currentCheck.impactDistance;
					if(currentCheck.shipHit==true)
					{
						closestShipHit = i;
						anyShipHit = true;
					}
				}
			}
		}
		
		//Handle impact
		if(!justCheck){
			if(anyShipHit){
				printf("\nship_%d shot down ship_%d", index, closestShipHit);
				enemies[closestShipHit].health-=2;
			}
			bullet.draw(pitch, roll, yaw, closestHitDist);
		}
	}
	return anyShipHit;
}

void c_player::Handle()   //c_player *player
{
	c_player playerptr;
	float directionx,directionz;
	
	if (health>0){
		if( type == ENEMY ) aidrive();
		else if(type == PLAYER) input();
	} else {
		pitch++;//1.57
		roll+=8;	
	}
	
	
	
	// reken direction uit aan de hand van hoek/speed dmv sinus/cosinus
	directionx=sin(pi/180 * yaw) * speed;
	directionz=-cos(pi/180 * yaw) * speed;
	gravity=-sin(pi/180 * pitch) * speed;
	
	position.x+=directionx;
	position.y+=gravity;
	position.z+=directionz;
	gravity-=0.1;
	
	
	while (roll > 180) roll-=360;
	while (roll < -180) roll+=360;
	
	speed = 30 - position.y / 150;
	
	roll = roll * .995;

	
	if(laserCharge<100)laserCharge++;
	
	Vector colLoc;
	colLoc = position;
	if(checkWorldCollision(colLoc)==true) Reset();
	
}
