#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__
#include <engine.h>
#define COBJMACROS
#include <d3d11_1.h>



typedef struct _BLOCK_MODEL* BlockModel;
typedef struct _BLOCK_MODEL* AssetID;



struct _BLOCK_MODEL_PART{
	size_t ib_l;
	ID3D11Buffer* ib;
	ID3D11Buffer* vb;
};



struct _BLOCK_MODEL{
	uint8_t l;
	struct _BLOCK_MODEL_PART* e;
};



AssetID resource_manager_load_asset(uint8_t id);



void resource_manager_draw_asset(AssetID a,RawMatrix wm);



#endif
