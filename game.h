#include <stdlib.h>
#include <stdio.h>
#include "myLib.h"
#include "sound.h"
#include "gb_images/mainmap.h"
#include "gb_images/collisionmainmap.h"
#include "gb_images/sprites.h"
#include "gb_sounds/CollectionJingle.h"
#include "gb_sounds/MainSoundtrack.h"

void hideSprites();
void clearShadowOAM();
void game();
void initGame();
void animate();
void movement();
void updateOAM();
void checkCollect();
void collect(int i);

unsigned int buttons;
unsigned int oldButtons;
OBJ_ATTR shadowOAM[128];
#define ROWMASK 0xFF
#define COLMASK 0x1FF
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define COLL_MAP_SIZE 512
#define NUM_BANANAS 10

typedef struct  
{
  int row;
  int col;
  int bigRow;
  int bigCol;
  int rdel;
  int cdel;
  int width;
  int height;
  int aniCounter;
  int aniState;
  int prevAniState;
  int currFrame;
  int isActive;
} SPRITE;

SPRITE bananas[NUM_BANANAS];
SPRITE player;

enum { PLAYERFRONT, PLAYERLEFT, PLAYERBACK, PLAYERRIGHT, PLAYERIDLE };
