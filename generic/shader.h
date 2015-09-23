//
// Created by hakeee on 2015-09-09.
//

#ifndef DISCOBREAK_SHADER_H
#define DISCOBREAK_SHADER_H

#include <list>
#include <string>

namespace discobreak {
    class GLShader;
#ifdef _WIN32
    class DxShader;
#endif

    class Shader {
#ifdef _WIN32
        DxShader* dxs_;
        GLShader* gls_;
		Shader(DxShader* dxs) : dxs_(dxs), gls_(nullptr) {};
		Shader(GLShader* gls) : gls_(gls), dxs_(nullptr) {};
#else
        GLShader *gls_;
        Shader(GLShader *gls):gls_(gls) {}
#endif
    public:
        ~Shader();

        // loads shader from file. filename = filename including path if needed. return false if it failed otherwise true
        bool Load(std::string& filename);

        // draws the selected gameobject to backbuffer.
        void Draw();

        friend class LGgraphics;
	    friend class Wdgraphics;
        friend class Wggraphics;
    };
};


#endif //DISCOBREAK_SHADER_H
