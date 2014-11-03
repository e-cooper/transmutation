#include "game.h"

void initGame() {
  player.width = 12;
  player.height = 8;
  player.rdel = 1;
  player.cdel = 1;
  player.row = 160/2-player.height/2;
  player.col = 240/2-player.width/2 + 2;
  player.aniCounter = 0;
  player.currFrame = 0;
  player.aniState = PLAYERFRONT;

  hOff = 0;
  vOff = 100;
  
  for (int i = 0; i < NUM_BANANAS; i++) {
    bananas[i].isActive = 1;
    bananas[i].width = 16;
    bananas[i].height = 16;
    bananas[i].bigRow = 200 + i * 40;
    bananas[i].bigCol = i * 40;
  }
}

void game() {
  loadPalette(mainmapPal);
  DMANow(3, mainmapTiles, &CHARBLOCKBASE[0], mainmapTilesLen);
  DMANow(3, mainmapMap, &SCREENBLOCKBASE[27], mainmapMapLen);

  hideSprites();
  clearShadowOAM();

  DMANow(3, spritesTiles, &CHARBLOCKBASE[4], spritesTilesLen/2);
  DMANow(3, spritesPal, SPRITE_PALETTE, 256);

  if (paused == 0) {
    playSoundA(MainSoundtrack, MAINSOUNDTRACKLEN, MAINSOUNDTRACKFREQ);
  }
  else {
    paused = 0;
    unpauseSound();
  }

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
  player.bigRow = player.row + vOff + 16;
  player.bigCol = player.col + hOff;
  
  for (int i = 0; i < NUM_BANANAS; i++) {
    bananas[i].row = bananas[i].bigRow - vOff;
    bananas[i].col = bananas[i].bigCol - hOff;
  }

  checkCollect();
  
  if(BUTTON_HELD(BUTTON_UP))
  {
    if (collisionmainmapBitmap[OFFSET(player.bigRow - player.rdel, player.bigCol + 1, 512)] == WHITE &&
        collisionmainmapBitmap[OFFSET(player.bigRow - player.rdel, player.bigCol + player.width - 1, 512)] == WHITE) {
      if (player.row > 30) {
        player.row -= player.rdel;
      }
      else if (vOff > 0) {
        vOff--;
      }
      else {
        player.row -= player.rdel;
      }
    }
  }
  if(BUTTON_HELD(BUTTON_DOWN))
  {
    if (collisionmainmapBitmap[OFFSET(player.bigRow + player.height + player.rdel, player.bigCol + 1, 512)] == WHITE &&
        collisionmainmapBitmap[OFFSET(player.bigRow + player.height + player.rdel, player.bigCol + player.width - 1, 512)] == WHITE) {
      if (player.row - player.height < SCREEN_HEIGHT - 60) {
        player.row += player.rdel;
      }
      else if (vOff < COLL_MAP_SIZE - SCREEN_HEIGHT) {
        vOff++;
      }
      else {
        player.row += player.rdel;
      }
    }
  }
  if(BUTTON_HELD(BUTTON_LEFT))
  {
    if (collisionmainmapBitmap[OFFSET(player.bigRow, player.bigCol, 512)] == WHITE &&
        collisionmainmapBitmap[OFFSET(player.bigRow + player.height, player.bigCol, 512)] == WHITE) {
      if (player.col > 30) {
        player.col -= player.cdel;
      }
      else if (hOff > 0) {
        hOff--;
      }
      else if (player.col > 0) {
        player.col -= player.cdel;
      }
    }
  }
  if(BUTTON_HELD(BUTTON_RIGHT))
  {
    if (collisionmainmapBitmap[OFFSET(player.bigRow, player.bigCol + player.width, 512)] == WHITE &&
        collisionmainmapBitmap[OFFSET(player.bigRow + player.height, player.bigCol + player.width, 512)] == WHITE) {
      if (player.col + player.width < 240 - 30) {
        player.col += player.cdel;
      }
      else if (hOff < COLL_MAP_SIZE - SCREEN_WIDTH) {
        hOff++;
      }
      else if (player.col + player.width < 240) {
        player.col += player.cdel;
      }
    }
  }
  
  REG_BG0HOFS = hOff;
  REG_BG0VOFS = vOff;
}

void checkCollect() {
  for (int i = 0; i < NUM_BANANAS; i++) {
    if ((bananas[i].isActive == 1) &&
      (player.bigCol + player.width >= bananas[i].bigCol) && 
      (player.bigCol <= bananas[i].bigCol + bananas[i].width) && 
      (player.bigRow + player.height >= bananas[i].bigRow) && 
      (player.bigRow <= bananas[i].bigRow + bananas[i].height)) {
      collect(i);
    }
  }
  // else if ((player.bigCol + player.width <= bananas.col + bananas.width) && 
  //   (player.bigCol + player.width >= bananas.col) && 
  //   (player.bigRow + player.height <= bananas.row + bananas.height) && 
  //   (player.bigRow + player.height >= bananas.row)) {
  //   collect();
  // }
  // else if (player.bigCol + player.width == bananas.col) {
  //   collect(bananas);
  // }
  // else if (player.bigRow == bananas.row + bananas.height) {
  //   collect(bananas);
  // }
  // else if (player.bigRow + player.height == bananas.col) {
  //   collect(bananas);
  // }
  // (player.row + player.height >= enemy[i].row &&
  //               player.row <= enemy[i].row + enemy[i].height &&
  //               player.col + player.width >= enemy[i].col &&
  //               player.col <= enemy[i].col + enemy[i].width)
}

void collect(int i) {
  playSoundB(CollectionJingle, COLLECTIONJINGLELEN, COLLECTIONJINGLEFREQ);
  bananas[i].isActive = 0;
}

void animate() {
  if (player.aniState != PLAYERIDLE) {
    player.prevAniState = player.aniState;
  }
    
  player.aniState = PLAYERIDLE;
    
  if(player.aniCounter % 20 == 0) {
    player.aniCounter = 0;
    if (player.currFrame >= 1) {
      player.currFrame = 0;
    }
    else {
      player.currFrame++;
    }
  }

  if(BUTTON_HELD(BUTTON_UP)) {
    player.aniState = PLAYERBACK;
  }
  if(BUTTON_HELD(BUTTON_DOWN)) {
    player.aniState = PLAYERFRONT;
  }
  if(BUTTON_HELD(BUTTON_LEFT)) {
    player.aniState = PLAYERLEFT;
  }
  if(BUTTON_HELD(BUTTON_RIGHT)) {
    player.aniState = PLAYERRIGHT;
  }
  if(player.aniState == PLAYERIDLE) {
    player.currFrame = 0;
  }
  else {
    player.aniCounter++;
  }
}

void updateOAM() {
  shadowOAM[0].attr0 = (ROWMASK & player.row) | ATTR0_8BPP | ATTR0_TALL;
  shadowOAM[0].attr1 = (COLMASK & player.col) | ATTR1_SIZE32;
  shadowOAM[0].attr2 = SPRITEOFFSET16(4*player.currFrame + 2,4*player.prevAniState + 4);
  
  // only draw banana if it is currently in the screen boundaries
  for (int i = 0; i < NUM_BANANAS; i++) {
    if ((bananas[i].isActive == 1) && 
        (bananas[i].bigRow + bananas[i].height >= vOff) && 
        (bananas[i].bigRow <= vOff + SCREEN_HEIGHT)) {
      shadowOAM[i + 1].attr0 = (ROWMASK & bananas[i].row) | ATTR0_8BPP | ATTR0_SQUARE;
      shadowOAM[i + 1].attr1 = (COLMASK & bananas[i].col) | ATTR1_SIZE16;
      shadowOAM[i + 1].attr2 = SPRITEOFFSET16(10,4);
    }
    else if (bananas[i].isActive == 0) {
      shadowOAM[i + 1].attr0 = ATTR0_HIDE;
    }
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
