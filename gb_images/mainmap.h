
//{{BLOCK(mainmap)

//======================================================================
//
//	mainmap, 512x512@8, 
//	+ palette 256 entries, not compressed
//	+ 223 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 512 + 14272 + 8192 = 22976
//
//	Time-stamp: 2014-12-01, 02:02:30
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAINMAP_H
#define GRIT_MAINMAP_H

#define mainmapTilesLen 14272
extern const unsigned short mainmapTiles[7136];

#define mainmapMapLen 8192
extern const unsigned short mainmapMap[4096];

#define mainmapPalLen 512
extern const unsigned short mainmapPal[256];

#endif // GRIT_MAINMAP_H

//}}BLOCK(mainmap)
