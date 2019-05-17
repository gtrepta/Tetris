#include <iostream>
#include <array>
#include "tetris.h"
#include "includes.h"
#include "input.h"
#include "block.h"
#include "font.h"

using std::array;

unsigned int gameTic = 0;
array<array<unsigned char, 10>, 20> board;
Block *currentBlock, *nextBlock;
extern int gameOverBit; // used to communicate game over to main

signed char lineClearIdx[5];
unsigned int animTics;
bool animationFlag = false;

struct {
    Uint32 score;
    Uint16 lines;
    unsigned char level;
} gameState;

void tetrisInit(int level) {
    gameTic = 0;

    animTics = 0;
    SDL_zero( lineClearIdx );
    lineClearIdx[4] = -1; // Delimiter value for iteration
    animationFlag = false;

    SDL_zero( gameState );
    SDL_zero( board );
    gameState.level = level;

    currentBlock = newBlock();
    nextBlock = newBlock();

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
}

void tetrisEnd() {
    delete currentBlock;
    delete nextBlock;
}

/*
int checkBlock()

Checks the current block's coordinates to see if it is either sitting out of
bounds or if it is overlapping any already occupied tiles

returns:
    0 - The block's position is OK
    1 - The block is either out of bounds or overlapping
    4 - Every square of the block is overlapping an occupied tile
        (game over)
*/
int checkBlock() {
    array<pos_t, 4> squares = currentBlock->getSquares();
    int result = 0;
    for ( int i = 0; i < 4; i++ ) {
        pos_t pos = squares[i];
        if ( pos.row >= 20 )
            return 1;
        if ( pos.col >= 10 || pos.col < 0 )
            return 1;
        if ( pos.row >= 0 && board[pos.row][pos.col] != 0 )
            result++;
    }
    return result;
}

/*
void putBlock()

Takes each square of the current block and "puts" it onto the board. The block
is guaranteed to be in an OK position as verified by checkBlock()
*/
void putBlock() {
    array<pos_t, 4> squares = currentBlock->getSquares();
    for ( int i = 0; i < 4; i++ ) {
        pos_t pos = squares[i];
        if ( pos.row >= 0) 
            board[pos.row][pos.col] = currentBlock->getColor();
    }
}

/*
void raiseBlock()

Takes each square of the current block and "lifts" it from the board. This is
called before manipulating the position of the block
*/
void raiseBlock() {
    array<pos_t, 4> squares = currentBlock->getSquares();
    for ( int i = 0; i < 4; i++ ) {
        pos_t pos = squares[i];
        if ( pos.row >= 0 )
            board[pos.row][pos.col] = 0;
    }
}

/*
void checkRows()

Scans for full rows, if it finds any, it lists them in the lineClearIdx array
and triggers the line-clearing animation
*/
void checkRows() {
    bool fullRow = true;
    unsigned char lines = 0;
    for ( int row = 0; row < 20; row++ ) {
        for ( int col = 0; col < 10; col++ ) {
            if ( board[row][col] == 0 ) {
                fullRow = false;
            }
        }

        if ( fullRow ) {
            animationFlag = true;
            animTics = 0;
            lineClearIdx[lines] = row;
            lines++;
        }
        fullRow = true;
    }

    lineClearIdx[lines] = -1;
}

/*
void DropnScore()

After full lines have been cleared, drops the remaining stack to the bottom of
the playfield and updates the score
*/
void DropnScore() {
    int lines;
    for ( lines = 0; lineClearIdx[lines] != -1; lines++ ) {
        int row = lineClearIdx[lines];
        for ( int itr = row; itr > 0; itr-- ) {
            board[itr].swap( board[itr - 1] );
        }
    }

    switch ( lines ) {
    case 1:
        gameState.score += 40 * ( gameState.level + 1 );
        break;
    case 2:
        gameState.score += 100 * ( gameState.level + 1 );
        break;
    case 3:
        gameState.score += 300 * ( gameState.level + 1 );
        break;
    case 4:
        gameState.score += 1200 * ( gameState.level + 1 );
        break;
    }

    gameState.lines += lines;
    if ( gameState.lines >= ( 10 * ( gameState.level + 1 ) ) ) {
        gameState.level++;
        return;
    }
}

/*
void animTic()

Short-circuits tetrisTic while full lines are being cleared out.
*/
void animTic() {
    int i;
    for ( i = 0; lineClearIdx[i] != -1; i++ ) {
        int row= lineClearIdx[i];
        int a = animTics / 8;
        board[row][4 - a] = 0;
        board[row][5 + a] = 0;
    }

    if ( i == 4 && !( animTics % 6 ) ) {
        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    } else if ( !( ( animTics + 1 ) % 4 ) ) {
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    }

    animTics++;

    if ( animTics > 32 ) {
        animationFlag = false;
        DropnScore();
    }
}

/*
void tetrisTic()

Reads player input and manipulates the current block
Checks for filled rows and game over whenever a block hits the bottom
*/
void tetrisTic() {
    static const unsigned char levelSpeed[30] =
        { 48, 43, 38, 33, 28, 23, 18, 13,  8,  6,
           5,  5,  5,  4,  4,  4,  3,  3,  3,  2,
           2,  2,  2,  2,  2,  2,  2,  2,  2,  1 };
    int bottomHit = 0;

    if ( animationFlag ) {
        animTic();
        return;
    }

    gameTic++;

    raiseBlock();
    if ( KEYHELD( glob_rotateLeftKey ) ) {
        currentBlock->rotateleft();
        if ( checkBlock() ) { // The block was moved to a bad position
            currentBlock->rotateright(); // So move it back
        }
    }

    if ( KEYHELD( glob_rotateRightKey ) ){
        currentBlock->rotateright();
        if ( checkBlock() ) {
            currentBlock->rotateleft();
        }
    }

    if ( KEYHELD( glob_rightKey ) ){
        currentBlock->move( 1, 0 );
        if ( checkBlock() ) {
            currentBlock->move(-1, 0);
        }
    }

    if ( KEYHELD( glob_leftKey ) ){
        currentBlock->move( -1, 0 );
        if ( checkBlock() ) {
            currentBlock->move(1, 0);
        }
    }

	if ( !( gameTic % levelSpeed[gameState.level]) ||
          ( glob_downKey && !( gameTic % 2 ) ) )
    {
		currentBlock->move( 0, 1 );
        if ( checkBlock() ) {
            currentBlock->move( 0, -1 );
            bottomHit = 1;
        }
	}
    putBlock();

	if ( bottomHit ) {
		delete currentBlock;
		checkRows();
		currentBlock = nextBlock;
		nextBlock = newBlock();

        if ( checkBlock() == 4 ) {
            gameOverBit = 1;
        }
	}
}

#define TILE_SIZE 8

void tetrisDraw() {

	int tileShift = TILE_SIZE + 1;
	glPushMatrix();
	glTranslatef( 115.0, 30.0, 0.0 );
	for ( int row = 0; row < 20; row++ ) {
		for ( int col = 0; col < 10; col++ ) {
			if ( board[row][col] == 0 ) {
				drawTile( COLOR_BLANK, 0 );
				glTranslatef( tileShift, 0.0, 0.0 );
			}
			else {
				drawTile( (colorid_t) board[row][col], gameState.level );
				glTranslatef( tileShift, 0.0, 0.0 );
			}
		}
		glTranslatef( tileShift * -10, tileShift, 0.0 );
	}
	glPopMatrix();

	char str[32];

	glPushMatrix();
	glTranslatef( 240.0, 20.0, 0.0 );
	printString( "Score" );
	glTranslatef( 0.0, 8.0, 0.0 );
	snprintf( str, 32, "%06d", gameState.score );
	printString( str );
	glPopMatrix();

	glPushMatrix();
	glTranslatef( 240.0f, 70.0f, 0.0f );
	printString( "Next" );
	glTranslatef( 0.0f, 8.0f, 0.0f );
	nextBlock->draw( gameState.level );
	glPopMatrix();

	glPushMatrix();
	glTranslatef( 240.0, 180.0, 0.0 );
	printString( "Level" );
	glTranslatef( 16.0, 8.0, 0.0 );
	snprintf( str, 32, "%02d", gameState.level );
	printString( str );
	glPopMatrix();

	glPushMatrix();
	glTranslatef( 115.0, 20.0, 0.0 );
	snprintf( str, 32, "Lines-%03d", gameState.lines );
	printString( str );
	glPopMatrix();
}

void tetrisPauseDraw() {
    glPushMatrix();
    glTranslatef( 150.0, 120.0, 0.0 );
    printString( "PAUSE" );
    glPopMatrix();
}

void tetrisGameOverDraw() {
    glPushMatrix();
    glTranslatef( 124.0, 120.0, 0.0 );
    printString( "GAME OVER" );
    glPopMatrix();
}
