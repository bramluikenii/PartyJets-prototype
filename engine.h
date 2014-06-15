#ifndef __ENGINE_H__
#define __ENGINE_H__

#define MAXUVCHANNELS 2
#define MAXFACEGROUPS 8


typedef struct {
    float x,y,z;
    } s_vector;
    
typedef struct {
    float x,y,z;
    } s_vertex;

typedef struct {
    float u,v;
    } s_tvertex;

typedef struct {
    unsigned short a,b,c;
    } s_face;

class c_node {
      protected:
      char name[20];
      float localmatrix[16];
      float globalmatrix[16];
      s_vector position,rot,scale;
      public:
      c_node(){};
      ~c_node(){};
      void GetName();
};

void DrawQuad(float sx,float sy,float w,float h);

#endif
