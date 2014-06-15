#include <gl/gl.h>
#include <cgl/cgl.h>
#include "engine.h"


void glEnable2D( void )
{
	int iViewport[4];
	// Get a copy of the viewport
	glGetIntegerv( GL_VIEWPORT, iViewport );
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	// Set up the orthographic projection
	glOrtho( iViewport[0], iViewport[0]+iViewport[2],iViewport[1]+iViewport[3], iViewport[1], -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	
	glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
}

void glDisable2D( void )
{
	glPopAttrib();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}

void DrawQuad(float sx,float sy,float w,float h)
{
    glEnable2D();
      glBegin( GL_QUADS );
      glTexCoord2f(0,0.93);glVertex2f(sx,sy);
      glTexCoord2f(0,0);glVertex2f(sx,sy+h);
      glTexCoord2f(1,0);glVertex2f(sx+w,sy+h);
      glTexCoord2f(1,0.93);glVertex2f(sx+w,sy);
      glEnd();
   glDisable2D();
}
