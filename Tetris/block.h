#include <array>
#include "tile.h"

#ifndef BLOCK_H
#define BLOCK_H

typedef struct {
    signed char col, row;
} pos_t;

/*
class Block

Stores information about a block's location, color, and how it moves.
*/
class Block {
public:
    Block() {}
    virtual void rotateleft() = 0;
    virtual void rotateright() = 0;
    virtual void draw( int colorset ) = 0;
    void move( int x, int y );
    std::array<pos_t, 4> getSquares() { return squares; }
    colorid_t getColor() { return colorid; }

protected:
    std::array<pos_t, 4> squares;
    colorid_t colorid;
};

Block *newBlock();

class OBlock : public Block {
public:
    OBlock();
    virtual void rotateleft() {}
    virtual void rotateright() {}
    virtual void draw( int colorset );
};

class JBlock : public Block {
public:
    JBlock();
    virtual void rotateleft();
    virtual void rotateright();
    virtual void draw( int colorset );
private:
    unsigned char rotation;
};

class LBlock : public Block {
public:
    LBlock();
    virtual void rotateleft();
    virtual void rotateright();
    virtual void draw( int colorset );
private:
    unsigned char rotation;
};

class TBlock : public Block {
public:
    TBlock();
    virtual void rotateleft();
    virtual void rotateright();
    virtual void draw( int colorset );
private:
    unsigned char rotation;
};

class SBlock : public Block {
public:
    SBlock();
    virtual void rotateleft();
    virtual void rotateright() { rotateleft(); }
    virtual void draw( int colorset );
private:
    unsigned char rotation;
};

class ZBlock : public Block {
public:
    ZBlock();
    virtual void rotateleft();
    virtual void rotateright() { rotateleft(); }
    virtual void draw( int colorset );
private:
    unsigned char rotation;
};

class IBlock : public Block {
public:
    IBlock();
    virtual void rotateleft();
    virtual void rotateright() { rotateleft(); }
    virtual void draw( int colorset );
private:
    unsigned char rotation;
};
#endif // BLOCK_H
