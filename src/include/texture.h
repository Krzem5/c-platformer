#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include <common.h>



typedef struct _RAW_TEXTURE RawTexture;
typedef ID3D11ShaderResourceView* Texture;
typedef struct _TEXTURE_MAP_DATA TextureMapData;
typedef struct _TEXTURE_MAP TextureMap;



struct _RAW_TEXTURE{
	uint32_t f;
	uint32_t w;
	uint32_t h;
	uint32_t p;
	uint32_t sp;
	const uint8_t* dt;
};



struct _TEXTURE_MAP_DATA{
	float x0;
	float y0;
	float x1;
	float y1;
};



struct _TEXTURE_MAP{
	uint32_t l;
	const TextureMapData* dt;
};



Texture create_texture(RawTexture r);



#endif
