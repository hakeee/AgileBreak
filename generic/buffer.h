//
// Created by hakeee on 2015-09-09.
//

#ifndef DISCOBREAK_BUFFER_H
#define DISCOBREAK_BUFFER_H

namespace discobreak {
    class GLBuffer;

#ifdef _WIN32
    class DxBuffer;
#endif
    class Buffer {
#ifdef _WIN32
        DxBuffer* dxb_;
        GLBuffer* glb_;

        Buffer(DxBuffer *dxb):glb_(nullptr), dxb_(dxb) {};
        Buffer(GLBuffer *glb):glb_(glb), dxb_(nullptr) {};
#else
        GLBuffer *glb_;
        Buffer(GLBuffer *glb):glb_(glb) {};
#endif

        unsigned int vertexCount_, indexCount_, stride_; // ska finnas i glb/dxb

        void Use();

        unsigned int GetIndicesCount() { return indexCount_; };
        unsigned int GetVertexCount() { return vertexCount_; };
        unsigned int GetStride() { return stride_; };

    public:
        ~Buffer();

        // count = amount of floats, array = pointer to data array, stride = size of a vertex point
        void SetVertexData(unsigned long count, float *array, unsigned int stride);

        // count = amounts of unsigned ints, array = pointer to data array
        void SetIndices(unsigned long count, unsigned int *array);

        friend class LGgraphics;
		friend class Wggraphics;
		friend class Wdgraphics;
        friend class GLShader;
        friend class DxShader;

    };
};

#endif //DISCOBREAK_BUFFER_H
