#include <common.h>
#include <player.h>
#include <renderer.h>
#include <level.h>
#include <ds4.h>
#include <stdio.h>



bool _intersect_aabb(float a[],float b[]){
	return (a[0]+a[2]/2>b[0]-b[2]/2&&a[0]-a[2]/2<b[0]+b[2]/2&&a[1]+a[3]/2>b[1]-b[3]/2&&a[1]-a[3]/2<b[1]+b[3]/2);
}



void _separate_wall_aabb(Player p,float a[]){
	if (a[0]-a[2]/2<0){
		p->vx=0;
		p->x=a[2]/2;
	}
	if (a[1]+a[3]/2<0){
		p->d=1;
	}
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



Player create_player(Level l,uint16_t x,uint8_t y){
	Player o=malloc(sizeof(struct _PLAYER));
	o->l=l;
	o->x=x+0.5f;
	o->y=y+0.5f;
	o->vx=0;
	o->vy=0;
	o->d=0;
	uint16_t* pil=malloc(6*sizeof(uint16_t));
	*pil=0;
	*(pil+1)=1;
	*(pil+2)=2;
	*(pil+3)=0;
	*(pil+4)=2;
	*(pil+5)=3;
	o->as=TILEMAP_TEX_IMG_PLAYER_STAND_FRONT;
	o->as_f=false;
	o->as_tm=-1;
	o->vl=malloc(16*sizeof(float));
	*o->vl=(o->x-0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(o->vl+1)=(l->sy-o->y-0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(o->vl+2)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,o->as);
	*(o->vl+3)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,o->as);
	*(o->vl+4)=(o->x+0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(o->vl+5)=(l->sy-o->y-0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(o->vl+6)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,o->as);
	*(o->vl+7)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,o->as);
	*(o->vl+8)=(o->x+0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(o->vl+9)=(l->sy-o->y+0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(o->vl+10)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,o->as);
	*(o->vl+11)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,o->as);
	*(o->vl+12)=(o->x-0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(o->vl+13)=(l->sy-o->y+0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(o->vl+14)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,o->as);
	*(o->vl+15)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,o->as);
	o->on_g=false;
	DS4DeviceList dd_l=DS4_find_all();
	assert(dd_l!=NULL);
	o->dd=DS4_connect(dd_l->p);
	DS4_free_list(dd_l);
	o->dd->r=50;
	o->dd->g=210;
	o->dd->b=140;
	o->_sx=x+0.5f;
	o->_sy=y+0.5f;
	o->_u=false;
	o->_dft=0;
	o->_dtm=0;
	o->_dftm=0;
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
	HRESULT hr=ID3D11Device_CreateBuffer(renderer_d3_d,&bd,&dt,&(o->_ib));
	assert(hr==S_OK);
	free(pil);
	bd.ByteWidth=(uint32_t)(16*sizeof(float));
	bd.Usage=D3D11_USAGE_DYNAMIC;
	bd.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	dt.pSysMem=o->vl;
	hr=ID3D11Device_CreateBuffer(renderer_d3_d,&bd,&dt,&(o->_vb));
	return o;
}



void update_player(Player p,float dt){
	assert(p!=NULL);
	if (p->d==2){
		p->x=p->_sx;
		p->y=p->_sy;
		p->vx=0;
		p->vy=0;
		p->dd->r=50;
		p->dd->g=210;
		p->dd->b=140;
		p->_u=false;
		p->_dft=0;
		p->_dtm=0;
		p->_dftm=0;
		p->d=0;
	}
	DS4_update_input(p->dd);
	if (p->d==1){
		if (p->_dtm==0){
			p->dd->sr=PLAYER_DEATH_RUMBLE_S;
		}
		else if (p->_dtm>=PLAYER_DEATH_ANIM_END){
			p->d=2;
			p->dd->r=0;
			p->dd->g=0;
			p->dd->b=0;
			p->dd->fr=0;
			p->dd->sr=0;
		}
		p->_dtm+=dt;
		p->_dftm+=dt;
		if (p->_dft==0||p->_dftm>=PLAYER_DEATH_ANIM_FLASH){
			if (p->_dft%2==0){
				p->dd->r=0;
				p->dd->g=0;
				p->dd->b=0;
				p->dd->fr=0;
			}
			else{
				p->dd->r=PLAYER_DEATH_ANIM_R;
				p->dd->g=PLAYER_DEATH_ANIM_G;
				p->dd->b=PLAYER_DEATH_ANIM_B;
				p->dd->fr=PLAYER_DEATH_RUMBLE_F;
			}
			p->_dftm=0;
			p->_dft++;
		}
	}
	else{
		bool n_as=false;
		if (p->dd->lx<=-PLAYER_JOYSTICK_X_BUFFOR||p->dd->lx>=PLAYER_JOYSTICK_X_BUFFOR){
			p->vx+=(p->dd->lx>0?1:-1)*(p->on_g==true?PLAYER_WALK_SPEED:PLAYER_AIR_WALK_SPEED)*((p->dd->btn&DS4_BUTTON_SQUARE)!=0?PLAYER_DASH_SPEED_MLT:1);
			if (p->as_tm==-1){
				p->as=TILEMAP_TEX_IMG_PLAYER_WALK_1;
				p->as_tm=0;
			}
			else{
				float tm=((p->dd->btn&DS4_BUTTON_SQUARE)!=0?PLAYER_DASH_FRAME_TIME:PLAYER_WALK_FRAME_TIME);
				if (p->as_tm>=tm){
					while (p->as_tm>=tm){
						p->as_tm-=tm;
					}
					if (p->as==TILEMAP_TEX_IMG_PLAYER_WALK_1){
						p->as=TILEMAP_TEX_IMG_PLAYER_WALK_2;
					}
					else{
						p->as=TILEMAP_TEX_IMG_PLAYER_WALK_1;
					}
				}
			}
			p->as_tm+=dt;
			p->as_f=(p->dd->lx>0?false:true);
			n_as=true;
		}
		else{
			p->as=TILEMAP_TEX_IMG_PLAYER_STAND;
			p->as_tm=-1;
		}
		if (p->on_g==true&&p->dd->ly<=-PLAYER_JOYSTICK_Y_BUFFOR){
			p->vx=0;
			p->as=TILEMAP_TEX_IMG_PLAYER_DUCK;
			n_as=true;
		}
		else if (p->on_g==true&&(p->dd->btn&DS4_BUTTON_CROSS)!=0){
			p->vy+=PLAYER_JUMP;
			p->on_g=false;
		}
		if (n_as==false){
			p->as=TILEMAP_TEX_IMG_PLAYER_STAND;
		}
		p->vy-=PLAYER_GRAVITY;
		if (p->vx<-PLAYER_MAX_X_SPEED||p->vx>PLAYER_MAX_X_SPEED){
			p->vx=(p->vx<0?-1:1)*PLAYER_MAX_X_SPEED;
		}
		if (p->vy<-PLAYER_MAX_Y_SPEED||p->vy>PLAYER_MAX_Y_SPEED){
			p->vy=(p->vy<0?-1:1)*PLAYER_MAX_Y_SPEED;
		}
		p->x+=p->vx*dt;
		p->y+=p->vy*dt;
		float aabb[]={p->x,p->y,PLAYER_WIDTH,1.0f};
		_separate_wall_aabb(p,aabb);
		aabb[0]=p->x;
		aabb[1]=p->y;
		p->on_g=false;
		for (uint32_t i=0;i<p->l->c_dt->b_dtl;i++){
			if (*(p->l->c_dt->b_dt+i)==0){
				continue;
			}
			float b_aabb[4];
			if (*(p->l->c_dt->b_dt+i)==1){
				float x;
				float y;
				COLLISION_DATA_XY_FROM_INDEX(p->l->c_dt,i,x,y);
				*b_aabb=x+0.5f;
				*(b_aabb+1)=y+0.5f;
				*(b_aabb+2)=1.0f;
				*(b_aabb+3)=1.0f;
			}
			if (_intersect_aabb(aabb,b_aabb)==true){
				bool g=_separate_aabb(p,aabb,b_aabb);
				if (g==true){
					p->on_g=true;
				}
				aabb[0]=p->x;
				aabb[1]=p->y;
			}
		}
		if (p->on_g==false){
			p->as=TILEMAP_TEX_IMG_PLAYER_JUMP;
			p->vx*=PLAYER_AIR_FRICTION;
		}
		else{
			p->vx=0;
		}
	}
	DS4_update_output(p->dd);
	*p->vl=(p->x-0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(p->vl+1)=(p->l->sy-p->y-0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(p->vl+4)=(p->x+0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(p->vl+5)=(p->l->sy-p->y-0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(p->vl+8)=(p->x+0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(p->vl+9)=(p->l->sy-p->y+0.5f)*SIZE_SCALE+HEIGHT_PAD;
	*(p->vl+12)=(p->x-0.5f)*SIZE_SCALE+WIDTH_PAD;
	*(p->vl+13)=(p->l->sy-p->y+0.5f)*SIZE_SCALE+HEIGHT_PAD;
	if (p->as_f==false){
		*(p->vl+2)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+3)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+6)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+7)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+10)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+11)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+14)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+15)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,p->as);
	}
	else{
		*(p->vl+2)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+3)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+6)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+7)=TEXTURE_Y0(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+10)=TEXTURE_X0(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+11)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+14)=TEXTURE_X1(TILEMAP_TEX_MAP_DATA,p->as);
		*(p->vl+15)=TEXTURE_Y1(TILEMAP_TEX_MAP_DATA,p->as);
	}
	ID3D11Resource* vbr;
	ID3D11Buffer_QueryInterface(p->_vb,&IID_ID3D11Resource,(void**)&vbr);
	D3D11_MAPPED_SUBRESOURCE mr;
	HRESULT hr=ID3D11DeviceContext_Map(renderer_d3_dc,vbr,0,D3D11_MAP_WRITE_DISCARD,0,&mr);
	float* nvl=(float*)(mr.pData);
	for (size_t i=0;i<16;i++){
		*(nvl+i)=*(p->vl+i);
	}
	ID3D11DeviceContext_Unmap(renderer_d3_dc,vbr,0);
	ID3D11Resource_Release(vbr);
}



void draw_player(Player p){
	assert(p!=NULL);
	unsigned int off=0;
	unsigned int st=4*sizeof(float);
	ID3D11DeviceContext_IASetPrimitiveTopology(renderer_d3_dc,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ID3D11DeviceContext_IASetVertexBuffers(renderer_d3_dc,0,1,&(p->_vb),&st,&off);
	ID3D11DeviceContext_IASetIndexBuffer(renderer_d3_dc,p->_ib,DXGI_FORMAT_R16_UINT,0);
	ID3D11DeviceContext_DrawIndexed(renderer_d3_dc,6,0,0);
}
