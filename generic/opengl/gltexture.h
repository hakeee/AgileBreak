//
// Created by hakeee on 2015-09-18.
//

#ifndef DISCOBREAK_GLTEXTURE_H
#define DISCOBREAK_GLTEXTURE_H
#include <string>
#include <GL/glew.h>

namespace discobreak {
    class GLTexture {
        unsigned int height, width, linearSize, mipMapCount, fourCC;
        GLuint textureID;
		GLTexture() {};
    public:

        ~GLTexture();

        bool Load(const std::string &filename);
        void SetTexture(unsigned int bindPoint);

		friend class Wggraphics;
		friend class LGgraphics;
    };
}

#endif //DISCOBREAK_GLTEXTURE_H
