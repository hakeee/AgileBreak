//
// Created by hakeee on 2015-09-06.
//

#include "graphics.h"
#ifdef _WIN32
#include "../windows/wdgraphics.h"
#include "../windows/wggraphics.h"
#else
#include "../linux/lggraphics.h"
#endif

using namespace discobreak;


void Graphics::Swap() {
	if (gg_ != nullptr) {
		gg_->Swap();
	}
#ifdef _WIN32
	else if (dg_ != nullptr) {
		dg_->Swap();
	}
#endif
};

void Graphics::Clear(float r, float g, float b, float a) {
	if (gg_ != nullptr) {
		gg_->Clear(r,g,b,a);
	}
#ifdef _WIN32
	else if (dg_ != nullptr) {
		dg_->Clear(r,g,b,a);
	}
#endif
};

Graphics::~Graphics() {
    if (gg_ != nullptr) {
        delete gg_;
    }
#ifdef _WIN32
    else if (dg_ != nullptr) {
			  delete dg_;
		  }
#endif
}

Buffer* Graphics::NewBuffer() {
    if(gg_) return gg_->NewBuffer();
#ifdef _WIN32
    if(dg_) return dg_->NewBuffer();
#endif
	return nullptr;
}

Shader* Graphics::NewShader() {
    if(gg_) return gg_->NewShader();
#ifdef _WIN32
    if(dg_) return dg_->NewShader();
#endif
	return nullptr;
}

Texture* Graphics::NewTexture() {
	if (gg_) return gg_->NewTexture();
#ifdef _WIN32
	if (dg_) return dg_->NewTexture();
#endif
	return nullptr;
}

void Graphics::PrintVersion() {
    if(gg_) gg_->PrintVersion();
#ifdef _WIN32
    if(dg_) dg_->PrintVersion();
#endif
}
