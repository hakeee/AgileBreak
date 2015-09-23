//
// Created by hakeee on 2015-09-09.
//

#include <GL/glew.h>
#include <GL/glu.h>
#include <fstream>
#include <vector>
#include <tuple>
#include "glshader.h"
#include "../texture.h"

using namespace discobreak;

bool GLShader::Load(std::string& filename) {

    std::ifstream in, in2;
    in.open(filename + ".vert", std::ios_base::in);

    if(!in.is_open())
    {
        printf("Couldn't open file at \"%s.vert\"\n", filename.c_str());
        return false;
    }

    std::string shaderSource;
    shaderSource.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    in.close();

    vs_ = glCreateShader(GL_VERTEX_SHADER);
    if(vs_ == 0)
    {
        printf("glCreateShader failed!\n");
        return false;
    }

    const GLchar* shaderSourceChar = shaderSource.c_str();
    glShaderSource(vs_, 1, static_cast<const GLchar**>(&shaderSourceChar), 0);
    glCompileShader(vs_);

    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv(vs_, GL_COMPILE_STATUS, &shaderCompiled);
    if(shaderCompiled != GL_TRUE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vs_, GL_INFO_LOG_LENGTH, &maxLength);

        GLint logLength = 0;
        GLchar* log = new char[maxLength];
        glGetShaderInfoLog(vs_, maxLength, &logLength, log);
        if(logLength > 0)
            printf("%s\n", log);

        delete[] log;
        glDeleteShader(vs_);
        vs_ = 0;
        return false;
    }
    glGetError();
    in2.open(filename + ".frag", std::ios_base::in);

    if(!in2.is_open())
    {
        printf("Couldn't open file at \"%s.frag\"\n", filename.c_str());
        return false;
    }

    shaderSource = std::string();
    shaderSource.assign(std::istreambuf_iterator<char>(in2), std::istreambuf_iterator<char>());
    in2.close();

    is_ = glCreateShader(GL_FRAGMENT_SHADER);
    if(is_ == 0)
    {
        printf("glCreateShader failed!\n");
        return false;
    }

    shaderSourceChar = shaderSource.c_str();
    glShaderSource(is_, 1, static_cast<const GLchar**>(&shaderSourceChar), 0);
    glCompileShader(is_);

    shaderCompiled = GL_FALSE;
    glGetShaderiv(is_, GL_COMPILE_STATUS, &shaderCompiled);

    if(shaderCompiled != GL_TRUE)
    {
        printf("GL Error: %i\n",glGetError());
        GLint maxLength = 0;
        glGetShaderiv(is_, GL_INFO_LOG_LENGTH, &maxLength);

        GLint logLength = 0;
        GLchar* log = new char[maxLength];
        glGetShaderInfoLog(is_, maxLength, &logLength, log);

        if(logLength >= 0)
            printf("%s\n", log);

            delete[] log;
        glDeleteShader(vs_);
        glDeleteShader(is_);
        is_ = 0;
        return false;
    }

    program_ = glCreateProgram();
    if(program_ == 0)
    {
        printf("Couldn't create shader program!\n");
        glDeleteShader(vs_);
        glDeleteShader(is_);

        return false;
    }

    glAttachShader(program_, vs_);
    glAttachShader(program_, is_);
    glLinkProgram(program_);
    glUseProgram(program_);

    GLint linkSuccess = GL_TRUE;
    glGetProgramiv(program_, GL_LINK_STATUS, &linkSuccess);
    if(linkSuccess != GL_TRUE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &maxLength);

        GLint logLength = 0;
        GLchar* log = new char[maxLength];
        glGetShaderInfoLog(program_, maxLength, &logLength, log);
        if(logLength > 0)
            printf("%s\n", log);
        else
            printf("An error occured while linking shader program but no log was available!");

        delete[] log;

        return false;
    }

    GLint numActiveAttribs = 0;
    GLint numActiveUniforms = 0;
    glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);
    glGetProgramiv(program_, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

    GLint maxAttribNameLength = 0;
    glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);
    std::vector<GLchar> nameDataAttrib(maxAttribNameLength);
    for(int attrib = 0; attrib < numActiveAttribs; ++attrib)
    {
        GLint arraySize = 0;
        GLenum type = 0;
        GLsizei actualLength = 0;
        glGetActiveAttrib(program_, attrib, nameDataAttrib.size(), &actualLength, &arraySize, &type, &nameDataAttrib[0]);
        std::string name((char*)&nameDataAttrib[0], actualLength);
        attribs_.push_back(std::make_tuple(name, type, arraySize));

    }

    std::vector<GLchar> nameDataUni(256);
    for(int unif = 0; unif < numActiveUniforms; ++unif)
    {
        GLint arraySize = 0;
        GLenum type = 0;
        GLsizei actualLength = 0;
        glGetActiveUniform(program_, unif, nameDataUni.size(), &actualLength, &arraySize, &type, &nameDataUni[0]);
        std::string name((char*)&nameDataUni[0], actualLength);
        uniforms_.push_back(std::make_tuple(name, type, arraySize));
    }
    return true;
}

void GLShader::Draw() const {
	/*glUseProgram(program_);
	
    object.GetBuffer()->Use();

    int offset = 0;
    for(unsigned int i = 0; i < attribs_.size(); i++) {
        GLuint p = (GLuint) glGetAttribLocation(program_, std::get<0>(attribs_[i]).c_str());
        glEnableVertexAttribArray(p);
        int fs = std::get<2>(attribs_[i]) * GetFloatCount(std::get<1>(attribs_[i]));
        glVertexAttribPointer(p, fs, GL_FLOAT, GL_FALSE, object.GetBuffer()->GetStride(), reinterpret_cast<void*>(offset));
        offset += sizeof(float) * fs;
    }

    for(unsigned int i = 0; i < uniforms_.size(); i++) {
        int p = glGetUniformLocation(program_, std::get<0>(uniforms_[i]).c_str());
        if(object.GetUniforms().count(std::get<0>(uniforms_[i])) == 0 && object.GetTextures().count(std::get<0>(uniforms_[i])) == 0) {
            printf("Uniform with name \"%s\" does not exist in current gameobject\n", std::get<0>(uniforms_[i]).c_str());
            continue;
        }
        switch (std::get<1>(uniforms_[i])) {
            case GL_FLOAT_MAT4:
                glUniformMatrix4fv(p, std::get<2>(uniforms_[i]), GL_FALSE, (float*)object.GetUniforms().at(std::get<0>(uniforms_[i])));
                break;
            case GL_FLOAT:
                glUniform1fv(p, std::get<2>(uniforms_[i]), (float*)object.GetUniforms().at(std::get<0>(uniforms_[i])));
                break;
            case GL_SAMPLER_2D:
                object.GetTextures().at(std::get<0>(uniforms_[i]))->SetTexture(p);
                break;
            default:
                printf("Uniform shadername:\"%s\"Enum code:%i, hex: %x, not implemented in \"int GLShader::Uniform(GLenum lenum)\"\n", std::get<0>(uniforms_[i]).c_str(), std::get<1>(uniforms_[i]), std::get<1>(uniforms_[i]));
       }
    }

    glDrawElements(
            GL_TRIANGLES,      // mode
            object.GetBuffer()->GetIndicesCount(),    // count
            GL_UNSIGNED_INT,   // type
            (void*)0           // element array buffer offset
    );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
	*/
}

int GLShader::GetFloatCount(GLenum lenum) const {
    switch (lenum) {
        case GL_FLOAT_VEC3:
            return 3;
        case GL_FLOAT_VEC2:
            return 2;
        case GL_FLOAT:
            return 1;
        default:
            printf("Enum code:%i, not implemented in \"int GLShader::GetFloatCount(GLenum lenum)\"", lenum);
    }
    return 0;
}

GLShader::~GLShader() {
    glDeleteProgram(program_);
    glDeleteShader(vs_);
    glDeleteShader(is_);
}
