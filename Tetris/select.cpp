#include "select.h"
#include "input.h"
#include "tetris.h"
#include "includes.h"
#include "font.h"

#define MIN(x, y) ((x) > (y) ? (y) : (x))
#define MAX(x, y) ((x) < (y) ? (y) : (x))

int selTic = 0;
int selection = 0;

void selectTic() {
    selTic++;
    int col = selection % 5;
    int row = selection / 5;

    if ( KEYHELD( glob_rightKey ) )
        selection = MIN( col + 1, 4 ) + 5 * row;

    if ( KEYHELD( glob_leftKey ) )
        selection = MAX( col - 1, 0 ) + 5 * row;

    if ( KEYHELD( glob_downKey ) )
        selection = col + 5;

    if ( KEYHELD( glob_upKey ) )
        selection = col;

    if ( KEYHELD( glob_enterKey ) ) {
        tetrisInit( selection );
    }
}

void selectDraw() {
    glPushMatrix();

    glTranslatef( 112.0, 96.0, 0.0 );
    printString( "Select Level" );

    glTranslatef( 12.0, 24.0, 0.0 );
    printString( "0 1 2 3 4" );
    glTranslatef( 0.0, 16.0, 0.0 );
    printString( "5 6 7 8 9" );

    glPopMatrix();

    glColor3f( 1.0, 0.5, 0.0 );
    int col = selection % 5;
    int row = selection / 5;
    if ( ( selTic % 4 ) >= 2 ) {
        glRectd( 124.0 + col * 16.0, 120.0 + row * 16.0, 132.0 + col * 16.0, 128.0 + row * 16.0 );
    }
}
