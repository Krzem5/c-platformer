#ifndef __LEVEL_H__
#define __LEVEL_H__
#include <common.h>
#include <tilemap.h>



#define LEVEL_BASE_FILE_PATH "rsrc/levels/"
#define LEVEL_FILE_MAGIC 0x204c764c
#define LEVEL_MAX_WIDTH 1024
#define LEVEL_MAX_HEIGHT 64



typedef struct _LEVEL* Level;
typedef struct _PLAYER* Player;



struct _LEVEL{
	uint8_t nml;
	char* nm;
	uint16_t sx;
	uint8_t sy;
	float cx;
	float cy;
	Player p;
	uint32_t bll;
	ID3D11Buffer* bl_ib;
	ID3D11Buffer* bl_vb;
};



struct _PLAYER{
	float x;
	float y;
	enum TILEMAP_TEX_IMG as;
	float* vl;
	bool _u;
	ID3D11Buffer* _ib;
	ID3D11Buffer* _vb;
};



Level load_level(char* nm);



void update_level(Level l,float dt);



void draw_level(Level l);



#endif
