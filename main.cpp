// =============================================================================
// CGL - Basic Open GL tutorial
// =============================================================================
#include <stdio.h>
#include <math.h>
#include <gl/gl.h>
#include <cgl/cgl.h>
#include "engine.h"
#include "c_mesh.h"
#include "c_player.h"

#include "src/core.h"
#include "src/game.h"
#include "src/colprim.h"

#define WORLD_SCALE 50
#define ENEMY_COUNT 10

// objecten
c_player enemies[ENEMY_COUNT];
c_player player;// = &enemies[0];
Game game;

// assets graphics
c_mesh mesh_chopper,mesh_bullet;
c_mesh mesh_building1,mesh_sky,mesh_crate,mesh_floor;
s_bitmap bmp_floor,bmp_chopper,bmp_sky,bmp_building1,bmp_bullet;
s_bitmap bmp_logo, bmp_aim;
s_font font_game;

// game vars
float CameraX =0, CameraY = 2.0f, CameraZ = 15,CameraHeight=5;
float currentframe=0;
int multitexture;
int gameactive;
ColPrimBox levelcollision[4];
Vector levelnavigation[4];
int numbcolprims = 4;
bool screen = true;

void loadLevel()
{
	
	levelcollision[0].positionition.x = -2.81 *WORLD_SCALE;
	levelcollision[0].positionition.y = 12.981 *WORLD_SCALE;
	levelcollision[0].positionition.z = -7.483 *WORLD_SCALE;
	levelcollision[0].width=84.182 *WORLD_SCALE;
	levelcollision[0].height=25.963 *WORLD_SCALE;
	levelcollision[0].depth=33.472 *WORLD_SCALE;
	levelcollision[0].solid=false;
	levelcollision[0].index=1;
	
	levelcollision[1].positionition.x=-43.031 *WORLD_SCALE;
	levelcollision[1].positionition.y=30.952 *WORLD_SCALE;
	levelcollision[1].positionition.z=23.848 *WORLD_SCALE;
	levelcollision[1].width=46.67 *WORLD_SCALE;
	levelcollision[1].height=27.001 *WORLD_SCALE;
	levelcollision[1].depth=67.279 *WORLD_SCALE;
	levelcollision[1].solid=false;
	levelcollision[1].index=2;
	
	levelcollision[2].positionition.x=-3.955 *WORLD_SCALE;
	levelcollision[2].positionition.y=13.222 *WORLD_SCALE;
	levelcollision[2].positionition.z=52.841 *WORLD_SCALE;
	levelcollision[2].width=82.14 *WORLD_SCALE;
	levelcollision[2].height=26.098 *WORLD_SCALE;
	levelcollision[2].depth=31.567 *WORLD_SCALE;
	levelcollision[2].solid=false;
	levelcollision[2].index=3;
	
	levelcollision[3].positionition.x=23.368 *WORLD_SCALE;
	levelcollision[3].positionition.y=13.473 *WORLD_SCALE;
	levelcollision[3].positionition.z=23.131 *WORLD_SCALE;
	levelcollision[3].width=17.099 *WORLD_SCALE;
	levelcollision[3].height=17.099 *WORLD_SCALE;
	levelcollision[3].depth=30.936 *WORLD_SCALE;
	levelcollision[3].solid=false;
	levelcollision[3].index=4;
	
	
	//TODO: Scale navigation with level. Devide first. *WORLD_SCALE
	levelnavigation[0].x = -1431;
	levelnavigation[0].y = 1211;
	levelnavigation[0].z = 52;
	
	levelnavigation[1].x = -1111;
	levelnavigation[1].y = 1021;
	levelnavigation[1].z = 1962;
	
	levelnavigation[2].x = 770;
	levelnavigation[2].y = 697;
	levelnavigation[2].z = 2084;
	
	levelnavigation[3].x = 770;
	levelnavigation[3].y = 697;
	levelnavigation[3].z = 52;
	
}

bool checkWorldCollision(Vector toCheck)
{
	int topindex = -1;
	for(int i=0; i<numbcolprims; i++)
	{
		if( levelcollision[i].checkCollision(toCheck) == true && levelcollision[i].index > levelcollision[topindex].index )
		{
			topindex = i;
		}
	}
	if(topindex == -1 || levelcollision[topindex].solid == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SetCamera()
{
	glLoadIdentity();
	//CGL_LookAt(player.x, player.y*1.1, player.z-80,player.x,player.y,player.z); 
	
	if(screen == false){
		glTranslatef(0,-25,-100);
		
		//glRotatef(player.roll,0,0,1);
		glRotatef(player.pitch,1,0,0);
		glRotatef(player.yaw,0,1,0);
		
		glTranslatef(-player.position.x,-player.position.y,-player.position.z);
	}else{
		glTranslatef(0,0,-100);
		glRotatef(sin(currentframe/100)*45,1,0,0);
		glRotatef(player.yaw+currentframe,0,1,0);
		glTranslatef(-player.position.x,-player.position.y,-player.position.z);
	}
} 

void LoadGraphics()
{ 
    printf("- load meshes\n");    
    mesh_chopper.Load("shipreverse.c3m");
    mesh_bullet.Load("bullet.c3m");
    mesh_floor.Load("testlevel.c3m");
    mesh_crate.Load("platform.c3m");
    mesh_sky.Load("sky.c3m");
    mesh_building1.Load("building1.c3m");
    
    printf("- load textures\n");
    CGL_LoadBitmap("floor.tga",&bmp_floor);
    CGL_LoadBitmap("shiptex.tga",&bmp_chopper);
    CGL_LoadBitmap("laser.tga",&bmp_bullet);
    CGL_LoadBitmap("building1.tga",&bmp_building1);
    CGL_LoadBitmap("sky.tga",&bmp_sky);
    CGL_LoadBitmap("logo.tga",&bmp_logo);
    CGL_LoadBitmap("aim.tga",&bmp_aim);
}

void InitFog()
{
	glClearColor(0,0,0,1);
	float fogcolor[4]={0,0,0,1};
	glFogfv(GL_FOG_COLOR,fogcolor);
	glFogf(GL_FOG_START,100);
	glFogf(GL_FOG_END,3000);
	glFogf(GL_FOG_MODE,GL_LINEAR);
	glEnable(GL_FOG);

}

void Projection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	CGL_Perspective(60.0f, 1.33, 0.01f, 3000.0f);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}

void HandleEnemies(){
	for(int i=0; i<ENEMY_COUNT; i++){
    	enemies[i].Handle();
    	enemies[i].Draw();
    }
}

void DrawInterface(){
    glBegin(GL_LINES);
        glColor3f(1,1,1);
        glVertex3f( 0, 1,-1);
        glVertex3f( 0,-1,-1);
        glVertex3f( 1, 0,-1);
        glVertex3f(-1, 0,-1);
    glEnd();
}

void DrawFloor(){
  	glBindTexture(GL_TEXTURE_2D,bmp_floor.textureid);
    glScalef(WORLD_SCALE,WORLD_SCALE,WORLD_SCALE); //yes I did
    mesh_floor.Draw();
}

void menu(){
	int key,keytrig;
	CGL_GetKeys(&key, &keytrig);
	
	if(screen == true){
		CGL_DrawBitmap(80,0,bmp_logo);
		if (key & CGL_INPUT_KEY_BUTTONA){
			screen = false;
			player.Reset();
			player.type = PLAYER;
			CGL_SetMouse(320,240);
		}
		if (keytrig & CGL_INPUT_KEY_EXIT){
				gameactive=0;
		}
	}else{
		if (key & CGL_INPUT_KEY_EXIT){
			player.type = ENEMY;
			screen = true;
		}
		CGL_DrawBitmap(-48,-64+player.laserCharge*.64,bmp_bullet);
		player.bullet.CheckMaxDist();
		CGL_DrawBitmap(320-16,(240-16)+6000/player.bullet.maxDist,bmp_aim);
	}
}

// ======================= Main program ===========================
void coremain()
{
    corefile_mountimage("res",MOUNT_DIR);        
    CGL_InitVideo(640,480,CGL_VIDEO_NONE);

  	glEnable( GL_POLYGON_SMOOTH );
  	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
  	glEnable( GL_ALPHA_TEST );
  	
  	//screen = true; 
  	
	LoadGraphics();
	loadLevel();
    player.Init(-1);
    
    for(int i=0; i<ENEMY_COUNT; i++){
    	enemies[i].type=1;
    	enemies[i].Init(i);
    }
    
    player.type=ENEMY;
        
    printf("- start update loop\n");
    gameactive=1;
    glClearColor(0, 0, 0, 1.0);
    do {
        CGL_WaitRefresh(); // wacht op update
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        currentframe++;

		Projection();
		InitFog();
		SetCamera();
		DrawInterface();
		DrawFloor();
        player.Draw();
        player.Handle();
	    HandleEnemies();
	    menu();
	    
        CGL_SwapBuffers();
    } while (gameactive);
    CGL_CloseVideo();
}
