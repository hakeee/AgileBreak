
#ifndef WGGRAHPICS_H_
#define WGGRAPHICS_H_

#include "wwindow.h"

namespace discobreak {
	class Wwindow;
	class Graphics;
    class Shader;
    class Buffer;
	class Texture;
	class Wggraphics {

		HDC mhDC;
		HGLRC mhRC;

		Wwindow* win_;
	private:

		Wggraphics(Wwindow* window);
		~Wggraphics();

		unsigned int windowWidth_;
		unsigned int windowHeight_;


		void Swap();
		void Clear(float r, float g, float b, float a);

        Buffer* NewBuffer();
        Shader* NewShader();
		Texture* NewTexture();
        void PrintVersion();

		friend class Graphics;
		friend class Wwindow;
		friend class GLShader;
	};
	
};























#endif //WDGRAPHICS_H_