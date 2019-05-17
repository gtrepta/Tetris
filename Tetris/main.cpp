#define SDL_MAIN_HANDLED
#include <stdio.h>
#include "includes.h"
#include "input.h"
#include "tetris.h"
#include "select.h"
#include "font.h"

bool waiting = true;

typedef struct _screenStack {
    void( *ticFunc  )( );
    void( *drawFunc )( );
    _screenStack *nextScreen;
} screenStack_t;

screenStack_t *screenStack;

enum {
    MAIN_MENU       = 0,
    GAME_SCREEN     = 1,
    GAMEOVER_SCREEN = 2,
    PAUSE_SCREEN    = 3,
    ESC_SCREEN      = 4
} appMode;

extern int gameOverBit = 0;

void checkGameOver() {
    if ( gameOverBit && appMode == GAME_SCREEN ) {
        screenStack_t *newScreen = new screenStack_t;
        newScreen->ticFunc = NULL;
        newScreen->drawFunc = tetrisGameOverDraw;
        newScreen->nextScreen = screenStack;
        screenStack = newScreen;
        appMode = GAMEOVER_SCREEN;
    }
}

/*
Uint32 timerFunc( Uint32 interval, void *param )

Drives the main loop, runs at 60hz
*/
Uint32 timerFunc( Uint32 interval, void *param ) {
    waiting = false;
    return interval;
}

/*
void mainTic()

Reads the player's input, pushes/pops the screen stack for menu navigation
*/
void mainTic() {
    switch ( appMode ) {
    case MAIN_MENU:
        if ( KEYHELD( glob_enterKey ) ) {
            screenStack->ticFunc = tetrisTic;
            screenStack->drawFunc = tetrisDraw;
            appMode = GAME_SCREEN;
        }
        break;
    case GAME_SCREEN:
        if ( KEYHELD( glob_enterKey ) ) {
            screenStack_t *newScreen = new screenStack_t;
            newScreen->ticFunc = NULL;
            newScreen->drawFunc = tetrisPauseDraw;
            newScreen->nextScreen = screenStack;
            screenStack = newScreen;
            appMode = PAUSE_SCREEN;
        }
        if ( KEYHELD( glob_escKey ) ) {
            screenStack_t *newScreen = new screenStack_t;
            newScreen->ticFunc = selectTic;
            newScreen->drawFunc = selectDraw;
            newScreen->nextScreen = screenStack;
            screenStack = newScreen;
            appMode = ESC_SCREEN;
        }
        break;
    case PAUSE_SCREEN:
        if ( KEYHELD( glob_enterKey ) ) {
            screenStack_t *oldScreen = screenStack;
            screenStack = screenStack->nextScreen;
            delete oldScreen;
            appMode = GAME_SCREEN;
        }
        if ( KEYHELD( glob_escKey ) ) {
            screenStack->ticFunc = selectTic;
            screenStack->drawFunc = selectDraw;
            appMode = ESC_SCREEN;
        }
        break;
    case GAMEOVER_SCREEN:
        if ( KEYHELD( glob_escKey ) || KEYHELD( glob_enterKey ) ) {
            screenStack->ticFunc = selectTic;
            screenStack->drawFunc = selectDraw;
            appMode = ESC_SCREEN;
        }
        break;
    case ESC_SCREEN:
        if ( KEYHELD( glob_escKey ) && (gameOverBit == 0) ) {
            screenStack_t *oldScreen = screenStack;
            screenStack = screenStack->nextScreen;
            delete oldScreen;
            appMode = GAME_SCREEN;
        }
        if ( KEYHELD( glob_enterKey ) ) {
            screenStack_t *oldScreen = screenStack;
            screenStack = screenStack->nextScreen;
            delete oldScreen;
            gameOverBit = 0;
            appMode = GAME_SCREEN;
        }
        break;
    default:;
    }
}

/*
void drawStack( screenStack_t *stack )

(recursive)

Draws the whole screen stack from bottom up, with 'transparent plates' between
each of them 
*/
void drawStack( screenStack_t *stack ) {
    if ( stack->nextScreen ) {
        drawStack( stack->nextScreen );
    } else {
        stack->drawFunc();
        return;
    }
    glColor4f( 0.0f, 0.0f, 0.0f, 0.5f );
    glRectd( 0.0, 0.0, 320, 240 );
    stack->drawFunc();
}

/*
void doWindow( SDL_WindowEvent *evn )

Handles any resizing of the window, adjusts the viewport accordingly
*/
void doWindow( SDL_WindowEvent *evn ) {
    if ( evn->event == SDL_WINDOWEVENT_RESIZED ) {
        glViewport( 0, 0, evn->data1, evn->data2 );
        double ratio = ( double )evn->data1 / ( double )evn->data2;
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        if ( ratio >= ( 4.0 / 3.0 ) ) {
            double width = 240.0 * ratio;
            double offset = ( width - 320.0 ) / 2;
            gluOrtho2D( -offset, 320 + offset, 240, 0 );
        } else {
            double height = 320.0 / ratio;
            double offset = ( height - 240.0 ) / 2;
            gluOrtho2D( 0, 320, 240 + offset, -offset );
        }
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
    }
}

SDL_Window *win;
SDL_GLContext glcon;

/*
void init()

Initializes SDL, a window, the viewport, the font subsystem, starts the clock,
and sets up the initial screen
*/
void init() {
    printf( "Initializing SDL...\n" );
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0 ) {
        printf( "SDL_Init Error: %s\n", SDL_GetError() );
        exit(1);
    }

    printf( "Initializing window...\n" );
    win = SDL_CreateWindow( "Tetris", 100, 100, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
    if ( win == NULL ) {
        printf( "SDL_CreateWindow Error: %s\n", SDL_GetError() );
        exit(1);
    }

    printf( "Initializing GL Context...\n" );
    glcon = SDL_GL_CreateContext( win );
    if ( glcon == NULL ) {
        printf( "SDL_GL_CreateContext Error: %s\n", SDL_GetError() );
        exit(1);
    }

    glViewport( 0, 0, 640, 480 );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, 320, 240, 0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glShadeModel( GL_FLAT );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glClearColor( 0.0f, 0.0f, 0.0f, 1 );

    fontInit();

    SDL_AddTimer( 1000 / 60, timerFunc, NULL );

    appMode = MAIN_MENU;
    screenStack = new screenStack_t;
    screenStack->ticFunc = selectTic;
    screenStack->drawFunc = selectDraw;
    screenStack->nextScreen = NULL;
}

/*
void end()

Deletes the screen stack and shuts down SDL
*/
void end() {
    screenStack_t *deadScreen = screenStack;
    while ( screenStack ) {
        deadScreen = screenStack->nextScreen;
        delete screenStack;
        screenStack = deadScreen;
    }

    tetrisEnd();

    SDL_GL_DeleteContext( glcon );
    SDL_DestroyWindow( win );
    SDL_Quit();
}

/*
main

Runs the game loop

Only the top screen on the screen stack will tic, all screens below it will be
suspended in animation
*/
int main( int argc, char *argv[] ) {
    SDL_Event evn;

    init();

loop:
    waiting = true;
    inputTic();

    if (screenStack->ticFunc)
        screenStack->ticFunc();

    checkGameOver();
    mainTic();

    glClear( GL_COLOR_BUFFER_BIT );
    drawStack( screenStack );
    SDL_GL_SwapWindow( win );

    do {
        if ( SDL_PollEvent( &evn ) )
            switch ( evn.type ) {
            case SDL_QUIT:
                goto quit;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                inputHandle( &evn );
                break;
            case SDL_WINDOWEVENT:
                doWindow( ( SDL_WindowEvent * )&evn );
                break;
            default:;
            }
    } while ( waiting );
    goto loop;

quit:
    end();
	return 0;
}
