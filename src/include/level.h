#ifndef __LEVEL_H__
#define __LEVEL_H__
#include <common.h>
#include <ds4.h>
#include <tilemap.h>



#define LEVEL_BASE_FILE_PATH "rsrc/levels/"
#define LEVEL_FILE_MAGIC 0x204c764c
#define LEVEL_MAX_WIDTH 1024
#define LEVEL_MAX_HEIGHT 64



typedef struct _LEVEL_COLLISION_DATA* CollisionData;
typedef struct _LEVEL* Level;
typedef struct _PLAYER* Player;



struct _LEVEL_COLLISION_DATA{
	uint32_t b_dtl;
	uint8_t* b_dt;
	uint16_t _w;
};



struct _LEVEL{
	uint8_t nml;
	char* nm;
	uint16_t sx;
	uint8_t sy;
	float cx;
	float cy;
	struct _LEVEL_COLLISION_DATA* c_dt;
	Player p;
	uint32_t pdll;
	ID3D11Buffer* pd_ib;
	ID3D11Buffer* pd_vb;
	uint32_t bll;
	ID3D11Buffer* bl_ib;
	ID3D11Buffer* bl_vb;
};



struct _PLAYER{
	float x;
	float y;
	float vx;
	float vy;
	uint8_t d;
	enum TILEMAP_TEX_IMG as;
	bool as_f;
	float* vl;
	bool on_g;
	DS4Device dd;
	bool _u;
	uint8_t _dft;
	float _dtm;
	float _dftm;
	ID3D11Buffer* _ib;
	ID3D11Buffer* _vb;
};



Level load_level(char* nm);



void update_level(Level l,float dt);



void draw_level(Level l);



#endif
