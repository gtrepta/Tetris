#include <cstdint>
#include <cstring>
#include "includes.h"
#include "fontimage.h"

GLuint fontOffset;
GLuint texName;
uint32_t fontimage[];

// This technique for drawing fonts is given by the
// OpenGL red book (6th ed. p309)
void makeRasterFont( void ) {
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures( 1, &texName );
	glBindTexture( GL_TEXTURE_2D, texName );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 128, 48, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, fontimage );

	fontOffset = glGenLists( 128 );
	GLuint i, j, k, l;
	for ( i = 0, j = ' '; i < 16 * 6; i++, j++ ) {
		k = i % 16;
		l = i / 16;
		glNewList( fontOffset + j, GL_COMPILE );
		glBegin( GL_QUADS );
		glTexCoord2f( ( k * 8.0f ) / 128.0f, ( (5-l) * 8.0f ) / 48.0f ); glVertex2f( 0.0f, 8.0f );
		glTexCoord2f( ( k * 8.0f ) / 128.0f, ( (6-l) * 8.0f ) / 48.0f ); glVertex2f( 0.0f, 0.0f );
		glTexCoord2f( ( ( k + 1 ) * 8.0f ) / 128.0f, ( (6-l) * 8.0f ) / 48.0f ); glVertex2f( 8.0f, 0.0f );
		glTexCoord2f( ( ( k + 1 ) * 8.0f ) / 128.0f, ( (5-l) * 8.0f ) / 48.0f ); glVertex2f( 8.0f, 8.0f );
		glEnd();
		glTranslated( 8.0, 0.0, 0.0 );
		glEndList();
	}
}

void fontInit( void ) {
	glShadeModel( GL_FLAT );
	makeRasterFont();
}

void printString( const char *s ) {
	glPushAttrib( GL_LIST_BIT );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glEnable( GL_TEXTURE_2D );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glBindTexture( GL_TEXTURE_2D, texName );
	glListBase( fontOffset );

	glPushMatrix();
	glCallLists( strlen( s ), GL_UNSIGNED_BYTE, ( GLubyte * )s );
	glPopMatrix();

	glDisable( GL_TEXTURE_2D );
	glPopAttrib();
}

