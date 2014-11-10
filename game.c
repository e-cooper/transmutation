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
  player.score = 0;
  player.state = HUMAN;
  player.hasWon = 0;

  hOff = 0;
  vOff = 100;

  int rowVals[] = { 5*8, 5*8, 200, 210, 220, 230, 240, 250, 260, 270};
  int colVals[] = { 8*8, 20*8, 100, 100, 100, 100, 100, 100, 100, 100};
  
  for (int i = 0; i < NUM_BANANAS; i++) {
    bananas[i].isActive = 1;
    bananas[i].width = 16;
    bananas[i].height = 16;
    bananas[i].bigRow = rowVals[i];
    bananas[i].bigCol = colVals[i];
  }

  score.width = 8;
  score.height = 8;
  score.row = 4;
  score.col = 4;
  score.aniState = player.score;
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
    cleanSlate();

    if (BUTTON_PRESSED(BUTTON_START)) {
      state = PAUSESCREEN;
    }
    if (BUTTON_PRESSED(BUTTON_A)) {
      if (player.state == FROG) {
        player.state = HUMAN;
      }
      else {
        player.state = FROG;
      }
    }
    if (BUTTON_PRESSED(BUTTON_B)) {
      if (player.state == CLIMBER) {
        player.state = HUMAN;
      }
      else {
        player.state = CLIMBER;
      }
    }
    if (player.hasWon == 1) {
      state = WINSCREEN;
    }

    movement();
    checkCollect();
    animate();
    updateOAM();

    DMANow(3, shadowOAM, OAM, 512);
    waitForVblank();
  }
}

void movement() {
  player.bigRow = player.row + vOff + 16;
  player.bigCol = player.col + hOff + 1;
  
  for (int i = 0; i < NUM_BANANAS; i++) {
    bananas[i].row = bananas[i].bigRow - vOff;
    bananas[i].col = bananas[i].bigCol - hOff;
  }
  
  if(BUTTON_HELD(BUTTON_UP))
  {
    if (isPassable(CHECKUP)) {
      if (player.row > SCREEN_HEIGHT/2 - player.height/2) {
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
    if (isPassable(CHECKDOWN)) {
      if (player.row < SCREEN_HEIGHT/2 - player.height/2) {
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
    if (isPassable(CHECKLEFT)) {
      if (player.col > SCREEN_WIDTH/2 - player.width/2) {
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
    if (isPassable(CHECKRIGHT)) {
      if (player.col < SCREEN_WIDTH/2 - player.width/2) {
        player.col += player.cdel;
      }
      else if (hOff < COLL_MAP_SIZE - SCREEN_WIDTH) {
        hOff++;
      }
      else if (player.col + player.width < SCREEN_WIDTH) {
        player.col += player.cdel;
      }
    }
  }
  
  REG_BG0HOFS = hOff;
  REG_BG0VOFS = vOff;
}

int isPassable(int direction) {
  int passable = 0, colorA, colorB;

  if (direction == CHECKUP) {
    colorA = defColor(player.bigRow - player.rdel, player.bigCol + 1, 512);
    colorB = defColor(player.bigRow - player.rdel, player.bigCol + player.width - 1, 512);
    passable = checkColor(colorA, colorB);
  }
  else if (direction == CHECKDOWN) {
    colorA = defColor(player.bigRow + player.height + player.rdel, player.bigCol + 1, 512);
    colorB = defColor(player.bigRow + player.height + player.rdel, player.bigCol + player.width - 1, 512);
    passable = checkColor(colorA, colorB);
  }
  else if (direction == CHECKLEFT) {
    colorA = defColor(player.bigRow, player.bigCol, 512);
    colorB = defColor(player.bigRow + player.height, player.bigCol, 512);
    passable = checkColor(colorA, colorB);
  }
  else if (direction == CHECKRIGHT) {
    colorA = defColor(player.bigRow, player.bigCol + player.width, 512);
    colorB = defColor(player.bigRow + player.height, player.bigCol + player.width, 512);
    passable = checkColor(colorA, colorB);
  }
  return passable;
}

int checkColor(int colorA, int colorB) {
  int canPass = 0;

  if ((player.score >= NUM_BANANAS - 2) && 
      colorA == RED && colorB == RED) {
    player.hasWon = 1;
  }
  else if (colorA == WHITE && colorB == WHITE) {
    canPass = 1;
  }
  else if (player.state == FROG && 
          (colorA == GREEN || colorA == WHITE) && 
          (colorB == GREEN || colorB == WHITE)) {
    canPass = 1;
  }
  else if (player.state == CLIMBER && 
          (colorA == BLUE || colorA == WHITE) && 
          (colorB == BLUE || colorB == WHITE)) {
    canPass = 1;
  }
  return canPass;
}

int defColor(int row, int col, int size) {
  return collisionmainmapBitmap[OFFSET(row, col, size)];
}

void checkCollect() {
  // if the banana is still active and the player sprite touches it, mark it as collected
  for (int i = 0; i < NUM_BANANAS; i++) {
    if ((bananas[i].isActive == 1) &&
      (player.bigCol + player.width >= bananas[i].bigCol) && 
      (player.bigCol <= bananas[i].bigCol + bananas[i].width) && 
      (player.bigRow + player.height >= bananas[i].bigRow) && 
      (player.bigRow <= bananas[i].bigRow + bananas[i].height)) {
      collect(i);
    }
  }
}

void collect(int i) {
  playSoundB(CollectionJingle, COLLECTIONJINGLELEN, COLLECTIONJINGLEFREQ);
  bananas[i].isActive = 0;
  player.score++;
  score.aniState++;
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
  // Player sprite
  shadowOAM[0].attr0 = (ROWMASK & player.row) | ATTR0_8BPP | ATTR0_TALL;
  shadowOAM[0].attr1 = (COLMASK & player.col) | ATTR1_SIZE32;
  shadowOAM[0].attr2 = SPRITEOFFSET16((4 * player.state) + (4 * player.currFrame) + 2, 4 * player.prevAniState + 4);
  // Score icon sprite
  shadowOAM[1].attr0 = (ROWMASK & score.row) | ATTR0_8BPP | ATTR0_SQUARE;
  shadowOAM[1].attr1 = (COLMASK & score.col) | ATTR1_SIZE8;
  shadowOAM[1].attr2 = SPRITEOFFSET16(6, 0);
  // First score digit sprite
  shadowOAM[2].attr0 = (ROWMASK & score.row) | ATTR0_8BPP | ATTR0_SQUARE;
  shadowOAM[2].attr1 = (COLMASK & (score.col + score.width)) | ATTR1_SIZE8;
  shadowOAM[2].attr2 = SPRITEOFFSET16(0, 2 * (score.aniState / 10));
  // Second score digit sprite
  shadowOAM[3].attr0 = (ROWMASK & score.row) | ATTR0_8BPP | ATTR0_SQUARE;
  shadowOAM[3].attr1 = (COLMASK & (score.col + score.width * 2)) | ATTR1_SIZE8;
  shadowOAM[3].attr2 = SPRITEOFFSET16(0, 2 * (score.aniState % 10));
  // Banana pickup sprites
  // only draw banana if it is currently in the screen boundaries
  for (int i = 0; i < NUM_BANANAS; i++) {
    if ((bananas[i].isActive == 1) && 
        (bananas[i].bigRow + bananas[i].height + 1 >= vOff) && 
        (bananas[i].bigRow <= vOff + SCREEN_HEIGHT)) {
      shadowOAM[i + 4].attr0 = (ROWMASK & bananas[i].row) | ATTR0_8BPP | ATTR0_SQUARE;
      shadowOAM[i + 4].attr1 = (COLMASK & bananas[i].col) | ATTR1_SIZE16;
      shadowOAM[i + 4].attr2 = SPRITEOFFSET16(4,0);
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
