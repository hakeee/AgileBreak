//
// Created by hakeee on 2015-09-09.
//

#ifndef DISCOBREAKWINDOWS_GLBUFFER_H
#define DISCOBREAKWINDOWS_GLBUFFER_H

#include "../buffer.h"
#include <GL/glew.h>

namespace discobreak {
    class Wggraphics;
    class LGgraphics;
    class GLBuffer {

        GLuint vertexId_, indexId_;

        GLBuffer(Wggraphics* graphics);

        GLBuffer(LGgraphics* graphics);

    public:
        ~GLBuffer();

        void Use();

        void SetVertexData(unsigned int count, float* array);

        void SetIndices(unsigned int count, unsigned int* array);

        friend class Buffer;
        friend class LGgraphics;
        friend class Wggraphics;
    };
}

#endif //DISCOBREAKWINDOWS_GLBUFFER_H
