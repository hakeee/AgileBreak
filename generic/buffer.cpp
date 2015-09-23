//
// Created by hakeee on 2015-09-09.
//

#include "buffer.h"
#include "window.h"
#include "opengl/glbuffer.h"
#ifdef _WIN32
#include "../windows/directx/dxbuffer.h"
#endif

discobreak::Buffer::~Buffer() {
	if (glb_)
		delete glb_;
#ifdef _WIN32
	if (dxb_)
		delete dxb_;
#endif
}

void discobreak::Buffer::SetVertexData(unsigned long count, float *array, unsigned int stride) {
    stride_ = stride;
    vertexCount_ = count;
    if(glb_) glb_->SetVertexData(count, array);
#ifdef _WIN32
    if(dxb_) dxb_->SetVertexData(count, array);
#endif
}

void discobreak::Buffer::Use() {
    if(glb_) glb_->Use();
#ifdef _WIN32
    if(dxb_) dxb_->Use(stride_);
#endif
}

void discobreak::Buffer::SetIndices(unsigned long count, unsigned int *array) {
    indexCount_ = count;
    if(glb_) glb_->SetIndices(count, array);
#ifdef _WIN32
    if(dxb_) dxb_->SetIndices(count, array);
#endif
}