#ifndef DISCOBREAKWINDOWS_DXTEXTURE_H
#define DISCOBREAKWINDOWS_DXTEXTURE_H

#include <d3d11.h>
#include <string>

namespace discobreak {
	class Wdgraphics;
	class DxTexture {
	public:
		DxTexture(Wdgraphics* graphics);
		bool Load(const std::string& filename);

		ID3D11ShaderResourceView* GetTextureResource();
		void SetTexture(unsigned int bindPoint);

		~DxTexture();
	private:

		ID3D11ShaderResourceView* texture_;
		unsigned int width_;
		unsigned int height_;

		Wdgraphics* graphics_;

		friend class Wdgraphics;
	};
}
#endif