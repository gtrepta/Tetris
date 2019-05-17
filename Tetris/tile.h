#ifndef TILE_H
#define TILE_H

#define TILE_SIZE 8

typedef enum {
	COLOR_BLANK     = 0,
	COLOR_ONE       = 1,
	COLOR_TWO       = 2,
	COLOR_BORDER    = 3
} colorid_t;

typedef struct {
	unsigned char color1[3];
	unsigned char color2[3];
} colorset_t;

void drawTile( colorid_t color, int colorset );

#endif //TILE_H