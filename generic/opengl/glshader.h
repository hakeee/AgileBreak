//
// Created by hakeee on 2015-09-09.
//

#ifndef DISCOBREAKWINDOWS_GLSHADER_H
#define DISCOBREAKWINDOWS_GLSHADER_H

#include <list>
#include <string>
#include <vector>
#include <tuple>
#include <GL/glew.h>

namespace discobreak {
    class GLShader {
        GLuint program_, varray_;
        GLuint vs_, is_;
        //                          namn    type    count
        std::vector<std::tuple<std::string, GLenum, GLint>> attribs_;
        std::vector<std::tuple<std::string, GLenum, GLint>> uniforms_;

        int GetFloatCount(GLenum) const;
    public:
        ~GLShader();
        bool Load(std::string &filename);

        void Draw() const;// get buffer and worldmatrics
    };
};

#endif //DISCOBREAKWINDOWS_GLSHADER_H
