#ifndef __LEVEL_H__
#define __LEVEL_H__
#include <common.h>



#define LEVEL_BASE_FILE_PATH "rsrc/levels/"
#define LEVEL_FILE_MAGIC 0x204c764c
#define LEVEL_MAX_WIDTH 1024
#define LEVEL_MAX_HEIGHT 64



typedef struct _LEVEL* Level;
typedef struct _PLAYER* Player;



enum PLAYER_ANIM_STAGE{
	PLAYER_ANIM_STAGE_FRONT_STAND=0x00,
	PLAYER_ANIM_STAGE_STAND=0x01,
	PLAYER_ANIM_STAGE_JUMP=0x02,
	PLAYER_ANIM_STAGE_DUCK=0x03,
	PLAYER_ANIM_STAGE_HIT=0x04,
	PLAYER_ANIM_STAGE_CLIMB=0x05,
	PLAYER_ANIM_STAGE_CLIMB_2=0x06,
	PLAYER_ANIM_STAGE_SWIM=0x07,
	PLAYER_ANIM_STAGE_SWIM_2=0x08,
	PLAYER_ANIM_STAGE_WALK=0x09,
	PLAYER_ANIM_STAGE_WALK_2=0x0a
};



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
	enum PLAYER_ANIM_STAGE as;
	float* vl;
	bool _u;
	ID3D11Buffer* _ib;
	ID3D11Buffer* _vb;
};



Level load_level(char* nm);



void update_level(Level l,float dt);



void draw_level(Level l);



#endif
