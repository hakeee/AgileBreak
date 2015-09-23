#include "texture.h"

#include "opengl/gltexture.h"
#ifdef _WIN32
#include "../windows//directx/dxtexture.h"
#endif

using namespace discobreak;

Texture::~Texture() {
	if (glTex_) delete glTex_;
#ifdef _WIN32
	if (dxTex_) delete dxTex_;
#endif
}

bool Texture::Load(const std::string& filename) {
	if (glTex_) return glTex_->Load(filename);
#ifdef _WIN32
	if (dxTex_) return dxTex_->Load(filename);
#endif
	return false;
}

void Texture::SetTexture(unsigned int bindPoint) {
	if (glTex_) return glTex_->SetTexture(bindPoint);
#ifdef _WIN32
	if (dxTex_) return dxTex_->SetTexture(bindPoint);
#endif
}