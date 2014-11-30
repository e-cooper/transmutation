
//{{BLOCK(mainmap)

//======================================================================
//
//	mainmap, 512x512@8, 
//	+ palette 256 entries, not compressed
//	+ 133 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 512 + 8512 + 8192 = 17216
//
//	Time-stamp: 2014-11-30, 01:24:12
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAINMAP_H
#define GRIT_MAINMAP_H

#define mainmapTilesLen 8512
extern const unsigned short mainmapTiles[4256];

#define mainmapMapLen 8192
extern const unsigned short mainmapMap[4096];

#define mainmapPalLen 512
extern const unsigned short mainmapPal[256];

#endif // GRIT_MAINMAP_H

//}}BLOCK(mainmap)
