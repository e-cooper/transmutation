#include "game.h"

void game() {
  loadPalette(mainmapPal);
  DMANow(3, mainmapTiles, &CHARBLOCKBASE[0], mainmapTilesLen);
  DMANow(3, mainmapMap, &SCREENBLOCKBASE[27], mainmapMapLen);
  DMANow(3, cloudsTiles, &CHARBLOCKBASE[1], cloudsTilesLen);
  DMANow(3, cloudsMap, &SCREENBLOCKBASE[25], cloudsMapLen);

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

    moveEnemies();
    movement();
    animate();
    updateOAM();

    DMANow(3, shadowOAM, OAM, 512);
    waitForVblank();

    checkPlayerState();
  }
}
