#include <stdlib.h>
#include <stdio.h>
#include "myLib.h"
#include "text.h"
#include "gb_images/gameScreen.h"
#include "gb_images/winScreen.h"
#include "gb_images/instructionScreen.h"
#include "gb_images/pauseScreen.h"
#include "gb_images/startScreen.h"
#include "gb_images/sprites.h"

void start();
void game();
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
unsigned int buttons;
unsigned int oldButtons;
OBJ_ATTR shadowOAM[128];
#define ROWMASK 0xFF
#define COLMASK 0x1FF

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
    shadowOAM[2].attr2 = SPRITEOFFSET16(0, 0);

    DMANow(3, shadowOAM, OAM, 128);
  }
}

void game() {
  loadPalette(gameScreenPal);
  DMANow(3, gameScreenTiles, &CHARBLOCKBASE[0], gameScreenTilesLen);
  DMANow(3, gameScreenMap, &SCREENBLOCKBASE[27], gameScreenMapLen);

  hideSprites();
  clearShadowOAM();

  while (state == GAMESCREEN) {
    oldButtons = buttons;
    buttons = BUTTONS;

    if (BUTTON_PRESSED(BUTTON_START)) {
      state = PAUSESCREEN;
    }
    if (BUTTON_PRESSED(BUTTON_A)) {
      state = WINSCREEN;
    }

    DMANow(3, shadowOAM, OAM, 128);
  }
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
