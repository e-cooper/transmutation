#include <stdlib.h>
#include <stdio.h>
#include "myLib.h"
#include "text.h"
#include "gb_images/house.h"
#include "gb_images/collisionmap.h"
#include "gb_images/winScreen.h"
#include "gb_images/instructionScreen.h"
#include "gb_images/pauseScreen.h"
#include "gb_images/startScreen.h"
#include "gb_images/sprites.h"

void start();
void game();
void initGame();
void animate();
void movement();
void updateOAM();
void pause();
void instruction();
void pause();
void win();
void hideSprites();
void clearShadowOAM();

typedef enum { STARTSCREEN, GAMESCREEN, INSTRUCTIONSCREEN, PAUSESCREEN, WINSCREEN } screen;

screen state;
int selector;
int selectRow;
int selectCol;
int hOff = 0;
int vOff = 0;
unsigned int buttons;
unsigned int oldButtons;
OBJ_ATTR shadowOAM[128];
#define ROWMASK 0xFF
#define COLMASK 0x1FF

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
} SPRITE;

SPRITE mouse2;
SPRITE mouse;

// enum { MONKEYFRONT, MONKEYBACK, MONKEYLEFT, MONKEYRIGHT, MONKEYIDLE }
enum { MOUSEFRONT, MOUSELEFT, MOUSEBACK, MOUSERIGHT, MOUSEIDLE };

int main()
{
	REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;
	REG_BG0CNT = CBB(0) | SBB(27) | BG_SIZE3 | COLOR256;
	buttons = BUTTONS;
	state = STARTSCREEN;

	DMANow(3, spritesTiles, &CHARBLOCKBASE[4], spritesTilesLen / 2);
  DMANow(3, spritesPal, SPRITE_PALETTE, spritesPalLen / 2);

	while(1)
	{
		oldButtons = buttons;
		buttons = BUTTONS;

		switch(state)
		{
			case STARTSCREEN:
				start();
				break;
			case GAMESCREEN:
        initGame();
				game();
				break;
			case INSTRUCTIONSCREEN:
				instruction();
				break;
			case PAUSESCREEN:
				pause();
				break;
			case WINSCREEN:
				win();
				break;
		}
	}
	return 0;
}

void start() {

	loadPalette(startScreenPal);
  DMANow(3, startScreenTiles, &CHARBLOCKBASE[0], startScreenTilesLen);
  DMANow(3, startScreenMap, &SCREENBLOCKBASE[27], startScreenMapLen);

  hideSprites();
  clearShadowOAM();

  selector = 0;
  selectRow = 92;
  selectCol = 70;

  while (state == STARTSCREEN) {
    oldButtons = buttons;
    buttons = BUTTONS;

    if (BUTTON_PRESSED(BUTTON_UP)) {
      if (selector == 1) {
        selector = 0;
        selectRow = 92;
        selectCol = 70;
      }
    }

    if (BUTTON_PRESSED(BUTTON_DOWN)) {
      if (selector == 0) {
        selector = 1;
        selectRow = 124;
        selectCol = 70;
      }
    }

    if (BUTTON_PRESSED(BUTTON_START)) {
      if (selector == 0) {
        state = GAMESCREEN;
      }
      if (selector == 1) {
        state = INSTRUCTIONSCREEN;
      }
    }

    shadowOAM[2].attr0 = (ROWMASK & selectRow) | ATTR0_8BPP | ATTR0_SQUARE;
    shadowOAM[2].attr1 = (COLMASK & selectCol) | ATTR1_SIZE8;
    shadowOAM[2].attr2 = SPRITEOFFSET16(1, 0);

    DMANow(3, shadowOAM, OAM, 128);
  }
}

void initGame() {
  mouse.width = 16;
  mouse.height = 16;
  mouse.rdel = 1;
  mouse.cdel = 1;
  mouse.row = 160/2-mouse.width/2;
  mouse.col = 240/2-mouse.height/2;
  mouse.aniCounter = 0;
  mouse.currFrame = 0;
  mouse.aniState = MOUSEFRONT;

  hOff = 9;
  vOff = 96;
  
  mouse2.width = 16;
  mouse2.height = 16;
  //Spoilers
  mouse2.bigRow = 10;
  mouse2.bigCol = 10;
}

void game() {
  loadPalette(housePal);
  DMANow(3, houseTiles, &CHARBLOCKBASE[0], houseTilesLen);
  DMANow(3, houseMap, &SCREENBLOCKBASE[27], houseMapLen);

  hideSprites();
  clearShadowOAM();

  DMANow(3, spritesTiles, &CHARBLOCKBASE[4], spritesTilesLen/2);
  DMANow(3, spritesPal, SPRITE_PALETTE, 256);

  while (state == GAMESCREEN) {
    oldButtons = buttons;
    buttons = BUTTONS;

    if (BUTTON_PRESSED(BUTTON_START)) {
      state = PAUSESCREEN;
    }
    if (BUTTON_PRESSED(BUTTON_A)) {
      state = WINSCREEN;
    }

    movement();
    animate();
    updateOAM();

    DMANow(3, shadowOAM, OAM, 512);
    waitForVblank();
  }
}

void movement() {
  //Updates pikachu's world position based on its row/col movement
  mouse.bigRow = mouse.row + vOff;
  mouse.bigCol = mouse.col + hOff;
  
  //Updates the stationary pikachu's row/col screen position based on its world position
  mouse2.row = mouse2.bigRow - vOff;
  mouse2.col = mouse2.bigCol - hOff;
  
  //Handle movement and collision logic using pikachu.bigRow and pikachu.bigCol
  if(BUTTON_HELD(BUTTON_UP))
  {
    if (collisionmapBitmap[OFFSET(mouse.bigRow - mouse.rdel, mouse.bigCol, 256)] == WHITE &&
        collisionmapBitmap[OFFSET(mouse.bigRow - mouse.rdel, mouse.bigCol + mouse.width, 256)] == WHITE)
      {
      if (mouse.row > 0) {
        mouse.row -= mouse.rdel;
      }
      else if (vOff > 0) {
        vOff--;
      }
    }
  }
  if(BUTTON_HELD(BUTTON_DOWN))
  {
    if (collisionmapBitmap[OFFSET(mouse.bigRow + mouse.height + mouse.rdel, mouse.bigCol, 256)] == WHITE &&
        collisionmapBitmap[OFFSET(mouse.bigRow + mouse.height + mouse.rdel, mouse.bigCol + mouse.width, 256)] == WHITE)
      {
      if (mouse.row + mouse.height < 160) {
        mouse.row += mouse.rdel;
      }
      else if (vOff + mouse.row < 160) {
        vOff++;
      }
    }
  }
  if(BUTTON_HELD(BUTTON_LEFT))
  {
    if (collisionmapBitmap[OFFSET(mouse.bigRow, mouse.bigCol, 256)] == WHITE &&
        collisionmapBitmap[OFFSET(mouse.bigRow + mouse.height, mouse.bigCol, 256)] == WHITE)
      {
      if (mouse.col > 0) {
        mouse.col -= mouse.cdel;
      }
      else if (hOff > 0) {
        hOff--;
      }
    }
  }
  if(BUTTON_HELD(BUTTON_RIGHT))
  {
    if (collisionmapBitmap[OFFSET(mouse.bigRow, mouse.bigCol + mouse.width, 256)] == WHITE &&
        collisionmapBitmap[OFFSET(mouse.bigRow + mouse.height, mouse.bigCol + mouse.width, 256)] == WHITE)
      {
      if (mouse.col + mouse.width < 240) {
        mouse.col += mouse.cdel;
      }
      else if (hOff + mouse.col < 240) {
        hOff++;
      }
    }
  }
  
  REG_BG0HOFS = hOff;
  REG_BG0VOFS = vOff;
}

void animate()
{
        if (mouse.aniState != MOUSEIDLE)
    {
      mouse.prevAniState = mouse.aniState;
    }
    
        mouse.aniState = MOUSEIDLE;
    
      //   if(mouse.aniCounter%20==0) 
      //   {
      // mouse.aniCounter = 0;
      // if (mouse.currFrame == 2)
      // {
      //   mouse.currFrame = 0;
      // }
      // else
      // {
      //   mouse.currFrame++;
      // }
      //   }

        if(BUTTON_HELD(BUTTON_UP))
    {
      mouse.aniState = MOUSEBACK;
    }
        if(BUTTON_HELD(BUTTON_DOWN))
    {
      mouse.aniState = MOUSEFRONT;
    }
        if(BUTTON_HELD(BUTTON_LEFT))
    {
      mouse.aniState = MOUSELEFT;
    }
        if(BUTTON_HELD(BUTTON_RIGHT))
    {
      mouse.aniState = MOUSERIGHT;
    }
    
        if(mouse.aniState == MOUSEIDLE)
    {
            mouse.currFrame = 0;
    }
        else
    {
            mouse.aniCounter++;
    }
}

void updateOAM()
{
  shadowOAM[0].attr0 = (ROWMASK & mouse.row) | ATTR0_8BPP | ATTR0_SQUARE;
  shadowOAM[0].attr1 = (COLMASK & mouse.col) | ATTR1_SIZE16;
  shadowOAM[0].attr2 = SPRITEOFFSET16(2*mouse.currFrame + 1,5*mouse.prevAniState + 2);
  
  //A stationary pikachu sprite
  shadowOAM[1].attr0 = (ROWMASK & mouse2.row) | ATTR0_8BPP | ATTR0_SQUARE;
  shadowOAM[1].attr1 = (COLMASK & mouse2.col) | ATTR1_SIZE16;
  shadowOAM[1].attr2 = SPRITEOFFSET16(1,2);
}

void pause() {
  loadPalette(pauseScreenPal);
  DMANow(3, pauseScreenTiles, &CHARBLOCKBASE[0], pauseScreenTilesLen);
  DMANow(3, pauseScreenMap, &SCREENBLOCKBASE[27], pauseScreenMapLen);

  hideSprites();
  clearShadowOAM();

  while (state == PAUSESCREEN) {
    oldButtons = buttons;
    buttons = BUTTONS;

    if (BUTTON_PRESSED(BUTTON_START)) {
      state = GAMESCREEN;
    }
    if (BUTTON_PRESSED(BUTTON_A)) {
      // state = PAUSESCREEN;
    }

    DMANow(3, shadowOAM, OAM, 128);
  }
}

void instruction() {
  loadPalette(instructionScreenPal);
  DMANow(3, instructionScreenTiles, &CHARBLOCKBASE[0], instructionScreenTilesLen);
  DMANow(3, instructionScreenMap, &SCREENBLOCKBASE[27], instructionScreenMapLen);

  hideSprites();
  clearShadowOAM();

  while (state == INSTRUCTIONSCREEN) {
    oldButtons = buttons;
    buttons = BUTTONS;

    if (BUTTON_PRESSED(BUTTON_START)) {
      state = STARTSCREEN;
    }

    DMANow(3, shadowOAM, OAM, 128);
  }
}

void win() {
  loadPalette(winScreenPal);
  DMANow(3, winScreenTiles, &CHARBLOCKBASE[0], winScreenTilesLen);
  DMANow(3, winScreenMap, &SCREENBLOCKBASE[27], winScreenMapLen);

  hideSprites();
  clearShadowOAM();

  while (state == WINSCREEN) {
    oldButtons = buttons;
    buttons = BUTTONS;

    if (BUTTON_PRESSED(BUTTON_START)) {
      state = STARTSCREEN;
    }

    DMANow(3, shadowOAM, OAM, 128);
  }
}

void hideSprites() {
  for(int i = 0; i < 128; i++) {
    shadowOAM[i].attr0 = ATTR0_HIDE;
  }
}

void clearShadowOAM() {
  for (int i = 0; i < 128; i++) {
    shadowOAM[i].attr0 = 0;
    shadowOAM[i].attr1 = 0;
    shadowOAM[i].attr2 = 0;
  }
}
