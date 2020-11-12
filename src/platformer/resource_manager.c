#include <resource_manager.h>
#include <engine.h>
#include <renderer.h>



struct _ASSET_LIST_NODE{
	struct _ASSET_LIST_NODE* n;
	uint8_t id;
	AssetID dt;
} _head={
	NULL,
	UINT8_MAX
	NULL
}



AssetID resource_manager_load_asset(uint8_t id){
	struct _ASSET_LIST_NODE n=&_head;
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
	n->dt->l=
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
	unsigned int st=8*sizeof(float);
	ID3D11DeviceContext_IASetVertexBuffers(renderer_d3_dc,0,1,&g_vb,&st,&off);
	ID3D11DeviceContext_IASetIndexBuffer(renderer_d3_dc,g_ib,DXGI_FORMAT_R16_UINT,0);
	ID3D11DeviceContext_IASetPrimitiveTopology(renderer_d3_dc,D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	ID3D11DeviceContext_DrawIndexed(renderer_d3_dc,(uint32_t)g_ib_l,0,0);
}
