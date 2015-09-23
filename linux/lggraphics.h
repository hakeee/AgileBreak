//
// Created by hakeee on 2015-09-07.
//

#ifndef DISCOBREAKLINUX_GRAPHICS_H
#define DISCOBREAKLINUX_GRAPHICS_H

//#include "../generic/graphics.h"
#include <GL/glew.h>
#include <GL/glx.h>
#include "../generic/shader.h"
#include "../generic/buffer.h"

namespace discobreak {
    class Lwindow;
	class Graphics;
    class LGgraphics {

        GLXContext glc_;
        Lwindow* win_;

    private:
        LGgraphics(Lwindow*);

    public:
        ~LGgraphics();

		void Clear(float r, float g, float b, float a);
		void Swap();

        Buffer* NewBuffer();

        Shader* NewShader();

        Texture* NewTexture();

        void PrintVersion();

		friend class Graphics;
        friend class Lwindow;
    };
};

#endif //DISCOBREAKLINUX_GRAPHICS_H
