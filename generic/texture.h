#ifndef DISCOBREAK_TEXTURE_H
#define DISCOBREAK_TEXTURE_H
#include <string>


namespace discobreak {
	class GLTexture;
#ifdef _WIN32
	class DxTexture;
#endif //_WIN32
	class Texture {
	private:
#ifdef _WIN32
		DxTexture* dxTex_;
		GLTexture* glTex_;
		Texture(DxTexture* dxTex) :dxTex_(dxTex), glTex_(nullptr) {};
		Texture(GLTexture* glTex) :dxTex_(nullptr), glTex_(glTex){};
#else
		GLTexture* glTex_;
		Texture(GLTexture* glTex) :glTex_(glTex) {};
#endif//_WIN32
		//Set texture to shader
		void SetTexture(unsigned int bindPoint);
	public:
		~Texture();
		//Loads a texture from file, Only works with .dds. Returns true if successful
		bool Load(const std::string& filename);


		friend class LGgraphics;
		friend class Wdgraphics;
		friend class Wggraphics;
		friend class DxShader;
		friend class GLShader;
	};
}

#endif //DISCOBREAK_TEXTURE_H