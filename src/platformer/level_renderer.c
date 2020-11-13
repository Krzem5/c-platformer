#include <level_renderer.h>
#include <engine.h>
#include <resource_manager.h>
#include <renderer.h>
#include <stdlib.h>
#include <stdio.h>



struct CBufferLayout{
	RawMatrix wm;
};



ID3D11Buffer* l_cb=NULL;



LevelLayout load_level(const char* nm){
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
	LevelLayout o=malloc(sizeof(struct _LEVEL_LAYOUT));
	o->nml=fgetc(f)&0xff;
	assert(o->nml<=20);
	o->nm=malloc((o->nml+1)*sizeof(char));
	for (size_t i=0;i<o->nml;i++){
		*(o->nm+i)=(fgetc(f)&0xff);
	}
	*(o->nm+o->nml)=0;
	o->sx=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8);
	assert(o->sx<LEVEL_MAX_WIDTH);
	o->sy=(fgetc(f)&0xff);
	assert(o->sy<LEVEL_MAX_HEIGHT);
	o->sz=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8);
	assert(o->sz<LEVEL_MAX_WIDTH);
	o->bl=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8)|((fgetc(f)&0xff)<<16)|((fgetc(f)&0xff)<<24);
	o->b=malloc(o->bl*sizeof(struct _LEVEL_BLOCK));
	printf("NAME: '%s' %hu %hhu %hu %lu\n",o->nm,o->sx,o->sy,o->sz,o->bl);
	for (uint32_t i=0;i<o->bl;i++){
		(o->b+i)->t=(fgetc(f)&0xff);
		(o->b+i)->x=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8);
		assert((o->b+i)->x<LEVEL_MAX_WIDTH);
		(o->b+i)->y=(fgetc(f)&0xff);
		assert((o->b+i)->y<LEVEL_MAX_HEIGHT);
		(o->b+i)->z=(fgetc(f)&0xff)|((fgetc(f)&0xff)<<8);
		assert((o->b+i)->z<LEVEL_MAX_WIDTH);
		printf("Block: %hhu %hu %hhu %hu\n",(o->b+i)->t,(o->b+i)->x,(o->b+i)->y,(o->b+i)->z);
	}
	fclose(f);
	return o;
}



Level create_level(LevelLayout ll){
	assert(ll!=NULL);
	Level o=malloc(sizeof(struct _LEVEL));
	o->ll=ll;
	o->p=malloc(sizeof(struct _PLAYER));
	o->p->x=ll->sx;
	o->p->y=ll->sy;
	o->p->z=ll->sz;
	o->bl=ll->bl;
	o->b=malloc(ll->bl*sizeof(struct _BLOCK_MODEL));
	for (uint32_t i=0;i<o->bl;i++){
		(o->b+i)->a=resource_manager_load_asset((ll->b+i)->t);
		(o->b+i)->x=(ll->b+i)->x;
		(o->b+i)->y=(ll->b+i)->y;
		(o->b+i)->z=(ll->b+i)->z;
	}
	return o;
}



void update_level(Level l,double dt){
	assert(l!=NULL);
}



void draw_level(Level l){
	assert(l!=NULL);
	for (uint32_t i=0;i<l->bl;i++){
		resource_manager_draw_asset((l->b+i)->a,raw_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1));
	}
}
