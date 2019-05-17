#include "tile.h"
#include "includes.h"

const float BLANK_COLOR[3] = { 0.1f, 0.1f, 0.1f };

colorset_t COLORS[] =
  { { { 0x3C, 0xBC, 0xFC },{ 0x00, 0x78, 0xF8 } },
    { { 0xb8, 0xf8, 0x18 },{ 0x00, 0xB8, 0x00 } },
    { { 0xF8, 0x78, 0xF8 },{ 0x68, 0x44, 0xFC } },
    { { 0x58, 0xD8, 0x54 },{ 0x00, 0x78, 0xF8 } },
    { { 0x58, 0xF8, 0x98 },{ 0xD8, 0x00, 0xCC } },
    { { 0x68, 0x88, 0xFC },{ 0x58, 0xF8, 0x98 } },
    { { 0x7C, 0x7C, 0x7C },{ 0xE4, 0x00, 0x58 } },
    { { 0x94, 0x00, 0x84 },{ 0x00, 0x58, 0xF8 } },
    { { 0xD8, 0x00, 0xCC },{ 0x00, 0x78, 0xF8 } },
    { { 0xFC, 0xA0, 0x44 },{ 0xE4, 0x00, 0x58 } } };

void drawTile( colorid_t color, int colorset ) {
	colorset_t *usecolor;
	void *drawcolor;

	usecolor = &COLORS[colorset % 10];

	switch ( color ) {
	case COLOR_BLANK:
		glColor3fv( BLANK_COLOR );
		glRectf( 0, 0, TILE_SIZE, TILE_SIZE );
		return;
	case COLOR_ONE:
		drawcolor = &usecolor->color1;
		break;
	case COLOR_TWO:
		drawcolor = &usecolor->color2;
		break;
	case COLOR_BORDER:
	default:
		drawcolor = &usecolor->color2;
	}

	glColor3ubv( (const GLubyte *)drawcolor );
	glRectf( 0, 0, TILE_SIZE, TILE_SIZE );
	glColor3f( 1.0, 1.0, 1.0 );
	if ( color != COLOR_BORDER ) {
		glRectf( 0, 0, 1, 1 );
		glRectf( 1, 1, 3, 2 );
		glRectf( 1, 2, 2, 3 );
	}
	else {
		glRectf( 0, 0, 1, 1 );
		glRectf( 1, 1, TILE_SIZE - 1, TILE_SIZE - 1 );
	}
}
