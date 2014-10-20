
//{{BLOCK(startScreen)

//======================================================================
//
//	startScreen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 66 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 4224 + 2048 = 6784
//
//	Time-stamp: 2014-10-20, 00:39:24
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_STARTSCREEN_H
#define GRIT_STARTSCREEN_H

#define startScreenTilesLen 4224
extern const unsigned short startScreenTiles[2112];

#define startScreenMapLen 2048
extern const unsigned short startScreenMap[1024];

#define startScreenPalLen 512
extern const unsigned short startScreenPal[256];

#endif // GRIT_STARTSCREEN_H

//}}BLOCK(startScreen)
