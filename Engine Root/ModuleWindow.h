#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void SetFullScreen(bool full_screen);
	void SetResizability(bool resizable);
	void SetBorderless(bool borderless);
	void SetFullDesktop(bool desktop);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	int width;
	int height;
	int previous_width;
	int previous_height;

	bool width_resize = false;
	bool height_resize = false;

	bool fullscreen = false;
	bool resizable = true;
	bool borderless = true;
	bool desktop = false;



};

#endif // __ModuleWindow_H__