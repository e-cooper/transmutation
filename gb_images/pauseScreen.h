
//{{BLOCK(pauseScreen)

//======================================================================
//
//	pauseScreen, 256x256@8, 
//	+ 96 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 6144 + 2048 = 8192
//
//	Time-stamp: 2014-11-10, 03:24:16
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PAUSESCREEN_H
#define GRIT_PAUSESCREEN_H

#define pauseScreenTilesLen 6144
extern const unsigned short pauseScreenTiles[3072];

#define pauseScreenMapLen 2048
extern const unsigned short pauseScreenMap[1024];

#endif // GRIT_PAUSESCREEN_H

//}}BLOCK(pauseScreen)
