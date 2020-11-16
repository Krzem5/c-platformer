#include <engine.h>
#include <renderer.h>
#include <level.h>
#include <stdio.h>



#define TOTAL_X_BLOCKS 30
#define TOTAL_Y_BLOCKS 20
#define SIZE_SCALE (((float)renderer_ww/TOTAL_X_BLOCKS)<((float)renderer_wh/TOTAL_Y_BLOCKS)?(float)renderer_ww/TOTAL_X_BLOCKS:(float)renderer_wh/TOTAL_Y_BLOCKS)
#define WIDTH_PAD ((renderer_ww-SIZE_SCALE*TOTAL_X_BLOCKS)/2)
#define HEIGHT_PAD ((renderer_wh-SIZE_SCALE*TOTAL_Y_BLOCKS)/2)



ID3D11Buffer* e_cb=NULL;



Level load_level(char* nm){
	size_t ln=0;
	while (*(nm+ln)!=0){
		ln++;
	}
	assert(ln!=0);
	size_t lnb=strlen(LEVEL_BASE_FILE_PATH);
	char* fp=malloc((ln+lnb+5)*sizeof(char));
	for (size_t i=0;i<lnb;i++){
		*(fp+i)=*(LEVEL_BASE_FILE_PATH+i);
	}
	for (size_t i=0;i<ln;i++){
		*(fp+i+lnb)=*(nm+i);
	}
	*(fp+lnb+ln)='.';
	*(fp+lnb+ln+1)='l';
	*(fp+lnb+ln+2)='v';
	*(fp+lnb+ln+3)='l';
	*(fp+lnb+ln+4)=0;
	FILE* f=NULL;
	errno_t e=fopen_s(&f,fp,"rb");
	free(fp);
	if (e==ENOENT){
		printf("File '%s' doesn't extist!\n",fp);
		return NULL;
	}
	uint32_t fm=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8)|((fgetc(f)&0xff)<<16)|((fgetc(f)&0xff)<<24);
	assert(fm==LEVEL_FILE_MAGIC);
	Level o=malloc(sizeof(struct _LEVEL));
	o->nml=fgetc(f)&0xff;
	assert(o->nml<=20);
	o->nm=malloc((o->nml+1)*sizeof(char));
	for (size_t i=0;i<o->nml;i++){
		*(o->nm+i)=(fgetc(f)&0xff);
	}
	*(o->nm+o->nml)=0;
	o->sx=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8);
	o->sy=o->sx&0x3f;
	o->sx>>=6;
	o->cx=0;
	o->cy=(float)(o->sy-TOTAL_Y_BLOCKS);
	if (o->cy<0){
		o->cy=0;
	}
	o->p=malloc(sizeof(struct _PLAYER));
	o->p->x=1.5f;
	o->p->y=(float)(fgetc(f)&0xff)-0.5f;
	uint16_t* pil=malloc(6*sizeof(uint16_t));
	*pil=0;
	*(pil+1)=1;
	*(pil+2)=2;
	*(pil+3)=0;
	*(pil+4)=2;
	*(pil+5)=3;
	o->p->as=TILEMAP_TEX_IMG_PLAYER_STAND_FRONT;
	o->p->vl=malloc(16*sizeof(float));
	*o->p->vl=(o->p->x-0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(o->p->vl+1)=(o->sy-o->p->y-0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(o->p->vl+2)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,o->p->as);
	*(o->p->vl+3)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,o->p->as);
	*(o->p->vl+4)=(o->p->x+0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(o->p->vl+5)=(o->sy-o->p->y-0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(o->p->vl+6)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,o->p->as);
	*(o->p->vl+7)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,o->p->as);
	*(o->p->vl+8)=(o->p->x+0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(o->p->vl+9)=(o->sy-o->p->y+0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(o->p->vl+10)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,o->p->as);
	*(o->p->vl+11)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,o->p->as);
	*(o->p->vl+12)=(o->p->x-0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(o->p->vl+13)=(o->sy-o->p->y+0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(o->p->vl+14)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,o->p->as);
	*(o->p->vl+15)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,o->p->as);
	o->p->_u=false;
	D3D11_BUFFER_DESC bd={
		(uint32_t)(6*sizeof(uint16_t)),
		D3D11_USAGE_IMMUTABLE,
		D3D11_BIND_INDEX_BUFFER,
		0,
		0,
		0
	};
	D3D11_SUBRESOURCE_DATA dt={
		pil,
		0,
		0
	};
	o->p->_ib=NULL;
	HRESULT hr=ID3D11Device_CreateBuffer(renderer_d3_d,&bd,&dt,&(o->p->_ib));
	assert(hr==S_OK);
	free(pil);
	bd.ByteWidth=(uint32_t)(16*sizeof(float));
	bd.Usage=D3D11_USAGE_DYNAMIC;
	bd.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	dt.pSysMem=o->p->vl;
	o->p->_vb=NULL;
	hr=ID3D11Device_CreateBuffer(renderer_d3_d,&bd,&dt,&(o->p->_vb));
	assert(hr==S_OK);
	o->bll=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8);
	uint32_t* il=malloc(o->bll*6*sizeof(uint32_t));
	float* vl=malloc(o->bll*16*sizeof(float));
	for (uint32_t i=0;i<o->bll;i++){
		uint8_t t=(fgetc(f)&0xff);
		uint16_t x=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8);
		uint8_t y=x&0x3f;
		x>>=6;
		*(vl+i*16)=x*SIZE_SCALE+WIDTH_PAD;
		*(vl+i*16+1)=(o->sy-y)*SIZE_SCALE+HEIGHT_PAD;
		*(vl+i*16+2)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,t);
		*(vl+i*16+3)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,t);
		*(vl+i*16+4)=x*SIZE_SCALE+WIDTH_PAD;
		*(vl+i*16+5)=(o->sy-y-1)*SIZE_SCALE+HEIGHT_PAD;
		*(vl+i*16+6)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,t);
		*(vl+i*16+7)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,t);
		*(vl+i*16+8)=(x+1)*SIZE_SCALE+WIDTH_PAD;
		*(vl+i*16+9)=(o->sy-y-1)*SIZE_SCALE+HEIGHT_PAD;
		*(vl+i*16+10)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,t);
		*(vl+i*16+11)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,t);
		*(vl+i*16+12)=(x+1)*SIZE_SCALE+WIDTH_PAD;
		*(vl+i*16+13)=(o->sy-y)*SIZE_SCALE+HEIGHT_PAD;
		*(vl+i*16+14)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,t);
		*(vl+i*16+15)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,t);
		*(il+i*6)=i*4;
		*(il+i*6+1)=i*4+1;
		*(il+i*6+2)=i*4+2;
		*(il+i*6+3)=i*4;
		*(il+i*6+4)=i*4+2;
		*(il+i*6+5)=i*4+3;
	}
	bd.ByteWidth=(uint32_t)(o->bll*6*sizeof(uint32_t));
	bd.Usage=D3D11_USAGE_IMMUTABLE;
	bd.BindFlags=D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags=0;
	dt.pSysMem=il;
	hr=ID3D11Device_CreateBuffer(renderer_d3_d,&bd,&dt,&(o->bl_ib));
	free(il);
	bd.ByteWidth=(uint32_t)(o->bll*16*sizeof(float));
	bd.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	dt.pSysMem=vl;
	hr=ID3D11Device_CreateBuffer(renderer_d3_d,&bd,&dt,&(o->bl_vb));
	free(vl);
	fclose(f);
	return o;
}



void update_level(Level l,float dt){
	assert(l!=NULL);
}



void draw_level(Level l){
	assert(l!=NULL);
	if (e_cb==NULL){
		e_cb=create_constant_buffer(sizeof(CBufferExtraLayout));
	}
	CBufferLayout e_cb_dt={
		raw_matrix(1,0,0,0,0,1,0,0,0,0,1,0,-l->cx*SIZE_SCALE,-l->cy*SIZE_SCALE,0,1)
	};
	update_constant_buffer(e_cb,(void*)&e_cb_dt);
	ID3D11DeviceContext_VSSetConstantBuffers(renderer_d3_dc,1,1,&e_cb);
	ID3D11DeviceContext_PSSetConstantBuffers(renderer_d3_dc,1,1,&e_cb);
	unsigned int off=0;
	unsigned int st=4*sizeof(float);
	ID3D11DeviceContext_IASetVertexBuffers(renderer_d3_dc,0,1,&(l->bl_vb),&st,&off);
	ID3D11DeviceContext_IASetIndexBuffer(renderer_d3_dc,l->bl_ib,DXGI_FORMAT_R32_UINT,0);
	ID3D11DeviceContext_IASetPrimitiveTopology(renderer_d3_dc,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ID3D11DeviceContext_DrawIndexed(renderer_d3_dc,l->bll*6,0,0);
	ID3D11DeviceContext_IASetVertexBuffers(renderer_d3_dc,0,1,&(l->p->_vb),&st,&off);
	ID3D11DeviceContext_IASetIndexBuffer(renderer_d3_dc,l->p->_ib,DXGI_FORMAT_R16_UINT,0);
	ID3D11DeviceContext_DrawIndexed(renderer_d3_dc,6,0,0);
}
