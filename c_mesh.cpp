#include <math.h>
#include <gl/gl.h>
#include <cgl/cgl.h>
#include "c_mesh.h"

void c_mesh::Draw()

{
    int f;
   
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    for (f=0;f<numfacegroups;f++) {
        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glNormalPointer(GL_FLOAT, 0, normal);
        glTexCoordPointer(2, GL_FLOAT, 0,tvertex[0]);
        glDrawElements(GL_TRIANGLES,numfaces[f]*3, GL_UNSIGNED_SHORT,face[f]);
        //glDrawElements(GL_LINE_LOOP,numfaces[f]*3, GL_UNSIGNED_SHORT,face[f]);
    }   
	glDisableClientState(GL_NORMAL_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void c_mesh::Build()
{
	
}

// delete
void c_mesh::Remove()

{
     int i;

     numvertices=0;numnormals=0;numuvchannels=0;numfacegroups=0;
     for (i=0;i<MAXFACEGROUPS;i++) wrap_free(face[i]);
     for (i=0;i<MAXUVCHANNELS;i++) wrap_free(tvertex[i]);
     wrap_free(normal);
     wrap_free(vertex);
}


void c_mesh::Load(char *fn)

{
     s_corefile f;
     s_face *facelist;
     s_tvertex *tvertex1;
     int id,flags;
     int numtfaces;
     int i,j;
     int version;
     unsigned short dummyw;

     numvertices=0;
     numfacegroups=0;
     numuvchannels=0;

     corefile_open(fn,&f,FILE_READ);

     // === Read ID ===
     corefile_read(&id,4,1,&f);
     corefile_read(&numvertices,2,1,&f);
     corefile_read(&numfacegroups,2,1,&f);
     corefile_read(&numuvchannels,2,1,&f);
     corefile_read(&flags,2,1,&f);
     version=1;if (flags & 32768) version=2; 

     printf("id: %c%c%c%c,verts: %d, facegroups: %d, uvchannels: %d, flags: %d, version: %d\n",id>>24,(id>>16)&255,(id>>8)&255,id&255,numvertices,numfacegroups,numuvchannels,flags,version);
     // === Read vertices ===
     vertex=(s_vertex*)wrap_malloc(numvertices*sizeof(s_vertex));
     for (i=0;i<numvertices;i++) {
         corefile_read(&vertex[i].x,4,1,&f);
         corefile_read(&vertex[i].y,4,1,&f);
         corefile_read(&vertex[i].z,4,1,&f);
     }
     // === read faces ===
     for (j=0;j<numfacegroups;j++) {
         corefile_read(&numfaces[j],2,1,&f);
         corefile_read(&matid[j],2,1,&f);
         face[j]=(s_face*)wrap_malloc(numfaces[j]*sizeof(s_face));
         facelist=(s_face*)face[j];
         //printf("group: %d, numfaces: %d\n",j,mesh->numfaces[j]);
         for (i=0;i<numfaces[j];i++) {
             corefile_read(&facelist[i].a,2,1,&f);
             corefile_read(&facelist[i].b,2,1,&f);
             corefile_read(&facelist[i].c,2,1,&f);
             corefile_read(&dummyw,2,1,&f);
             //      corefile_read(&facelist[i].flags,2,1,&f);
         }
     }
     if (version==2) { 

	    // normals
        corefile_read(&numnormals,2,1,&f);
        normal=(s_vertex*)wrap_malloc(numnormals*sizeof(s_vertex));
        printf("num normals: %d\n",numnormals);
        for (i=0;i<numnormals;i++) {
            corefile_read(&normal[i].x,4,1,&f);
            corefile_read(&normal[i].z,4,1,&f);
            corefile_read(&normal[i].y,4,1,&f);
            normal[i].z=-normal[i].z;
            //    printf("%i - normal: %f,%f,%f\n",i,normal[i].x,normal[i].y,normal[i].z);
        }  

	    // UV coords
        corefile_read(&numtvertices[0],2,1,&f);
        tvertex[0]=(s_tvertex*)wrap_malloc(numtvertices[0]*sizeof(s_tvertex));
        printf("num tvertices: %d\n",numtvertices[0]);
        for (i=0;i<numtvertices[0];i++) {
            corefile_read(&tvertex[0][i].u,4,1,&f);
            corefile_read(&tvertex[0][i].v,4,1,&f);
           // tvertex[0][i].v*=-1;
        }      
     }
     corefile_close(&f);
     Build();
}

