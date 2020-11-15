#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include <engine.h>



typedef struct _RAW_TEXTURE RawTexture;
typedef ID3D11ShaderResourceView* Texture;



struct _RAW_TEXTURE{
	uint32_t f;
	uint32_t w;
	uint32_t h;
	uint32_t p;
	uint32_t sp;
	const uint8_t* dt;
};



Texture create_texture(RawTexture r);



#endif
