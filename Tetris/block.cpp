#include <iostream>
#include <random>
#include "includes.h"
#include "block.h"
#include "tile.h"

#define TILESHIFT (TILE_SIZE + 1)

Block *newBlock() {
    std::random_device rd;

    switch ( rd() % 7 ) {
    case 0:
        return new OBlock();
    case 1:
        return new JBlock();
    case 2:
        return new LBlock();
    case 3:
        return new TBlock();
    case 4:
        return new SBlock();
    case 5:
        return new ZBlock();
    case 6:
        return new IBlock();
    default:;
    }
}

void Block::move( int dCol, int dRow ) {
    std::array<pos_t, 4> newpos;

    std::copy( squares.begin(), squares.end(), newpos.begin() );

    for ( int i = 0; i < 4; i++ ) {
        newpos[i].col += dCol;
        newpos[i].row += dRow;
    }

    squares.swap( newpos );
}

////////////////
// O-BLOCK CODE
////////////////

const pos_t OBLOCK_START[4] =
{ { 4, 0 },{ 5, 0 },{ 4, 1 },{ 5, 1 } };

OBlock::OBlock() : Block() {
    std::copy( OBLOCK_START, OBLOCK_START + 4, squares.begin() );
    colorid = COLOR_BORDER;
}

void OBlock::draw( int colorset ) {
    glPushMatrix();
    glTranslatef( TILESHIFT * 1.0f, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( -TILESHIFT, TILESHIFT, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glPopMatrix();
}

////////////////
// J-BLOCK CODE
////////////////

const pos_t JBLOCK_START[4] =
{ { 4, 0 },{ 5, 0 },{ 6, 0 },{ 6, 1 } };

JBlock::JBlock() : Block() {
    std::copy( JBLOCK_START, JBLOCK_START + 4, squares.begin() );
    rotation = 0;
    colorid = COLOR_TWO;
}

void JBlock::rotateleft() {
    switch ( rotation ) {
    case 0:
        squares[0].col += 1;
        squares[0].row += 1;
        squares[2].col -= 1;
        squares[2].row -= 1;
        squares[3].row -= 2;
        rotation = 3;
        break;
    case 1:
        squares[0].col -= 1;
        squares[0].row += 1;
        squares[2].col += 1;
        squares[2].row -= 1;
        squares[3].col += 2;
        rotation = 0;
        break;
    case 2:
        squares[0].col -= 1;
        squares[0].row -= 1;
        squares[2].col += 1;
        squares[2].row += 1;
        squares[3].row += 2;
        rotation = 1;
        break;
    case 3:
        squares[0].col += 1;
        squares[0].row -= 1;
        squares[2].col -= 1;
        squares[2].row += 1;
        squares[3].col -= 2;
        rotation = 2;
        break;
    default:
        ;
    }
}

void JBlock::rotateright() {
    switch ( rotation ) {
    case 0:
        squares[0].col += 1;
        squares[0].row -= 1;
        squares[2].col -= 1;
        squares[2].row += 1;
        squares[3].col -= 2;
        rotation = 1;
        break;
    case 1:
        squares[0].col += 1;
        squares[0].row += 1;
        squares[2].col -= 1;
        squares[2].row -= 1;
        squares[3].row -= 2;
        rotation = 2;
        break;
    case 2:
        squares[0].col -= 1;
        squares[0].row += 1;
        squares[2].col += 1;
        squares[2].row -= 1;
        squares[3].col += 2;
        rotation = 3;
        break;
    case 3:
        squares[0].col -= 1;
        squares[0].row -= 1;
        squares[2].col += 1;
        squares[2].row += 1;
        squares[3].row += 2;
        rotation = 0;
        break;
    default:
        ;
    }
}

void JBlock::draw( int colorset ) {
    glPushMatrix();
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( 0.0f, TILESHIFT, 0.0f );
    drawTile( colorid, colorset );
    glPopMatrix();
}

////////////////
// L-BLOCK CODE
////////////////

const pos_t LBLOCK_START[4] =
{ { 5, 0 },{ 6, 0 },{ 4, 0 },{ 4, 1 } };

LBlock::LBlock() : Block() {
    std::copy( LBLOCK_START, LBLOCK_START + 4, squares.begin() );
    rotation = 0;
    colorid = COLOR_ONE;
}

void LBlock::rotateleft() {
    switch ( rotation ) {
    case 0:
        squares[1].col -= 1;
        squares[1].row -= 1;
        squares[2].col += 1;
        squares[2].row += 1;
        squares[3].col += 2;
        rotation = 3;
        break;
    case 1:
        squares[1].col += 1;
        squares[1].row -= 1;
        squares[2].col -= 1;
        squares[2].row += 1;
        squares[3].row += 2;
        rotation = 0;
        break;
    case 2:
        squares[1].col += 1;
        squares[1].row += 1;
        squares[2].col -= 1;
        squares[2].row -= 1;
        squares[3].col -= 2;
        rotation = 1;
        break;
    case 3:
        squares[1].col -= 1;
        squares[1].row += 1;
        squares[2].col += 1;
        squares[2].row -= 1;
        squares[3].row -= 2;
        rotation = 2;
        break;
    default:
        ;
    }
}

void LBlock::rotateright() {
    switch ( rotation ) {
    case 0:
        squares[1].col -= 1;
        squares[1].row += 1;
        squares[2].col += 1;
        squares[2].row -= 1;
        squares[3].row -= 2;
        rotation = 1;
        break;
    case 1:
        squares[1].col -= 1;
        squares[1].row -= 1;
        squares[2].col += 1;
        squares[2].row += 1;
        squares[3].col += 2;
        rotation = 2;
        break;
    case 2:
        squares[1].col += 1;
        squares[1].row -= 1;
        squares[2].col -= 1;
        squares[2].row += 1;
        squares[3].row += 2;
        rotation = 3;
        break;
    case 3:
        squares[1].col += 1;
        squares[1].row += 1;
        squares[2].col -= 1;
        squares[2].row -= 1;
        squares[3].col -= 2;
        rotation = 0;
        break;
    default:
        ;
    }
}

void LBlock::draw( int colorset ) {
    glPushMatrix();
    glTranslatef( 0.0f, TILESHIFT, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( 0.0f, -TILESHIFT, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glPopMatrix();
}

////////////////
// T-BLOCK CODE
////////////////

const pos_t TBLOCK_START[4] =
{ { 5, 0 },{ 4, 0 },{ 6, 0 },{ 5, 1 } };

TBlock::TBlock() : Block() {
    std::copy( TBLOCK_START, TBLOCK_START + 4, squares.begin() );
    rotation = 0;
    colorid = COLOR_BORDER;
}

void TBlock::rotateleft() {
    switch ( rotation ) {
    case 0:
        squares[1].col += 1;
        squares[1].row += 1;
        squares[2].col -= 1;
        squares[2].row -= 1;
        squares[3].col += 1;
        squares[3].row -= 1;
        rotation = 3;
        break;
    case 1:
        squares[1].col -= 1;
        squares[1].row += 1;
        squares[2].col += 1;
        squares[2].row -= 1;
        squares[3].col += 1;
        squares[3].row += 1;
        rotation = 0;
        break;
    case 2:
        squares[1].col -= 1;
        squares[1].row -= 1;
        squares[2].col += 1;
        squares[2].row += 1;
        squares[3].col -= 1;
        squares[3].row += 1;
        rotation = 1;
        break;
    case 3:
        squares[1].col += 1;
        squares[1].row -= 1;
        squares[2].col -= 1;
        squares[2].row += 1;
        squares[3].col -= 1;
        squares[3].row -= 1;
        rotation = 2;
        break;
    default:
        ;
    }
}

void TBlock::rotateright() {
    switch ( rotation ) {
    case 0:
        squares[1].col += 1;
        squares[1].row -= 1;
        squares[2].col -= 1;
        squares[2].row += 1;
        squares[3].col -= 1;
        squares[3].row -= 1;
        rotation = 1;
        break;
    case 1:
        squares[1].col += 1;
        squares[1].row += 1;
        squares[2].col -= 1;
        squares[2].row -= 1;
        squares[3].col += 1;
        squares[3].row -= 1;
        rotation = 2;
        break;
    case 2:
        squares[1].col -= 1;
        squares[1].row += 1;
        squares[2].col += 1;
        squares[2].row -= 1;
        squares[3].col += 1;
        squares[3].row += 1;
        rotation = 3;
        break;
    case 3:
        squares[1].col -= 1;
        squares[1].row -= 1;
        squares[2].col += 1;
        squares[2].row += 1;
        squares[3].col -= 1;
        squares[3].row += 1;
        rotation = 0;
        break;
    default:
        ;
    }
}

void TBlock::draw( int colorset ) {
    glPushMatrix();
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( -TILESHIFT, TILESHIFT, 0.0f );
    drawTile( colorid, colorset );
    glPopMatrix();
}

////////////////
// S-BLOCK CODE
////////////////

const pos_t SBLOCK_START[4] =
{ { 5, 0 },{ 6, 0 },{ 4, 1 },{ 5, 1 } };

SBlock::SBlock() : Block() {
    std::copy( SBLOCK_START, SBLOCK_START + 4, squares.begin() );
    rotation = 0;
    colorid = COLOR_ONE;
}

void SBlock::rotateleft() {
    switch ( rotation ) {
    case 0:
        squares[2].col += 1;
        squares[2].row -= 2;
        squares[3].col += 1;
        rotation = 1;
        break;
    case 1:
        squares[2].col -= 1;
        squares[2].row += 2;
        squares[3].col -= 1;
        rotation = 0;
        break;
    default:
        ;
    }
}

void SBlock::draw( int colorset ) {
    glPushMatrix();
    glTranslatef( 0.0f, TILESHIFT, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( 0.0f, -TILESHIFT, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glPopMatrix();
}

////////////////
// Z-BLOCK CODE
////////////////

const pos_t ZBLOCK_START[4] =
{ { 5, 0 },{ 5, 1 },{ 4, 0 },{ 6, 1 } };

ZBlock::ZBlock() : Block() {
    std::copy( ZBLOCK_START, ZBLOCK_START + 4, squares.begin() );
    rotation = 0;
    colorid = COLOR_TWO;
}

void ZBlock::rotateleft() {
    switch ( rotation ) {
    case 0:
        squares[2].col += 2;
        squares[3].row -= 2;
        rotation = 1;
        break;
    case 1:
        squares[2].col -= 2;
        squares[3].row += 2;
        rotation = 0;
        break;
    default:
        ;
    }
}

void ZBlock::draw( int colorset ) {
    glPushMatrix();
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( 0.0f, TILESHIFT, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glPopMatrix();
}

////////////////
// I-BLOCK CODE
////////////////

const pos_t IBLOCK_START[4] =
{ { 5, 0 },{ 3, 0 },{ 4, 0 },{ 6, 0 } };

IBlock::IBlock() : Block() {
    std::copy( IBLOCK_START, IBLOCK_START + 4, squares.begin() );
    rotation = 0;
    colorid = COLOR_BORDER;
}

void IBlock::rotateleft() {
    switch ( rotation ) {
    case 0:
        squares[1].col += 2;
        squares[1].row -= 2;
        squares[2].col += 1;
        squares[2].row -= 1;
        squares[3].col -= 1;
        squares[3].row += 1;
        rotation = 1;
        break;
    case 1:
        squares[1].col -= 2;
        squares[1].row += 2;
        squares[2].col -= 1;
        squares[2].row += 1;
        squares[3].col += 1;
        squares[3].row -= 1;
        rotation = 0;
        break;
    default:
        ;
    }
}

void IBlock::draw( int colorset ) {
    glPushMatrix();
    glTranslatef( 0.0f, TILESHIFT, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glTranslatef( TILESHIFT, 0.0f, 0.0f );
    drawTile( colorid, colorset );
    glPopMatrix();
}
