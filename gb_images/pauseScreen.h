
//{{BLOCK(pauseScreen)

//======================================================================
//
//	pauseScreen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 109 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 6976 + 2048 = 9536
//
//	Time-stamp: 2014-10-20, 01:07:32
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PAUSESCREEN_H
#define GRIT_PAUSESCREEN_H

#define pauseScreenTilesLen 6976
extern const unsigned short pauseScreenTiles[3488];

#define pauseScreenMapLen 2048
extern const unsigned short pauseScreenMap[1024];

#define pauseScreenPalLen 512
extern const unsigned short pauseScreenPal[256];

#endif // GRIT_PAUSESCREEN_H

//}}BLOCK(pauseScreen)
