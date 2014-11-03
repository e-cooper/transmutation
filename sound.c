#include "sound.h"

void setupSounds() {
  REG_SOUNDCNT_X = SND_ENABLED;

  REG_SOUNDCNT_H = SND_OUTPUT_RATIO_100 | 
                    DSA_OUTPUT_RATIO_100 | 
                    DSA_OUTPUT_TO_BOTH | 
                    DSA_TIMER0 | 
                    DSA_FIFO_RESET |
                    DSB_OUTPUT_RATIO_100 | 
                    DSB_OUTPUT_TO_BOTH | 
                    DSB_TIMER1 | 
                    DSB_FIFO_RESET;

  REG_SOUNDCNT_L = 0;
}

void playSoundA( const unsigned char* sound, int length, int frequency) {
  dma[1].cnt = 0;
  vbCountA = 0;

  int interval = 16777216/frequency;

  DMANow(1, sound, REG_FIFO_A, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);

  REG_TM0CNT = 0;

  REG_TM0D = -interval;
  REG_TM0CNT = TIMER_ON;

  soundA.data = sound;
  soundA.length = length;
  soundA.frequency = frequency;
  soundA.isPlaying = 1;
  soundA.loops = 1;
  soundA.duration = ((60*length)/frequency) - ((length/frequency)*3)-1;
  soundA.priority = 0;    
}


void playSoundB( const unsigned char* sound, int length, int frequency) {
  dma[2].cnt = 0;
  vbCountB = 0;

  int interval = 16777216/frequency;

  DMANow(2, sound, REG_FIFO_B, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);

  REG_TM1CNT = 0;

  REG_TM1D = -interval;
  REG_TM1CNT = TIMER_ON;

  soundB.data = sound;
  soundB.length = length;
  soundB.frequency = frequency;
  soundB.isPlaying = 1;
  soundB.loops = 0;
  soundB.duration = ((60*length)/frequency) - ((length/frequency)*3)-1;
  soundB.priority = 1;
}

void stopSound() {
  dma[1].cnt = 0;
  dma[2].cnt = 0;
  soundA.isPlaying = 0;
  soundB.isPlaying = 0;
}

void setupInterrupts() {
  REG_IME = 0;
  REG_INTERRUPT = (unsigned int)interruptHandler;
  REG_IE |= INT_VBLANK;
  REG_DISPSTAT |= INT_VBLANK_ENABLE;
  REG_IME = 1;
}

void interruptHandler() {
  REG_IME = 0;
  if (REG_IF & INT_VBLANK) {
    if (soundA.isPlaying) {
        vbCountA++;
    }
    if (vbCountA >= soundA.duration) {
      REG_TM0CNT = 0;
      dma[1].cnt = 0;
      if (soundA.loops && soundA.isPlaying) {
        playSoundA(soundA.data, soundA.length, soundA.frequency);
      }
    }
    if (soundB.isPlaying) {
        vbCountB++;
    }
    if (vbCountB >= soundB.duration) {
      REG_TM1CNT = 0;
      dma[2].cnt = 0;
      if (soundB.loops && soundA.isPlaying) {
        playSoundB(soundB.data, soundB.length, soundB.frequency);
      }
    }

    REG_IF = INT_VBLANK; 
  }

  REG_IME = 1;
}