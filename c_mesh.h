#ifndef __C_MESH_H__
#define __C_MESH_H__

#include <cgl/core.h>
#include <core/corefile.h>
#include "engine.h"

class c_mesh: public c_node {
      public:
      s_vertex *vertex;
      s_vertex *normal;
      s_vertex *tangent,bitangent;
      s_tvertex *tvertex[MAXUVCHANNELS];
      s_face *face[MAXFACEGROUPS];
      int numfaces[MAXFACEGROUPS];
      int matid[MAXFACEGROUPS];
      int numtvertices[MAXUVCHANNELS];
      int numvertices;
      int numnormals;
      int numfacegroups;
      int numuvchannels;
      int displaylist_id,displaylist_id2;

      void Load(char *fn);
      void Build();
      void Draw();
      void Remove();

};

#endif
