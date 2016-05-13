#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ClassDemoApp.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

int main(int argc, char *argv[])
{
	ClassDemoApp game = ClassDemoApp();
	while (!game.done) { 
		game.UpdateAndRender();
	}
	return 0;
}
