#include <renderer.h>
#include <engine.h>
#include <ui.h>
#include <stdio.h>



#define C_NONE C_APPSTARTING
#define C_APPSTARTING 32650
#define C_ARROW 32512
#define C_CROSS 32515
#define C_HAND 32649
#define C_HELP 32651
#define C_IBEAM 32513
#define C_ICON 32641
#define C_NO 32648
#define C_SIZE 32640
#define C_SIZEALL 32646
#define C_SIZENESW 32643
#define C_SIZENS 32645
#define C_SIZENWSE 32642
#define C_SIZEWE 32644
#define C_UPARROW 32516
#define C_WAIT 32514



uint16_t _lc=C_NONE;
uint16_t nc=C_NONE;



void update_ui(double dt){
	(void)dt;
}



void draw_ui(void){
	if (nc!=_lc){
		_lc=nc;
		SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(nc)));
	}
}
