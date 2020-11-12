#ifndef __LEVEL_RENDERER_H__
#define __LEVEL_RENDERER_H__
#include <engine.h>
#include <resource_manager.h>



#define LEVEL_BASE_FILE_PATH "rsrc/levels/"
#define LEVEL_FILE_MAGIC 0x204c764c
#define LEVEL_MAX_WIDTH 4096
#define LEVEL_MAX_HEIGHT 256



typedef struct _LEVEL_LAYOUT* LevelLayout;
typedef struct _LEVEL* Level;
typedef struct _PLAYER* Player;



struct _LEVEL_LAYOUT_BLOCK{
	uint8_t t;
	uint16_t x;
	uint8_t y;
	uint16_t z;
};



struct _LEVEL_BLOCK{
	AssetID a;
	uint16_t x;
	uint8_t y;
	uint16_t z;
};



struct _LEVEL_LAYOUT{
	uint8_t nml;
	char* nm;
	uint16_t sx;
	uint8_t sy;
	uint16_t sz;
	uint32_t bl;
	struct _LEVEL_LAYOUT_BLOCK* b;
};



struct _PLAYER{
	float x;
	float y;
	float z;
};



struct _LEVEL{
	LevelLayout ll;
	Player p;
	uint32_t bl;
	struct _LEVEL_BLOCK* b;
};



LevelLayout load_level(const char* nm);



Level create_level(LevelLayout ll);



void update_level(Level l,double dt);



void draw_level(Level l);



#endif
