#include <resource_manager.h>
#include <engine.h>
#include <renderer.h>
#include <stdio.h>



struct CBufferLayout{
	RawMatrix wm;
};



struct _ASSET_LIST_NODE{
	struct _ASSET_LIST_NODE* n;
	uint8_t id;
	AssetID dt;
} _head={
	NULL,
	UINT8_MAX,
	NULL
};
ID3D11Buffer* a_cb=NULL;



AssetID resource_manager_load_asset(uint8_t id){
	struct _ASSET_LIST_NODE* n=&_head;
	while (n->n!=NULL){
		if (n->id==id){
			return n->dt;
		}
		n=n->n;
	}
	if (n->id!=UINT8_MAX){
		n->n=malloc(sizeof(struct _ASSET_LIST_NODE));
		n=n->n;
	}
	n->id=id;
	n->dt=malloc(sizeof(struct _BLOCK_MODEL));
	size_t ln=strlen(ASSET_BASE_FILE_PATH);
	char* fp=malloc((ln+7)*sizeof(char));
	for (size_t i=0;i<ln;i++){
		*(fp+i)=*(ASSET_BASE_FILE_PATH+i);
	}
	*(fp+ln)=(id>>4)+(id>=160?39:0)+48;
	*(fp+ln+1)=(id&0x0f)+((id&0x0f)>=10?39:0)+48;
	*(fp+ln+2)='.';
	*(fp+ln+3)='m';
	*(fp+ln+4)='d';
	*(fp+ln+5)='l';
	*(fp+ln+6)=0;
	FILE* f=NULL;
	errno_t e=fopen_s(&f,"a","rb");
	assert(e==0);
	n->dt->l=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8)|((fgetc(f)&0xff)<<16)|((fgetc(f)&0xff)<<24);
	float* il=malloc(n->dt->l*3*sizeof(uint16_t));
	for (uint32_t i=0;i<n->dt->l;i++){
		*(il+(size_t)i*3)=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8);
		*(il+(size_t)i*3+1)=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8);
		*(il+(size_t)i*3+2)=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8);
	}
	free(il);
	fclose(f);
	return n->dt;
}



void resource_manager_draw_asset(AssetID a,RawMatrix wm){
	assert(a!=NULL);
	if (a_cb==NULL){
		a_cb=create_constant_buffer(sizeof(struct CBufferLayout));
	}
	struct CBufferLayout cb_dt={
		wm
	};
	update_constant_buffer(a_cb,(void*)&cb_dt);
	ID3D11DeviceContext_VSSetConstantBuffers(renderer_d3_dc,1,1,&a_cb);
	ID3D11DeviceContext_PSSetConstantBuffers(renderer_d3_dc,1,1,&a_cb);
	unsigned int off=0;
	unsigned int st=11*sizeof(float);
	ID3D11DeviceContext_IASetPrimitiveTopology(renderer_d3_dc,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ID3D11DeviceContext_IASetVertexBuffers(renderer_d3_dc,0,1,&(a->e->vb),&st,&off);
	ID3D11DeviceContext_IASetIndexBuffer(renderer_d3_dc,a->e->ib,DXGI_FORMAT_R16_UINT,0);
	ID3D11DeviceContext_DrawIndexed(renderer_d3_dc,a->e->ib_l,0,0);
}
