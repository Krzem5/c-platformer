#include <common.h>
#include <engine.h>
#include <renderer.h>
#include <level.h>
// #include <color_2d_pixel.h>
// #include <color_2d_vertex.h>
#include <stdio.h>



struct CBufferLayout{
	RawMatrix pm;
};



struct CBufferExtraLayout{
	RawMatrix wm;
};



// ID3D11VertexShader* cl_vs=NULL;
// ID3D11PixelShader* cl_ps=NULL;
// ID3D11InputLayout* cl_vl=NULL;
ID3D11Buffer* cb=NULL;
Level l=NULL;



void init_engine(void){
	// D3D11_INPUT_ELEMENT_DESC cl_il[]={
	// 	{
	// 		"POSITION",
	// 		0,
	// 		DXGI_FORMAT_R32G32B32A32_FLOAT,
	// 		0,
	// 		0,
	// 		D3D11_INPUT_PER_VERTEX_DATA
	// 	},
	// 	{
	// 		"COLOR",
	// 		0,
	// 		DXGI_FORMAT_R32G32B32A32_FLOAT,
	// 		0,
	// 		D3D11_APPEND_ALIGNED_ELEMENT,
	// 		D3D11_INPUT_PER_VERTEX_DATA
	// 	}
	// };
	// cl_vs=load_vertex_shader(g_color_2d_vs,sizeof(g_color_2d_vs),cl_il,sizeof(cl_il)/sizeof(D3D11_INPUT_ELEMENT_DESC),&cl_vl);
	// cl_ps=load_pixel_shader(g_color_2d_ps,sizeof(g_color_2d_ps));
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
	update_level(l,(float)(dt*1e-6));
	if (renderer_w==NULL){
		return;
	}
	draw_level(l);
}
