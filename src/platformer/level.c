#include <engine.h>
#include <renderer.h>
#include <level.h>
#include <ds4.h>
#include <stdio.h>



#define TOTAL_X_BLOCKS 20
#define TOTAL_Y_BLOCKS 15
#define SIZE_SCALE (((float)renderer_ww/TOTAL_X_BLOCKS)<((float)renderer_wh/TOTAL_Y_BLOCKS)?(float)renderer_ww/TOTAL_X_BLOCKS:(float)renderer_wh/TOTAL_Y_BLOCKS)
#define WIDTH_PAD ((renderer_ww-SIZE_SCALE*TOTAL_X_BLOCKS)/2)
#define HEIGHT_PAD ((renderer_wh-SIZE_SCALE*TOTAL_Y_BLOCKS)/2)
#define COLLISION_DATA_INDEX(c,x,y) (*(c->b_dt+y*c->_w+x))
#define COLLISION_DATA_XY_FROM_INDEX(c,i,x,y) \
	do { \
		x=(float)(i%((c)->_w)); \
		y=(float)(i/((c)->_w)); \
	} while(0)
#define PLAYER_WIDTH 0.8f
#define WALK_SPEED 5
#define AIR_WALK_SPEED 0.1f
#define JUMP (1.5*(1/GRAVITY))
#define GRAVITY 0.1f
#define FRICTION 0.5f
#define AIR_FRICTION 0.98f
#define MAX_SPEED 30



bool _intersect_aabb(float a[],float b[]){
	return (a[0]+a[2]/2>b[0]-b[2]/2&&a[0]-a[2]/2<b[0]+b[2]/2&&a[1]+a[3]/2>b[1]-b[3]/2&&a[1]-a[3]/2<b[1]+b[3]/2);
}



bool _separate_aabb(Player p,float a[],float b[]){
	float mo=0;
	float ox=0;
	float oy=0;
	float o;
	if (a[0]<b[0]){
		o=(a[0]+a[2]/2)-(b[0]-b[2]/2);
	}
	else{
		o=(a[0]-a[2]/2)-(b[0]+b[2]/2);
	}
	if (mo==0||(o<0?-o:o)<mo){
		mo=(o<0?-o:o);
		ox=o;
		oy=0;
	}
	if (a[1]<b[1]){
		o=(a[1]+a[3]/2)-(b[1]-b[3]/2);
	}
	else{
		o=(a[1]-a[3]/2)-(b[1]+b[3]/2);
	}
	if (mo==0||(o<0?-o:o)<mo){
		mo=(o<0?-o:o);
		ox=0;
		oy=o;
	}
	p->x-=ox;
	p->y-=oy;
	if (ox!=0){
		p->vx=0;
	}
	if (oy!=0){
		p->vy=0;
		if (a[1]>b[1]){
			return true;
		}
	}
	return false;
}



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
	o->p=malloc(sizeof(struct _PLAYER));
	o->p->x=1.5f;
	o->p->y=(float)(fgetc(f)&0xff)-0.5f;
	o->p->vx=0;
	o->p->vy=0;
	uint16_t* pil=malloc(6*sizeof(uint16_t));
	*pil=0;
	*(pil+1)=1;
	*(pil+2)=2;
	*(pil+3)=0;
	*(pil+4)=2;
	*(pil+5)=3;
	o->p->as=TILEMAP_TEX_IMG_PLAYER_STAND_FRONT;
	o->p->as_f=false;
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
	o->p->on_g=false;
	DS4DeviceList dd_l=DS4_find_all();
	assert(dd_l!=NULL);
	o->p->dd=DS4_connect(dd_l->p);
	DS4_free_list(dd_l);
	o->p->dd->r=50;
	o->p->dd->g=210;
	o->p->dd->b=140;
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
	#define JOYSTICK_TURN_BUFFOR 32
	assert(l!=NULL);
	DS4_update(l->p->dd);
	if (l->p->dd->lx<=-JOYSTICK_TURN_BUFFOR||l->p->dd->lx>=JOYSTICK_TURN_BUFFOR){
		l->p->vx+=(l->p->dd->lx>0?1:-1)*(l->p->on_g==true?WALK_SPEED:AIR_WALK_SPEED);
		l->p->as=TILEMAP_TEX_IMG_PLAYER_STAND;
		l->p->as_f=(l->p->dd->lx>0?false:true);
	}
	if (l->p->on_g==true&&(l->p->dd->btn&DS4_BUTTON_CROSS)!=0){
		l->p->vy+=JUMP;
		l->p->as=TILEMAP_TEX_IMG_PLAYER_JUMP;
		l->p->on_g=false;
	}
	else if (l->p->on_g==true){
		l->p->as=TILEMAP_TEX_IMG_PLAYER_STAND;
	}
	l->p->vx*=(l->p->on_g==true?FRICTION:AIR_FRICTION);
	l->p->vy-=GRAVITY;
	float m=l->p->vx*l->p->vx+l->p->vy*l->p->vy;
	if (m>MAX_SPEED*MAX_SPEED){
		m=MAX_SPEED/sqrtf(m);
		l->p->vx*=m;
		l->p->vy*=m;
	}
	l->p->x+=l->p->vx*dt;
	l->p->y+=l->p->vy*dt;
	float aabb[]={l->p->x,l->p->y,PLAYER_WIDTH,1.0f};
	l->p->on_g=false;
	for (uint32_t i=0;i<l->c_dt->b_dtl;i++){
		if (*(l->c_dt->b_dt+i)==0){
			continue;
		}
		float b_aabb[4];
		if (*(l->c_dt->b_dt+i)==1){
			float x;
			float y;
			COLLISION_DATA_XY_FROM_INDEX(l->c_dt,i,x,y);
			*b_aabb=x+0.5f;
			*(b_aabb+1)=y+0.5f;
			*(b_aabb+2)=1.0f;
			*(b_aabb+3)=1.0f;
		}
		if (_intersect_aabb(aabb,b_aabb)==true){
			bool g=_separate_aabb(l->p,aabb,b_aabb);
			if (g==true){
				l->p->on_g=true;
			}
			aabb[0]=l->p->x;
			aabb[1]=l->p->y;
		}
	}
	*l->p->vl=(l->p->x-0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(l->p->vl+1)=(l->sy-l->p->y-0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(l->p->vl+4)=(l->p->x+0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(l->p->vl+5)=(l->sy-l->p->y-0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(l->p->vl+8)=(l->p->x+0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(l->p->vl+9)=(l->sy-l->p->y+0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(l->p->vl+12)=(l->p->x-0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(l->p->vl+13)=(l->sy-l->p->y+0.5f)*SIZE_SCALE+HEIGHT_PAD;
	if (l->p->as_f==false){
		*(l->p->vl+2)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+3)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+6)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+7)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+10)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+11)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+14)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+15)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,l->p->as);
	}
	else{
		*(l->p->vl+2)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+3)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+6)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+7)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+10)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+11)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+14)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,l->p->as);
		*(l->p->vl+15)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,l->p->as);
	}
	ID3D11Resource* vbr;
	ID3D11Buffer_QueryInterface(l->p->_vb,&IID_ID3D11Resource,(void**)&vbr);
	D3D11_MAPPED_SUBRESOURCE mr;
	HRESULT hr=ID3D11DeviceContext_Map(renderer_d3_dc,vbr,0,D3D11_MAP_WRITE_DISCARD,0,&mr);
	float* nvl=(float*)(mr.pData);
	for (size_t i=0;i<16;i++){
		*(nvl+i)=*(l->p->vl+i);
	}
	ID3D11DeviceContext_Unmap(renderer_d3_dc,vbr,0);
	ID3D11Resource_Release(vbr);
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
