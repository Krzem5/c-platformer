#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <common.h>
#include <ds4.h>
#include <tilemap.h>



#define PLAYER_START_X 1
#define PLAYER_WIDTH 0.8f
#define PLAYER_WALK_SPEED 3
#define PLAYER_DASH_SPEED_MLT 2.25f
#define PLAYER_WALK_FRAME_TIME 0.125f
#define PLAYER_DASH_FRAME_TIME 0.075f
#define PLAYER_AIR_WALK_SPEED 0.01f
#define PLAYER_JUMP 13
#define PLAYER_GRAVITY 0.1f
#define PLAYER_AIR_FRICTION 0.995f
#define PLAYER_MAX_X_SPEED (PLAYER_WALK_SPEED*PLAYER_DASH_SPEED_MLT)
#define PLAYER_MAX_Y_SPEED 25.0f
#define PLAYER_DEATH_ANIM_END 1.25f
#define PLAYER_DEATH_ANIM_FLASH 0.25f
#define PLAYER_DEATH_ANIM_R 50
#define PLAYER_DEATH_ANIM_G 0
#define PLAYER_DEATH_ANIM_B 0
#define PLAYER_DEATH_RUMBLE_S 128
#define PLAYER_DEATH_RUMBLE_F 255
#define PLAYER_JOYSTICK_X_BUFFOR 32
#define PLAYER_JOYSTICK_Y_BUFFOR 96



typedef struct _PLAYER* Player;
#include <level.h>



struct _PLAYER{
	Level l;
	float x;
	float y;
	float vx;
	float vy;
	uint8_t d;
	enum TILEMAP_TEX_IMG as;
	bool as_f;
	float as_tm;
	float* vl;
	bool on_g;
	DS4Device dd;
	float _sx;
	float _sy;
	bool _u;
	uint8_t _dft;
	float _dtm;
	float _dftm;
	ID3D11Buffer* _ib;
	ID3D11Buffer* _vb;
};



Player create_player(Level l,uint16_t x,uint8_t y);



void update_player(Player p,float dt);



void draw_player(Player p);



#endif
