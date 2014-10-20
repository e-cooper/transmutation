
//{{BLOCK(winScreen)

//======================================================================
//
//	winScreen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 59 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 3776 + 2048 = 6336
//
//	Time-stamp: 2014-10-20, 01:07:50
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_WINSCREEN_H
#define GRIT_WINSCREEN_H

#define winScreenTilesLen 3776
extern const unsigned short winScreenTiles[1888];

#define winScreenMapLen 2048
extern const unsigned short winScreenMap[1024];

#define winScreenPalLen 512
extern const unsigned short winScreenPal[256];

#endif // GRIT_WINSCREEN_H

//}}BLOCK(winScreen)
