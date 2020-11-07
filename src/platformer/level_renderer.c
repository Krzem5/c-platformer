#include <level_renderer.h>
#include <engine.h>



LevelLayout load_level(const char* nm);



Level create_level(LevelLayout ll);



void update_level(Level l,double dt);



void draw_level(Level l);
