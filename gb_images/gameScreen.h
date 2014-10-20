
//{{BLOCK(gameScreen)

//======================================================================
//
//	gameScreen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 74 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 4736 + 2048 = 7296
//
//	Time-stamp: 2014-10-20, 01:06:51
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GAMESCREEN_H
#define GRIT_GAMESCREEN_H

#define gameScreenTilesLen 4736
extern const unsigned short gameScreenTiles[2368];

#define gameScreenMapLen 2048
extern const unsigned short gameScreenMap[1024];

#define gameScreenPalLen 512
extern const unsigned short gameScreenPal[256];

#endif // GRIT_GAMESCREEN_H

//}}BLOCK(gameScreen)
