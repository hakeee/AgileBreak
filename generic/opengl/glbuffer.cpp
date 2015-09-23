//
// Created by hakeee on 2015-09-09.
//

#include "glbuffer.h"
#include <GL/glew.h>
#include <GL/glu.h>

void discobreak::GLBuffer::Use() {
    glBindBuffer(GL_ARRAY_BUFFER, vertexId_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId_);
}

void discobreak::GLBuffer::SetVertexData(unsigned int count, float* array) {
    glBindBuffer(GL_ARRAY_BUFFER, vertexId_);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), array, GL_STATIC_DRAW);
}

void discobreak::GLBuffer::SetIndices(unsigned int count, unsigned int* array) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), array, GL_STATIC_DRAW);
}

discobreak::GLBuffer::GLBuffer(Wggraphics* graphics) {
    (void)graphics;
    glGenBuffers(1, &vertexId_);
    glGenBuffers(1, &indexId_);
}

discobreak::GLBuffer::GLBuffer(LGgraphics* graphics) {
    (void)graphics;
    glGenBuffers(1, &vertexId_);
    glGenBuffers(1, &indexId_);
}

discobreak::GLBuffer::~GLBuffer() {
    glDeleteBuffers(1, &vertexId_);
    glDeleteBuffers(1, &indexId_);
}
