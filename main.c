#include <stdlib.h>
#include <stdio.h>
#include "myLib.h"
#include "game.h"

int main() {
	REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;
	REG_BG0CNT = CBB(0) | SBB(27) | BG_SIZE3 | COLOR256;
  REG_BG1CNT = CBB(1) | SBB(25) | BG_SIZE3 | COLOR256;
	buttons = BUTTONS;
  paused = 0;
	state = STARTSCREEN;

	DMANow(3, spritesTiles, &CHARBLOCKBASE[4], spritesTilesLen / 2);
  DMANow(3, spritesPal, SPRITE_PALETTE, spritesPalLen / 2);

  setupInterrupts();
  setupSounds();

	while(1) {
		oldButtons = buttons;
		buttons = BUTTONS;

		switch(state) {
			case STARTSCREEN:
				start();
				break;
			case GAMESCREEN:
        if (paused == 0) {
          initSprites();
        }
				game();
				break;
			case INSTRUCTIONSCREEN:
				instruction();
				break;
			case PAUSESCREEN:
        paused = 1;
				pause();
				break;
			case WINSCREEN:
				win();
				break;
      case LOSESCREEN:
        lose();
        break;
		}
	}
	return 0;
}

void start() {
  initState(STARTSCREEN);

  while (state == STARTSCREEN) {
    cleanSlate();

    if (BUTTON_PRESSED(BUTTON_UP)) {
      if (selector == 1) {
        selector = 0;
        selectRow = 100;
        selectCol = 56;
      }
    }

    if (BUTTON_PRESSED(BUTTON_DOWN)) {
      if (selector == 0) {
        selector = 1;
        selectRow = 132;
        selectCol = 56;
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
    shadowOAM[2].attr2 = SPRITEOFFSET16(2, 0);

    DMANow(3, shadowOAM, OAM, 512);
  }
}

void pause() {
  initState(PAUSESCREEN);

  while (state == PAUSESCREEN) {
    cleanSlate();

    if (BUTTON_PRESSED(BUTTON_START)) {
      state = GAMESCREEN;
    }
    if (BUTTON_PRESSED(BUTTON_A)) {
      paused = 0;
      state = STARTSCREEN;
    }

    DMANow(3, shadowOAM, OAM, 512);
  }
}

void instruction() {
  initState(INSTRUCTIONSCREEN);

  while (state == INSTRUCTIONSCREEN) {
    cleanSlate();

    if (BUTTON_PRESSED(BUTTON_START)) {
      state = STARTSCREEN;
    }

    DMANow(3, shadowOAM, OAM, 512);
  }
}

void win() {
  initState(WINSCREEN);

  while (state == WINSCREEN) {
    cleanSlate();

    if (BUTTON_PRESSED(BUTTON_START)) {
      state = STARTSCREEN;
    }

    DMANow(3, shadowOAM, OAM, 512);
  }
}

void lose() {
  initState(LOSESCREEN);

  while (state == LOSESCREEN) {
    cleanSlate();

    if (BUTTON_PRESSED(BUTTON_START)) {
      state = STARTSCREEN;
    }

    DMANow(3, shadowOAM, OAM, 512);
  }
}

void cleanSlate() {
  oldButtons = buttons;
  buttons = BUTTONS;
  hideSprites();
}
