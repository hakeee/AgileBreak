//
// Created by hakeee on 2015-09-07.
//

#include "lggraphics.h"
#include "lwindow.h"
#include "../generic/opengl/glbuffer.h"
#include "../generic/opengl/glshader.h"
#include "../generic/opengl/gltexture.h"

using namespace discobreak;

LGgraphics::LGgraphics(Lwindow* win):win_(win) {
    glc_ = glXCreateContext(win_->display_->xdisplay_, win_->vi_, NULL, GL_TRUE);
};

void LGgraphics::Swap() {
	glXSwapBuffers(win_->display_->xdisplay_, win_->win_);
}

void LGgraphics::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

LGgraphics::~LGgraphics() {
    glXDestroyContext(win_->display_->xdisplay_, win_->GetGraphics()->gg_->glc_);
}

Buffer* LGgraphics::NewBuffer() {
    return new Buffer(new GLBuffer(this));
}

Shader *LGgraphics::NewShader() {
    return new Shader(new GLShader());
}

void LGgraphics::PrintVersion() {
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

Texture *LGgraphics::NewTexture() {
    return new Texture(new GLTexture());
}
