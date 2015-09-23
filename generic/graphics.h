//
// Created by hakeee on 2015-09-06.
//

#ifndef DISCOBREAK_GRAPHICS_H
#define DISCOBREAK_GRAPHICS_H

#pragma warning(push, 0)
#include "buffer.h"
//#include "shader.h"
#include "texture.h"
#pragma warning(pop)

namespace discobreak {
    class Shader;
    enum GRAPHICS {
        DIRECTX,
        OPENGL
    };
#ifdef _WIN32
    class Wggraphics;
    class Wdgraphics;

#else

    class LGgraphics;

#endif

    class Graphics {
        GRAPHICS type_;
#ifdef _WIN32
        Wggraphics* gg_;
        Wdgraphics* dg_;
        Graphics(Wggraphics* gg): type_(GRAPHICS::OPENGL), gg_(gg), dg_(nullptr) {};
        Graphics(Wdgraphics* dg): type_(GRAPHICS::DIRECTX), gg_(nullptr), dg_(dg) {};

#else
        LGgraphics *gg_;

        Graphics(LGgraphics *gg): type_(GRAPHICS::OPENGL), gg_(gg) { };

#endif

    public:
        ~Graphics();

        // swaps frontbuffer with backbuffer
        void Swap();

        // clear the screen with rgba format
        void Clear(float r, float g, float b, float a);

        // Gives you a new buffer object, dont forget to delete it when you do not want it anymore
        Buffer* NewBuffer();

        // Gives you a new shader object, dont forget to delete it when you do not want it anymore
        Shader* NewShader();

		// Gives you a new texture object, dont forget to delete it when you do not want it anymore
		Texture* NewTexture();

		//Returns enum of which graphics api we're using
		GRAPHICS GetGraphicsType() { return type_; };

        // prints the version of the graphics context
        void PrintVersion();

        friend class Wwindow;
        friend class Lwindow;
        friend class LGgraphics;
    };
};

#endif //DISCOBREAKLINUX_GRAPHICS_H
