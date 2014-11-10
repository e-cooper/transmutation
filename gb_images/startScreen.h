
//{{BLOCK(startScreen)

//======================================================================
//
//	startScreen, 256x256@8, 
//	+ palette 16 entries, not compressed
//	+ 72 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 32 + 4608 + 2048 = 6688
//
//	Time-stamp: 2014-11-10, 03:32:50
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_STARTSCREEN_H
#define GRIT_STARTSCREEN_H

#define startScreenTilesLen 4608
extern const unsigned short startScreenTiles[2304];

#define startScreenMapLen 2048
extern const unsigned short startScreenMap[1024];

#define startScreenPalLen 32
extern const unsigned short startScreenPal[16];

#endif // GRIT_STARTSCREEN_H

//}}BLOCK(startScreen)
