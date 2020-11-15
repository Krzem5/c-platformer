#include <engine.h>
#include <renderer.h>
#include <ui.h>
#include <texture_3d_vertex.h>
#include <texture_3d_pixel.h>
#include <tilemap.h>
#include <stdio.h>



typedef struct _CAMERA* Camera;



struct _CAMERA{
	float x;
	float y;
	float z;
	float rx;
	float ry;
	float rz;
	float zm;
	float ms;
	float rs;
	bool enabled;
	uint16_t _f;
	int16_t _rx;
	int16_t _ry;
	int16_t _px;
	int16_t _py;
	float _ox;
	float _oy;
	float _oz;
};




Camera create_camera(float ms,float rs,float x,float y,float z,float rx,float ry,float rz,float zm){
	Camera o=malloc(sizeof(struct _CAMERA));
	o->x=x;
	o->y=y;
	o->z=z;
	o->rx=(rx-90)*PI_DIV_180;
	o->ry=ry*PI_DIV_180;
	o->rz=rz*PI_DIV_180;
	o->zm=zm;
	o->ms=ms;
	o->rs=rs;
	o->enabled=false;
	o->_f=0;
	o->_rx=0;
	o->_ry=0;
	o->_px=0;
	o->_py=0;
	o->_ox=0;
	o->_oy=0;
	o->_oz=0;
	return o;
}



Matrix update_camera(Camera c,float dt){
	if (c->enabled==true){
		bool rc=false;
		if ((renderer_mf&M_RIGHT)!=0){
			rc=true;
			if ((c->_f&M_RIGHT)==0){
				c->_f|=M_RIGHT;
				c->_rx=renderer_mx;
				c->_ry=renderer_my;
			}
			c->rx-=(c->_ry-renderer_my)*c->rs*dt*PI_DIV_180;
			c->ry-=(c->_rx-renderer_mx)*c->rs*dt*PI_DIV_180;
			if (c->rx>PI_DIV_TWO-EPSILON){
				c->rx=PI_DIV_TWO-EPSILON;
			}
			else if (c->rx<-PI_DIV_TWO+EPSILON){
				c->rx=-PI_DIV_TWO+EPSILON;
			}
			c->_rx=renderer_mx;
			c->_ry=renderer_my;
		}
		else{
			c->_f&=(~M_RIGHT);
		}
		float dx=cosf(c->ry)*c->ms*dt;
		float dz=sinf(c->ry)*c->ms*dt;
		if (is_pressed(0x57)){
			rc=true;
			c->_ox+=dx;
			c->_oz+=dz;
		}
		if (is_pressed(0x53)){
			rc=true;
			c->_ox-=dx;
			c->_oz-=dz;
		}
		if (is_pressed(0x41)){
			rc=true;
			c->_ox+=dz;
			c->_oz-=dx;
		}
		if (is_pressed(0x44)){
			rc=true;
			c->_ox-=dz;
			c->_oz+=dx;
		}
		if (is_pressed(VK_SPACE)){
			rc=true;
			c->_oy+=c->ms*dt;
		}
		if (is_pressed(VK_LSHIFT)){
			rc=true;
			c->_oy-=c->ms*dt;
		}
		if (rc==true){
			c->x=c->_ox+c->zm*sinf(-c->rx+PI_DIV_TWO)*cosf(PI-c->ry);
			c->y=c->_oy+c->zm*cosf(-c->rx+PI_DIV_TWO);
			c->z=c->_oz+c->zm*sinf(-c->rx+PI_DIV_TWO)*sinf(PI-c->ry);
		}
	}
	return look_at_matrix(c->x,c->y,c->z,sinf(c->rx-PI_DIV_TWO)*cosf(c->ry),cosf(c->rx-PI_DIV_TWO),sinf(c->rx-PI_DIV_TWO)*sinf(c->ry),0,1,0);
}



struct CBufferLayout{
	RawMatrix cm;
	RawMatrix pm;
};



ID3D11VertexShader* t_vs=NULL;
ID3D11PixelShader* t_ps=NULL;
ID3D11InputLayout* t_vl=NULL;
Camera c;
RawMatrix pm;
ID3D11Buffer* cb=NULL;
/*******************/
ID3D11Buffer* tx_ib=NULL;
size_t tx_ib_l=0;
ID3D11Buffer* tx_vb=NULL;
ID3D11SamplerState* ss=NULL;
ID3D11ShaderResourceView* tx_sr=NULL;
/*******************/



void init_engine(void){
	D3D11_INPUT_ELEMENT_DESC t_il[]={
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
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
	t_vs=load_vertex_shader(g_texture_3d_vs,sizeof(g_texture_3d_vs),t_il,sizeof(t_il)/sizeof(D3D11_INPUT_ELEMENT_DESC),&t_vl);
	t_ps=load_pixel_shader(g_texture_3d_ps,sizeof(g_texture_3d_ps));
	cb=create_constant_buffer(sizeof(struct CBufferLayout));
	*renderer_cc=1;
	*(renderer_cc+1)=1;
	*(renderer_cc+2)=1;
	*(renderer_cc+3)=1;
	c=create_camera(0,0,10,10,-10,90,90,0,0);//create_camera(10,500,0,5,0,90,0,0,1);
	// c->enabled=true;
	float n=EPSILON;
	float f=1000;
	pm=raw_matrix(1/renderer_aspect_ratio,0,0,0,0,1,0,0,0,0,-2/(f-n),-(f+n)/(f-n),0,0,0,1);
	init_ui();
}



void update_engine(double dt){
	static float t=0;
	t+=(float)(dt*1e-6);
	update_ui(dt);
	if (renderer_w==NULL){
		return;
	}
	Matrix cm=update_camera(c,(float)(dt*1e-6));
	if (cm==NULL){
		return;
	}
	/******************/
	if (tx_ib==NULL||tx_vb==NULL){
		assert(tx_ib==NULL);
		assert(tx_vb==NULL);
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
		HRESULT hr=ID3D11Device_CreateTexture2D(renderer_d3_d,&d,&sd,&t);
		if (FAILED(hr)){
			assert(0);
		}
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
		ID3D11Device_CreateShaderResourceView(renderer_d3_d,tr,&rvd,&tx_sr);
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
		assert(ID3D11Device_CreateSamplerState(renderer_d3_d,&ss_d,&ss)==S_OK);
		tx_ib_l=6;
		uint32_t vlsz=4*5;
		uint16_t* il=malloc(6*sizeof(uint16_t));
		*il=0;
		*(il+1)=1;
		*(il+2)=2;
		*(il+3)=0;
		*(il+4)=2;
		*(il+5)=3;
		float* vl=malloc(4*5*sizeof(float));
		#define SIZE 20
		*vl=0;
		*(vl+1)=0;
		*(vl+2)=0;
		*(vl+3)=1;
		*(vl+4)=1;
		*(vl+5)=SIZE;
		*(vl+6)=0;
		*(vl+7)=0;
		*(vl+8)=0;
		*(vl+9)=1;
		*(vl+10)=SIZE;
		*(vl+11)=SIZE;
		*(vl+12)=0;
		*(vl+13)=0;
		*(vl+14)=0;
		*(vl+15)=0;
		*(vl+16)=SIZE;
		*(vl+17)=0;
		*(vl+18)=1;
		*(vl+19)=0;
		D3D11_BUFFER_DESC bd={
			(uint32_t)(tx_ib_l*sizeof(uint16_t)),
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0,
			0
		};
		D3D11_SUBRESOURCE_DATA dt={
			il,
			0,
			0
		};
		hr=ID3D11Device_CreateBuffer(renderer_d3_d,&bd,&dt,&tx_ib);
		bd.ByteWidth=(uint32_t)(vlsz*sizeof(float));
		bd.BindFlags=D3D11_BIND_VERTEX_BUFFER;
		dt.pSysMem=vl;
		hr=ID3D11Device_CreateBuffer(renderer_d3_d,&bd,&dt,&tx_vb);
	}
	/******************/
	struct CBufferLayout cb_dt={
		as_raw_matrix(cm),
		pm
	};
	update_constant_buffer(cb,(void*)&cb_dt);
	ID3D11DeviceContext_VSSetConstantBuffers(renderer_d3_dc,0,1,&cb);
	ID3D11DeviceContext_PSSetConstantBuffers(renderer_d3_dc,0,1,&cb);
	ID3D11DeviceContext_VSSetShader(renderer_d3_dc,t_vs,NULL,0);
	ID3D11DeviceContext_PSSetShader(renderer_d3_dc,t_ps,NULL,0);
	ID3D11DeviceContext_IASetInputLayout(renderer_d3_dc,t_vl);
	/******************/
	unsigned int off=0;
	unsigned int st=5*sizeof(float);
	float bf[]={0,0,0,0};
	ID3D11DeviceContext_OMSetBlendState(renderer_d3_dc,renderer_d3_bse,bf,0xffffffff);
	ID3D11DeviceContext_PSSetSamplers(renderer_d3_dc,0,1,&ss);
	ID3D11DeviceContext_PSSetShaderResources(renderer_d3_dc,0,1,&tx_sr);
	ID3D11DeviceContext_IASetVertexBuffers(renderer_d3_dc,0,1,&tx_vb,&st,&off);
	ID3D11DeviceContext_IASetIndexBuffer(renderer_d3_dc,tx_ib,DXGI_FORMAT_R16_UINT,0);
	ID3D11DeviceContext_IASetPrimitiveTopology(renderer_d3_dc,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ID3D11DeviceContext_DrawIndexed(renderer_d3_dc,(uint32_t)tx_ib_l,0,0);
	/******************/
	draw_ui();
	free(cm);
}
