#ifndef __LEVEL_H__
#define __LEVEL_H__
#include <common.h>



#define LEVEL_BASE_FILE_PATH "rsrc/levels/"
#define LEVEL_FILE_MAGIC 0x204c764c
#define TOTAL_X_BLOCKS 20
#define TOTAL_Y_BLOCKS 15
#define SIZE_SCALE (((float)renderer_ww/TOTAL_X_BLOCKS)<((float)renderer_wh/TOTAL_Y_BLOCKS)?(float)renderer_ww/TOTAL_X_BLOCKS:(float)renderer_wh/TOTAL_Y_BLOCKS)
#define WIDTH_PAD ((renderer_ww-SIZE_SCALE*TOTAL_X_BLOCKS)/2)
#define HEIGHT_PAD ((renderer_wh-SIZE_SCALE*TOTAL_Y_BLOCKS)/2)
#define SIDE_TILE_SIZE 128
#define SIDE_TILE_OFFSET 80
#define SIDE_TILE_MOVE_SPEED 1
#define COLLISION_DATA_INDEX(c,x,y) (*(c->b_dt+y*c->_w+x))
#define COLLISION_DATA_XY_FROM_INDEX(c,i,x,y) \
	do { \
		x=(float)(i%((c)->_w)); \
		y=(float)(i/((c)->_w)); \
	} while(0)



typedef struct _LEVEL_COLLISION_DATA* CollisionData;
typedef struct _LEVEL* Level;
#include <player.h>



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
	CollisionData c_dt;
	Player p;
	float pd_y;
	uint32_t pdll;
	ID3D11Buffer* pd_ib;
	ID3D11Buffer* pd_vb;
	uint32_t bll;
	ID3D11Buffer* bl_ib;
	ID3D11Buffer* bl_vb;
};



Level load_level(char* nm);



void update_level(Level l,float dt);



void draw_level(Level l);



#endif
