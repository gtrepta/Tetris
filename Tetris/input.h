#include "includes.h"

#ifndef INPUT_H
#define INPUT_H

#define KEYHELD(x) (x == 1 || x > 16 && (x % 2) == 0)

extern int glob_upKey;
extern int glob_downKey;
extern int glob_leftKey;
extern int glob_rightKey;
extern int glob_enterKey;
extern int glob_escKey;
extern int glob_rotateLeftKey;
extern int glob_rotateRightKey;

void inputTic();
void inputHandle( SDL_Event *evn );

#endif //INPUT_H
