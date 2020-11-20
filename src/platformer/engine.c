#include <common.h>
#include <engine.h>
#include <renderer.h>
#include <level.h>
#include <stdio.h>



struct CBufferLayout{
	RawMatrix pm;
};



struct CBufferExtraLayout{
	RawMatrix wm;
};



ID3D11Buffer* cb=NULL;
Level l=NULL;



void init_engine(void){
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
