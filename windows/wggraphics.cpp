#include "wggraphics.h"
#include "../generic/shader.h"
#include "../generic/buffer.h"
#include <GL/glew.h>
#include "../generic/opengl/glshader.h"
#include "../generic/opengl/glbuffer.h"
#include "../generic/opengl/gltexture.h"

using namespace discobreak;




Wggraphics::Wggraphics(Wwindow* window) {
	windowWidth_ = window->windowWidth_;
	windowHeight_ = window->windowHeight_;
	win_ = window;

	GLuint		PixelFormat;
	static	PIXELFORMATDESCRIPTOR pfd =	{			// pfd Tells Windows How We Want Things To Be

		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(mhDC = GetDC(win_->hWnd_))) {							// Did We Get A Device Context?

		//KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		//return FALSE;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(mhDC, &pfd)))	{ // Did Windows Find A Matching Pixel Format?

		//KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		//return FALSE;								// Return FALSE
	}

	if (!SetPixelFormat(mhDC, PixelFormat, &pfd)) {		// Are We Able To Set The Pixel Format?

		//KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		//return FALSE;								// Return FALSE
	}

	if (!(mhRC = wglCreateContext(mhDC))) {				// Are We Able To Get A Rendering Context?

		//KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		//return FALSE;								// Return FALSE
	}

	if (!wglMakeCurrent(mhDC, mhRC)) {					// Try To Activate The Rendering Context

		//KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		//return FALSE;								// Return FALSE
	}

	GLenum err = glewInit();
	if (GLEW_OK != err) {

		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	RECT rc;
	GetClientRect(win_->hWnd_, &rc);
	glViewport(0, 0, rc.right - rc.left, rc.bottom - rc.top);						// Reset The Current Viewport

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 1.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	
}

Wggraphics::~Wggraphics() {
	if (mhRC = wglGetCurrentContext()) {

		// obtain its associated device context  
		mhDC = wglGetCurrentDC();

		// make the rendering context not current  
		wglMakeCurrent(NULL, NULL);

		// release the device context  
		ReleaseDC(win_->hWnd_, mhDC);

		// delete the rendering context  
		wglDeleteContext(mhRC);
	}
}


void Wggraphics::Swap() {
	SwapBuffers(mhDC);
}

void discobreak::Wggraphics::Clear(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

Buffer* Wggraphics::NewBuffer() {
    return new Buffer(new GLBuffer(this));
}

Shader* Wggraphics::NewShader() {
    return new Shader(new GLShader());
}

Texture* Wggraphics::NewTexture() {
	return new Texture(new GLTexture());
}

void Wggraphics::PrintVersion() {
    /* we can now get data for the specific OpenGL instance we created */
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("GL Vendor : %s\n", vendor);
    printf("GL Renderer : %s\n", renderer);
    printf("GL Version (string) : %s\n", version);
    printf("GL Version (integer) : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);
}