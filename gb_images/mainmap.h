
//{{BLOCK(mainmap)

//======================================================================
//
//	mainmap, 512x512@8, 
//	+ palette 256 entries, not compressed
//	+ 102 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 512 + 6528 + 8192 = 15232
//
//	Time-stamp: 2014-11-08, 05:10:34
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAINMAP_H
#define GRIT_MAINMAP_H

#define mainmapTilesLen 6528
extern const unsigned short mainmapTiles[3264];

#define mainmapMapLen 8192
extern const unsigned short mainmapMap[4096];

#define mainmapPalLen 512
extern const unsigned short mainmapPal[256];

#endif // GRIT_MAINMAP_H

//}}BLOCK(mainmap)
