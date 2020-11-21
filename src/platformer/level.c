#include <common.h>
#include <level.h>
#include <engine.h>
#include <player.h>
#include <renderer.h>
#include <color_2d_pixel.h>
#include <color_2d_vertex.h>
#include <texture_2d_vertex.h>
#include <texture_2d_pixel.h>
#include <stdio.h>



ID3D11VertexShader* cl_vs=NULL;
ID3D11PixelShader* cl_ps=NULL;
ID3D11InputLayout* cl_vl=NULL;
ID3D11VertexShader* tx_vs=NULL;
ID3D11PixelShader* tx_ps=NULL;
ID3D11InputLayout* tx_vl=NULL;
ID3D11SamplerState* tx_ss=NULL;
ID3D11ShaderResourceView* tx_sr=NULL;
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
	o->c_dt=malloc(sizeof(struct _LEVEL_COLLISION_DATA));
	o->c_dt->b_dtl=o->sx*o->sy;
	o->c_dt->b_dt=malloc(o->sx*o->sy*sizeof(uint8_t));
	o->c_dt->_w=o->sx;
	for (uint32_t i=0;i<o->c_dt->b_dtl;i++){
		*(o->c_dt->b_dt+i)=0;
	}
	o->p=create_player(o,PLAYER_START_X,fgetc(f)&0xff);
	o->bll=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8);
	uint16_t pd_x=((uint16_t)WIDTH_PAD+SIDE_TILE_SIZE-1)/SIDE_TILE_SIZE+1;
	uint16_t pd_y=(renderer_wh+SIDE_TILE_SIZE-1)/SIDE_TILE_SIZE+1;
	o->pd_y=0;
	o->pdll=pd_x*pd_y;
	uint16_t* pd_il=malloc(o->pdll*6*sizeof(uint16_t));
	float* pd_vl=malloc(o->pdll*16*sizeof(float));
	for (uint16_t i=0;i<pd_x;i++){
		for (uint16_t j=0;j<pd_y;j++){
			uint32_t k=((uint32_t)j)*pd_x+i;
			*(pd_vl+k*16)=(float)(i*SIDE_TILE_SIZE)-SIDE_TILE_OFFSET;
			*(pd_vl+k*16+1)=(float)(j*SIDE_TILE_SIZE)-SIDE_TILE_SIZE;
			*(pd_vl+k*16+2)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,TILEMAP_TEX_IMG_SIDE);
			*(pd_vl+k*16+3)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,TILEMAP_TEX_IMG_SIDE);
			*(pd_vl+k*16+4)=(float)(i*SIDE_TILE_SIZE+SIDE_TILE_SIZE)-SIDE_TILE_OFFSET;
			*(pd_vl+k*16+5)=(float)(j*SIDE_TILE_SIZE)-SIDE_TILE_SIZE;
			*(pd_vl+k*16+6)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,TILEMAP_TEX_IMG_SIDE);
			*(pd_vl+k*16+7)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,TILEMAP_TEX_IMG_SIDE);
			*(pd_vl+k*16+8)=(float)(i*SIDE_TILE_SIZE+SIDE_TILE_SIZE)-SIDE_TILE_OFFSET;
			*(pd_vl+k*16+9)=(float)(j*SIDE_TILE_SIZE+SIDE_TILE_SIZE)-SIDE_TILE_SIZE;
			*(pd_vl+k*16+10)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,TILEMAP_TEX_IMG_SIDE);
			*(pd_vl+k*16+11)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,TILEMAP_TEX_IMG_SIDE);
			*(pd_vl+k*16+12)=(float)(i*SIDE_TILE_SIZE)-SIDE_TILE_OFFSET;
			*(pd_vl+k*16+13)=(float)(j*SIDE_TILE_SIZE+SIDE_TILE_SIZE)-SIDE_TILE_SIZE;
			*(pd_vl+k*16+14)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,TILEMAP_TEX_IMG_SIDE);
			*(pd_vl+k*16+15)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,TILEMAP_TEX_IMG_SIDE);
			*(pd_il+k*6)=k*4;
			*(pd_il+k*6+1)=k*4+1;
			*(pd_il+k*6+2)=k*4+2;
			*(pd_il+k*6+3)=k*4;
			*(pd_il+k*6+4)=k*4+2;
			*(pd_il+k*6+5)=k*4+3;
		}
	}
	D3D11_BUFFER_DESC bd={
		(uint32_t)(o->pdll*6*sizeof(uint16_t)),
		D3D11_USAGE_IMMUTABLE,
		D3D11_BIND_INDEX_BUFFER,
		0,
		0,
		0
	};
	D3D11_SUBRESOURCE_DATA dt={
		pd_il,
		0,
		0
	};
	HRESULT hr=ID3D11Device_CreateBuffer(renderer_d3_d,&bd,&dt,&(o->pd_ib));
	free(pd_il);
	bd.ByteWidth=(uint32_t)(o->pdll*16*sizeof(float));
	bd.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	dt.pSysMem=pd_vl;
	hr=ID3D11Device_CreateBuffer(renderer_d3_d,&bd,&dt,&(o->pd_vb));
	free(pd_vl);
	uint32_t* il=malloc(o->bll*6*sizeof(uint32_t));
	float* vl=malloc(o->bll*16*sizeof(float));
	for (uint32_t i=0;i<o->bll;i++){
		uint8_t t=(fgetc(f)&0xff);
		uint16_t x=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8);
		uint8_t y=x&0x3f;
		x>>=6;
		COLLISION_DATA_INDEX(o->c_dt,x,y)=1;
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
	bd.BindFlags=D3D11_BIND_INDEX_BUFFER;
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
	update_player(l->p,dt);
	l->pd_y+=dt*SIDE_TILE_MOVE_SPEED*SIDE_TILE_SIZE;
	if (l->pd_y>SIDE_TILE_SIZE){
		l->pd_y-=SIDE_TILE_SIZE;
	}
}



void draw_level(Level l){
	assert(l!=NULL);
	if (e_cb==NULL){
		e_cb=create_constant_buffer(sizeof(CBufferExtraLayout));
	}
	if (cl_vs==NULL||cl_vl==NULL||cl_ps==NULL||tx_vs==NULL||tx_vl==NULL||tx_ps==NULL||tx_sr==NULL||tx_ss==NULL){
		assert(cl_vs==NULL);
		assert(cl_vl==NULL);
		assert(cl_ps==NULL);
		assert(tx_vs==NULL);
		assert(tx_vl==NULL);
		assert(tx_ps==NULL);
		assert(tx_sr==NULL);
		assert(tx_ss==NULL);
		D3D11_INPUT_ELEMENT_DESC cl_il[]={
			{
				"POSITION",
				0,
				DXGI_FORMAT_R32G32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			{
				"COLOR",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			}
		};
		cl_vs=load_vertex_shader(g_color_2d_vs,sizeof(g_color_2d_vs),cl_il,sizeof(cl_il)/sizeof(D3D11_INPUT_ELEMENT_DESC),&cl_vl);
		cl_ps=load_pixel_shader(g_color_2d_ps,sizeof(g_color_2d_ps));
		D3D11_INPUT_ELEMENT_DESC tx_il[]={
			{
				"POSITION",
				0,
				DXGI_FORMAT_R32G32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			{
				"TEXCOORD",
				0,
				DXGI_FORMAT_R32G32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			}
		};
		tx_vs=load_vertex_shader(g_texture_2d_vs,sizeof(g_texture_2d_vs),tx_il,sizeof(tx_il)/sizeof(D3D11_INPUT_ELEMENT_DESC),&tx_vl);
		tx_ps=load_pixel_shader(g_texture_2d_ps,sizeof(g_texture_2d_ps));
		RawTexture r=TILEMAP_TEX;
		D3D11_TEXTURE2D_DESC d={
			r.w,
			r.h,
			1,
			1,
			r.f,
			{
				1,
				0
			},
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			0
		};
		D3D11_SUBRESOURCE_DATA sd={
			r.dt,
			r.p,
			r.sp
		};
		ID3D11Texture2D* t=NULL;
		assert(ID3D11Device_CreateTexture2D(renderer_d3_d,&d,&sd,&t)==S_OK);
		D3D11_SHADER_RESOURCE_VIEW_DESC rvd={
			r.f,
			D3D_SRV_DIMENSION_TEXTURE2D,
			{
				.Texture2D={
					0,
					1
				}
			}
		};
		ID3D11Resource* tr;
		ID3D11Texture2D_QueryInterface(t,&IID_ID3D11Resource,(void**)&tr);
		assert(ID3D11Device_CreateShaderResourceView(renderer_d3_d,tr,&rvd,&tx_sr)==S_OK);
		IUnknown_Release(tr);
		D3D11_SAMPLER_DESC ss_d={
			D3D11_FILTER_MIN_MAG_MIP_POINT,
			D3D11_TEXTURE_ADDRESS_WRAP,
			D3D11_TEXTURE_ADDRESS_WRAP,
			D3D11_TEXTURE_ADDRESS_WRAP,
			0,
			1,
			D3D11_COMPARISON_ALWAYS,
			{
				0,
				0,
				0,
				0
			},
			0,
			D3D11_FLOAT32_MAX
		};
		assert(ID3D11Device_CreateSamplerState(renderer_d3_d,&ss_d,&tx_ss)==S_OK);
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		ShowCursor(false);
	}
	float bf[]={0,0,0,0};
	ID3D11DeviceContext_OMSetBlendState(renderer_d3_dc,renderer_d3_bse,bf,0xffffffff);
	ID3D11DeviceContext_OMSetDepthStencilState(renderer_d3_dc,renderer_d3_ddss,1);
	ID3D11DeviceContext_IASetInputLayout(renderer_d3_dc,tx_vl);
	ID3D11DeviceContext_PSSetSamplers(renderer_d3_dc,0,1,&tx_ss);
	ID3D11DeviceContext_PSSetShaderResources(renderer_d3_dc,0,1,&tx_sr);
	CBufferExtraLayout e_cb_dt={
		raw_matrix(1,0,0,0,0,1,0,0,0,0,1,0,-l->cx*SIZE_SCALE,-l->cy*SIZE_SCALE,0,1)
	};
	update_constant_buffer(e_cb,(void*)&e_cb_dt);
	ID3D11DeviceContext_VSSetShader(renderer_d3_dc,cl_vs,NULL,0);
	ID3D11DeviceContext_PSSetShader(renderer_d3_dc,cl_ps,NULL,0);
	unsigned int off=0;
	unsigned int st=4*sizeof(float);
	// ID3D11DeviceContext_IASetVertexBuffers(renderer_d3_dc,0,1,&(l->bl_vb),&st,&off);
	// ID3D11DeviceContext_IASetIndexBuffer(renderer_d3_dc,l->bl_ib,DXGI_FORMAT_R16_UINT,0);
	ID3D11DeviceContext_IASetPrimitiveTopology(renderer_d3_dc,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// ID3D11DeviceContext_DrawIndexed(renderer_d3_dc,l->bll*6,0,0);
	ID3D11DeviceContext_VSSetShader(renderer_d3_dc,tx_vs,NULL,0);
	ID3D11DeviceContext_PSSetShader(renderer_d3_dc,tx_ps,NULL,0);
	ID3D11DeviceContext_VSSetConstantBuffers(renderer_d3_dc,1,1,&e_cb);
	ID3D11DeviceContext_PSSetConstantBuffers(renderer_d3_dc,1,1,&e_cb);
	ID3D11DeviceContext_IASetVertexBuffers(renderer_d3_dc,0,1,&(l->bl_vb),&st,&off);
	ID3D11DeviceContext_IASetIndexBuffer(renderer_d3_dc,l->bl_ib,DXGI_FORMAT_R32_UINT,0);
	ID3D11DeviceContext_DrawIndexed(renderer_d3_dc,l->bll*6,0,0);
	draw_player(l->p);
	// ID3D11DeviceContext_IASetVertexBuffers(renderer_d3_dc,0,1,&(l->p->_vb),&st,&off);
	// ID3D11DeviceContext_IASetIndexBuffer(renderer_d3_dc,l->p->_ib,DXGI_FORMAT_R16_UINT,0);
	// ID3D11DeviceContext_DrawIndexed(renderer_d3_dc,6,0,0);
	e_cb_dt.wm=raw_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,l->pd_y,0,1);
	update_constant_buffer(e_cb,(void*)&e_cb_dt);
	ID3D11DeviceContext_IASetVertexBuffers(renderer_d3_dc,0,1,&(l->pd_vb),&st,&off);
	ID3D11DeviceContext_IASetIndexBuffer(renderer_d3_dc,l->pd_ib,DXGI_FORMAT_R16_UINT,0);
	ID3D11DeviceContext_DrawIndexed(renderer_d3_dc,l->pdll*6,0,0);
}
