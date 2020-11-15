#include <common.h>
#include <engine.h>
#include <renderer.h>
#include <ui.h>
#include <level.h>
#include <color_2d_pixel.h>
#include <color_2d_vertex.h>
#include <texture_2d_vertex.h>
#include <texture_2d_pixel.h>
#include <tilemap.h>
#include <stdio.h>



struct CBufferLayout{
	RawMatrix pm;
};



struct CBufferExtraLayout{
	RawMatrix wm;
};



ID3D11VertexShader* cl_vs=NULL;
ID3D11PixelShader* cl_ps=NULL;
ID3D11InputLayout* cl_vl=NULL;
ID3D11VertexShader* tx_vs=NULL;
ID3D11PixelShader* tx_ps=NULL;
ID3D11InputLayout* tx_vl=NULL;
ID3D11Buffer* cb=NULL;
Level l=NULL;
/*******************/
ID3D11Buffer* tx_ib=NULL;
size_t tx_ib_l=0;
ID3D11Buffer* tx_vb=NULL;
ID3D11SamplerState* ss=NULL;
ID3D11ShaderResourceView* tx_sr=NULL;
/*******************/



void init_engine(void){
	D3D11_INPUT_ELEMENT_DESC cl_il[]={
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA
		},
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA
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
	cb=create_constant_buffer(sizeof(CBufferLayout));
	#define _near (0.1f)
	#define _far (1000.0f)
	CBufferLayout cb_dt={
		raw_matrix(2.0f/renderer_ww,0,0,0,0,-2.0f/renderer_wh,0,0,0,0,1/(_far-_near),_near/(_near-_far),-1,1,0,1)
	};
	update_constant_buffer(cb,(void*)&cb_dt);
	ID3D11DeviceContext_VSSetConstantBuffers(renderer_d3_dc,0,1,&cb);
	ID3D11DeviceContext_PSSetConstantBuffers(renderer_d3_dc,0,1,&cb);
	l=load_level("level1");
}



void update_engine(double dt){
	if (renderer_wf==false){
		return;
	}
	static float t=0;
	t+=(float)(dt*1e-6);
	ID3D11DeviceContext_OMSetDepthStencilState(renderer_d3_dc,renderer_d3_ddss,1);
	ID3D11DeviceContext_VSSetShader(renderer_d3_dc,tx_vs,NULL,0);
	ID3D11DeviceContext_PSSetShader(renderer_d3_dc,tx_ps,NULL,0);
	ID3D11DeviceContext_IASetInputLayout(renderer_d3_dc,tx_vl);
	float bf[]={0,0,0,0};
	ID3D11DeviceContext_OMSetBlendState(renderer_d3_dc,renderer_d3_bse,bf,0xffffffff);
	update_ui(dt);
	update_level(l,(float)(dt*1e-6));
	if (renderer_w==NULL){
		return;
	}
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
		uint32_t vlsz=4*4;
		uint16_t* il=malloc(6*sizeof(uint16_t));
		*il=0;
		*(il+1)=1;
		*(il+2)=2;
		*(il+3)=0;
		*(il+4)=2;
		*(il+5)=3;
		float* vl=malloc(4*4*sizeof(float));
		#define SIZE 1080
		*vl=0;
		*(vl+1)=0;
		*(vl+2)=0;
		*(vl+3)=0;
		*(vl+4)=SIZE;
		*(vl+5)=0;
		*(vl+6)=1;
		*(vl+7)=0;
		*(vl+8)=SIZE;
		*(vl+9)=SIZE;
		*(vl+10)=1;
		*(vl+11)=1;
		*(vl+12)=0;
		*(vl+13)=SIZE;
		*(vl+14)=0;
		*(vl+15)=1;
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
	ID3D11DeviceContext_PSSetSamplers(renderer_d3_dc,0,1,&ss);
	ID3D11DeviceContext_PSSetShaderResources(renderer_d3_dc,0,1,&tx_sr);
	draw_level(l);
	draw_ui();
}
