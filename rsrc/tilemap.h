#ifndef __TILEMAP_H__
#define __TILEMAP_H__
#include <common.h>
#include <texture.h>



enum TILEMAP_TEX_IMG{
	TILEMAP_TEX_IMG_SIDE=0x0,
	TILEMAP_TEX_IMG_PLAYER_CLIMB_1=0x1,
	TILEMAP_TEX_IMG_PLAYER_CLIMB_2=0x2,
	TILEMAP_TEX_IMG_PLAYER_DUCK=0x3,
	TILEMAP_TEX_IMG_PLAYER_HURT=0x4,
	TILEMAP_TEX_IMG_PLAYER_JUMP=0x5,
	TILEMAP_TEX_IMG_PLAYER_STAND=0x6,
	TILEMAP_TEX_IMG_PLAYER_STAND_FRONT=0x7,
	TILEMAP_TEX_IMG_PLAYER_SWIM_1=0x8,
	TILEMAP_TEX_IMG_PLAYER_SWIM_2=0x9,
	TILEMAP_TEX_IMG_PLAYER_WALK_1=0xa,
	TILEMAP_TEX_IMG_PLAYER_WALK_2=0xb,
	TILEMAP_TEX_IMG_BLOCK_GRASS=0xc,
	TILEMAP_TEX_IMG_BLOCK_GRASS_CENTER=0xd,
	TILEMAP_TEX_IMG_BLOCK_GRASS_CENTER_ROUNDED=0xe,
	TILEMAP_TEX_IMG_BLOCK_GRASS_CLIFF_LEFT=0xf,
	TILEMAP_TEX_IMG_BLOCK_GRASS_CLIFF_LEFT_ALT=0x10,
	TILEMAP_TEX_IMG_BLOCK_GRASS_CLIFF_RIGHT=0x11,
	TILEMAP_TEX_IMG_BLOCK_GRASS_CLIFF_RIGHT_ALT=0x12,
	TILEMAP_TEX_IMG_BLOCK_GRASS_HALF=0x13,
	TILEMAP_TEX_IMG_BLOCK_GRASS_HALF_LEFT=0x14,
	TILEMAP_TEX_IMG_BLOCK_GRASS_HALF_MID=0x15,
	TILEMAP_TEX_IMG_BLOCK_GRASS_HALF_RIGHT=0x16,
	TILEMAP_TEX_IMG_BLOCK_GRASS_HILL_LEFT=0x17,
	TILEMAP_TEX_IMG_BLOCK_GRASS_HILL_LEFT_2=0x18,
	TILEMAP_TEX_IMG_BLOCK_GRASS_HILL_RIGHT=0x19,
	TILEMAP_TEX_IMG_BLOCK_GRASS_HILL_RIGHT_2=0x1a,
	TILEMAP_TEX_IMG_BLOCK_GRASS_LEFT=0x1b,
	TILEMAP_TEX_IMG_BLOCK_GRASS_MID=0x1c,
	TILEMAP_TEX_IMG_BLOCK_GRASS_RIGHT=0x1d,
	TILEMAP_TEX_IMG_PLAYER_BADGE_1=0x1e,
	TILEMAP_TEX_IMG_BLOCK_GRASS_LEDGE_LEFT=0x1f,
	TILEMAP_TEX_IMG_BLOCK_GRASS_LEDGE_RIGHT=0x20,
};



extern const uint8_t TILEMAP_TEX_DATA[];



extern const RawTexture TILEMAP_TEX;



extern const TextureMapData TILEMAP_TEX_MAP_DATA[];



#endif
