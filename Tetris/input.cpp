#include "input.h"

unsigned char keyBits = 0;

enum {
    UP_KEY         = 0x1,
    DOWN_KEY       = 0x1 << 1,
    LEFT_KEY       = 0x1 << 2,
    RIGHT_KEY      = 0x1 << 3,
    ROT_LEFT_KEY   = 0x1 << 4,
    ROT_RIGHT_KEY  = 0x1 << 5,
    ESC_KEY        = 0x1 << 6,
    RETURN_KEY     = 0x1 << 7
};

#define SET_KEY( x ) keyBits |= x
#define CLEAR_KEY( x ) keyBits &= 0xff ^ x
#define KEY_VALUE( x ) (int) !!(keyBits & x)

int glob_upKey = 0;
int glob_downKey = 0;
int glob_leftKey = 0;
int glob_rightKey = 0;
int glob_enterKey = 0;
int glob_escKey = 0;
int glob_rotateLeftKey = 0;
int glob_rotateRightKey = 0;

void inputHandle( SDL_Event *evn ) {
	if ( evn->type == SDL_KEYDOWN ) {
		switch ( evn->key.keysym.sym ) {
		case SDLK_RIGHT:
                SET_KEY( RIGHT_KEY );
			break;
		case SDLK_LEFT:
                SET_KEY( LEFT_KEY );
			break;
		case SDLK_UP:
                SET_KEY( UP_KEY );
			break;
		case SDLK_DOWN:
                SET_KEY( DOWN_KEY );
			break;
        case SDLK_z:
                SET_KEY( ROT_LEFT_KEY );
            break;
        case SDLK_x:
                SET_KEY( ROT_RIGHT_KEY );
            break;
        case SDLK_ESCAPE:
                SET_KEY( ESC_KEY );
            break;
        case SDLK_RETURN:
                SET_KEY( RETURN_KEY );
            break;
        default:;
		}
	}

	if ( evn->type == SDL_KEYUP ) {
		switch ( evn->key.keysym.sym ) {
		case SDLK_RIGHT:
            CLEAR_KEY( RIGHT_KEY );
            glob_rightKey = 0;
			break;
		case SDLK_LEFT:
            CLEAR_KEY( LEFT_KEY );
            glob_leftKey = 0;
			break;
		case SDLK_UP:
            CLEAR_KEY( UP_KEY );
            glob_upKey = 0;
			break;
		case SDLK_DOWN:
            CLEAR_KEY( DOWN_KEY );
            glob_downKey = 0;
			break;
		case SDLK_z:
            CLEAR_KEY( ROT_LEFT_KEY );
            glob_rotateLeftKey = 0;
			break;
		case SDLK_x:
            CLEAR_KEY( ROT_RIGHT_KEY );
            glob_rotateRightKey = 0;
			break;
		case SDLK_ESCAPE:
            CLEAR_KEY( ESC_KEY );
            glob_escKey = 0;
			break;
		case SDLK_RETURN:
            CLEAR_KEY( RETURN_KEY );
            glob_enterKey = 0;
			break;
        default:;
		}
	}
}

void inputTic() {
    glob_upKey          += KEY_VALUE( UP_KEY );
    glob_downKey        += KEY_VALUE( DOWN_KEY );
    glob_leftKey        += KEY_VALUE( LEFT_KEY );
    glob_rightKey       += KEY_VALUE( RIGHT_KEY );
    glob_escKey         += KEY_VALUE( ESC_KEY );
    glob_enterKey       += KEY_VALUE( RETURN_KEY );
    glob_rotateLeftKey  += KEY_VALUE( ROT_LEFT_KEY );
    glob_rotateRightKey += KEY_VALUE( ROT_RIGHT_KEY );
}
