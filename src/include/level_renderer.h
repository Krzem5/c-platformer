#ifndef __LEVEL_RENDERER_H__
#define __LEVEL_RENDERER_H__
#include <engine.h>
#include <d3d11_1.h>



#define LEVEL_BASE_FILE_PATH "rsrc/levels/"
#define LEVEL_FILE_SUFFIX ".lvl"
#define LEVEL_MAX_WIDTH 4096
#define LEVEL_MAX_HEIGHT 256



typedef struct _RAW_BLOCK_MODEL* RawBlockModel;
typedef struct _BLOCK_MODEL* BlockModel;
typedef struct _LEVEL_LAYOUT* LevelLayout;
typedef struct _LEVEL* Level;
typedef struct _PLAYER* Player;



struct _RAW_BLOCK_MODEL{
	size_t il;
	size_t vl;
	uint16_t* i;
	float* v;
};



struct _BLOCK_MODEL_PART{
	size_t ib_l;
	ID3D11Buffer* ib;
	ID3D11Buffer* vb;
};



struct _BLOCK_MODEL{
	uint8_t l;
	struct _BLOCK_MODEL_PART* e;
};



struct _LEVEL_LAYOUT_BLOCK{
	uint8_t t;
	uint16_t x;
	uint8_t y;
	uint16_t z;
	uint8_t* dt;
};



struct _LEVEL_LAYOUT{
	const char* nm;
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
	struct _BLOCK_MODEL* b;
};



LevelLayout load_level(const char* nm);



Level create_level(LevelLayout ll);



void update_level(Level l,double dt);



void draw_level(Level l);



#endif
