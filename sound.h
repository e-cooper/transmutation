#include <stdlib.h>
#include <stdio.h>
#include "myLib.h"

void playSoundA( const unsigned char* sound, int length, int frequency);
void playSoundB( const unsigned char* sound, int length, int frequency);
void setupInterrupts();
void interruptHandler();
void setupSounds();
void stopSound();
void pauseSound();
void unpauseSound();

typedef struct{
  const unsigned char* data;
  int length;
  int frequency;
  int isPlaying;
  int loops;
  int duration;
  int priority;
} SOUND;

SOUND soundA;
SOUND soundB;
int vbCountA;
int vbCountB;