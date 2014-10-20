
//{{BLOCK(instructionScreen)

//======================================================================
//
//	instructionScreen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 59 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 3776 + 2048 = 6336
//
//	Time-stamp: 2014-10-20, 01:07:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_INSTRUCTIONSCREEN_H
#define GRIT_INSTRUCTIONSCREEN_H

#define instructionScreenTilesLen 3776
extern const unsigned short instructionScreenTiles[1888];

#define instructionScreenMapLen 2048
extern const unsigned short instructionScreenMap[1024];

#define instructionScreenPalLen 512
extern const unsigned short instructionScreenPal[256];

#endif // GRIT_INSTRUCTIONSCREEN_H

//}}BLOCK(instructionScreen)
