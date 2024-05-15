#include "glwindow.h"

GLWindow::GLWindow() {
	//constructor
}

void GLWindow::create(const char* title, int width, int height) {
	// gl version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// data and buffer sizes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
 	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// antialiasing
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL_INIT_VIDEO Failed: " << SDL_GetError() << '\n';
		exit(1);
	}

	w_width = width;
	w_height = height;
	Settings::Instance()->winWidth = &w_width;
	Settings::Instance()->winHeight = &w_height;

	SDL_WindowFlags windowFlags;
	settingsfs = Settings::Instance()->getCVarPtr("fullscreen");
	fs = *settingsfs;
	if ( fs == 1 )
		windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN);
	else
		windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_width, w_height, windowFlags);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1); //0) vsync off 1) vsync -1)adaptive vsync

	//Window icon
	//SDL_Surface *icon = SDL_IM("../share/critterding/pixmaps/cd.bmp");
	//SDL_SetWindowIcon(window, icon);
	//SDL_FreeSurface(icon);

	//Solve many problems associated with opengl's many modules and version
	GLenum err = glewInit();
	cout << '\n';
	cout << "====== OpenGL ======" << '\n';
    cout << "Vendor: " << glGetString(GL_VENDOR) << '\n';
    cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
    cout << "Version: " << glGetString(GL_VERSION) << '\n';
    cout << "GLEW: " << ((GLEW_OK ==  err)?glewGetString(GLEW_VERSION):glewGetErrorString(err)) << '\n';
}


void GLWindow::resize() {
	if ( w_height == 0 ) w_height = 1;
	if ( w_width == 0 ) w_width = 1;

	SDL_SetWindowSize(window, w_width, w_height);
}


void GLWindow::toggleFs() {
	SDL_WindowFlags flags;

	if ( fs ) {
		/*
		flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN);
		if ( w_height == 0 ) w_height = 1;
		if ( w_width == 0 ) w_width = 1;

		SDL_FreeSurface(surface);
		n_width = w_width;
		n_height = w_height;
		w_width = Settings::Instance()->getCVar("fsX");
		w_height = Settings::Instance()->getCVar("fsY");
		*/
		//surface = SDL_SetVideoMode( w_width, w_heig_
		SDL_SetWindowFullscreen(window, (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI));
		//SDL_SetWindowDisplayMode(window, SDL_WINDOW_RESIZABLE);
		Displaylists::Instance()->generateList();
		Textprinter::Instance()->setUpFonts();
	}
	else
	{
		/*
		flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
		SDL_FreeSurface(surface);
		w_width = n_width;
		w_height = n_height;
		*/
		//surface = SDL_SetVideoMode( w_width, w_height, w_bpp, flags);
		SDL_SetWindowSize(window, w_width, w_height);
		Displaylists::Instance()->generateList();
		Textprinter::Instance()->setUpFonts();
	}
}

void GLWindow::runGLScene(GLScene* glscene)
{
	bool stop = false;

	while(!stop)
	{
		if ( fs != *settingsfs )
		{
			fs = *settingsfs;
			toggleFs();
		}

		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
				w_width = event.window.data1;
				w_height = event.window.data2;
				resize();
			}

			if(event.type == SDL_QUIT)
				stop = true;

			else if(event.type == SDL_KEYDOWN)
				glscene->handlekeyPressed( event.key.keysym.sym );

			else if(event.type == SDL_KEYUP)
				glscene->handlekeyReleased( event.key.keysym.sym );

			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mousex, &mousey);
				glscene->handlemousebuttonPressed( mousex, mousey, event.button.button );
			}

			else if (event.type == SDL_MOUSEBUTTONUP) {
				SDL_GetMouseState(&mousex, &mousey);
				glscene->handlemousebuttonReleased( mousex, mousey, event.button.button );
			}

			else if(event.type == SDL_MOUSEMOTION) {
				glscene->handleMouseMotionRel( event.motion.xrel, event.motion.yrel );
				glscene->handleMouseMotionAbs( event.motion.x, event.motion.y );
			}
		}

		glscene->draw();
		SDL_GL_SwapWindow(window);
	}

	SDL_Quit();
	exit(0);
}


GLWindow::~GLWindow()
{

}


