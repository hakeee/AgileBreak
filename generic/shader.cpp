//
// Created by hakeee on 2015-09-09.
//

#include "shader.h"
#include "opengl/glshader.h"

#ifdef _WIN32
#include "../windows/directx/dxshader.h"
#endif

bool discobreak::Shader::Load(std::string& filename) {
    if(gls_) return gls_->Load(filename);
#ifdef _WIN32
    if(dxs_) return dxs_->Load(filename);
#endif
    return false;
}



void discobreak::Shader::Draw() {
    if(gls_) gls_->Draw();
#ifdef _WIN32
    if(dxs_) dxs_->Draw();
#endif
}

discobreak::Shader::~Shader() {
    if(gls_) delete gls_;
#ifdef _WIN32
    if(dxs_) delete dxs_;
#endif
}
