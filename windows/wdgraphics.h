
#ifndef WDGRAHPICS_H_
#define WDGRAPHICS_H_

#include <d3d11.h>

namespace discobreak {
	class Graphics;
	class Wwindow;
	class Shader;
	class Buffer;
	class Texture;
	class Wdgraphics {

	private:



		Wdgraphics(Wwindow* window);
		ID3D11Device*			device_ = nullptr;
		ID3D11DeviceContext*	deviceContext_ = nullptr;
		IDXGISwapChain*			swapChain_ = nullptr;
		ID3D11RenderTargetView* renderTarget_ = nullptr;

		ID3D11Texture2D*		depthStencilBuffer_ = nullptr;
		ID3D11DepthStencilView*	depthStencilView_ = nullptr;

		ID3D11RasterizerState*	rasterizerState_ = nullptr;

		D3D11_VIEWPORT			viewPort_;


		unsigned int windowWidth_;
		unsigned int windowHeight_;


		HRESULT InitDeviceAndSwapChain(HWND hWnd);
		HRESULT CreateRenderTargetView();
		HRESULT CreateDepthStencilView();
		HRESULT CreateRasterizerState();
		void	SetViewport();


	public:

		~Wdgraphics();

		void Swap();
		void Clear(float r, float g, float b, float a);

		Buffer* NewBuffer();

		Shader* NewShader();

		Texture* NewTexture();

		void PrintVersion();

		friend class Graphics;
		friend class Wwindow;
		friend class DxBuffer;
		friend class DxShader;
		friend class DxConstantBuffer;
		friend class DxTexture;
	};
	
};


#endif //WDGRAPHICS_H_