#include <stdlib.h>
#include <stdio.h>
#include "myLib.h"
#include "sound.h"
#include "gb_images/collisionmainmap.h"
#include "gb_sounds/CollectionJingle.h"
#include "gb_sounds/HurtSound.h"

unsigned int buttons;
unsigned int oldButtons;
OBJ_ATTR shadowOAM[128];
#define ROWMASK 0xFF
#define COLMASK 0x1FF
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define COLL_MAP_SIZE 512
#define NUM_BANANAS 10
#define NUM_ENEMIES 4

typedef struct {
  int row;
  int col;
  int bigRow;
  int bigCol;
  int rdel;
  int cdel;
  int width;
  int height;
  int waitTime;
  int aniCounter;
  int aniState;
  int prevAniState;
  int currFrame;
  int isActive;
  int score;
  int state;
  int hasWon;
  int health;
  int damage;
  int vulnerable;
  int vulnCounter;
} SPRITE;

SPRITE bananas[NUM_BANANAS], player, score, health, enemies[NUM_ENEMIES];

enum { PLAYERFRONT, PLAYERLEFT, PLAYERBACK, PLAYERRIGHT, PLAYERIDLE };
enum { HUMAN, FROG = 2, CLIMBER = 4, GHOST = 6 };
enum { CHECKUP, CHECKDOWN, CHECKLEFT, CHECKRIGHT };

void checkPlayerState();
void hideSprites();
void clearShadowOAM();
void initSprites();
void animate();
void movement();
void updateOAM();
int checkCollision(SPRITE sprite);
void collect(int i);
int defColor(int row, int col, int size);
int isPassable(int direction);
int checkColor(int colorA, int colorB);
void moveEnemies();
void checkDamagePlayer();
void damagePlayer();