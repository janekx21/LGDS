#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>
#include <SOIL/SOIL.h>

#define NO_STDIO_REDIRECT
#define GL_CLAMP_TO_EDGE 0x812F

void glError(char errorName[]){
	int err = glGetError();
	while(err!=0){
		SDL_Log("glError:%x,%s\n",err,errorName);
		err = glGetError();
	}
	return;
}

SDL_Window* win;

GLuint mainTexture = 0;

void drawSprite(int index){

	int tilesSize = 16;

	float size = 1.0/(float)tilesSize;
	float xoff = (float)(index%tilesSize)*size;
	float yoff = (1.0 - size) - (float)(index/tilesSize)*size;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mainTexture);

	glPushMatrix(); //PUSH
	glScalef(.5,.5,.5);
	glBegin(GL_QUADS);

	glTexCoord2f(xoff + size,yoff);
	glVertex2f(0.5,-0.5);

	glTexCoord2f(xoff + size,yoff + size);
	glVertex2f(0.5,0.5);
	

	glTexCoord2f(xoff,yoff + size);
	glVertex2f(-0.5,0.5);
	
	glTexCoord2f(xoff,yoff);
	glVertex2f(-.5,-.5);
	

	glEnd();
	glPopMatrix(); //POP
	glDisable(GL_TEXTURE_2D);
}


void loop(){
	for(;;){
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT)return;
		}
		glColor3f(0.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glColor3f(1.0, 1.0, 1.0);

		glRotatef(1,0,0,1);
		drawSprite(10);


		SDL_GL_SwapWindow(win);
	}
}

void loadMainTextures(){
	mainTexture = SOIL_load_OGL_texture(
		"src/assets/pics/blaskw.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	if( 0 == mainTexture ){
		SDL_Log( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mainTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glDisable(GL_TEXTURE_2D);

}

int main(int argc, char *argv[]){
//	SDL_Log("Hello, World!\n");
	//Hier geht mein programmm los

//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); //Set to opengl 4.0
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	printf("%d\n",SDL_Init(SDL_INIT_EVERYTHING));
	win = SDL_CreateWindow("OpenGL",64,64,640,640,SDL_WINDOW_OPENGL);//|SDL_WINDOW_RESIZABLE);
	if(win == NULL){
		SDL_Log("Coult no create Window and %s\n",SDL_GetError());
	}

	SDL_GLContext* glcontext = SDL_GL_CreateContext(win);

	if(glcontext == NULL){
		SDL_Log("Coult no create OpenGl Context and %s\n",SDL_GetError());
	}

	loadMainTextures();

	
	loop();


	SDL_GL_DeleteContext(glcontext);
	
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
