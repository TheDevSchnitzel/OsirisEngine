#ifndef DIRECT3DDEVICE_HPP_
#define DIRECT3DDEVICE_HPP_
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Buffer;	
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11DepthStencilState;
struct ID3D11BlendState;
struct ID3D11RasterizerState;
struct ID3D10Device1;
struct ID2D1RenderTarget;
struct IDXGIKeyedMutex;
struct IDXGIAdapter1;

enum D3D_FEATURE_LEVEL;
enum D3D_DRIVER_TYPE;

namespace OsirisI {
	namespace Graphics {
		namespace Devices {
			class OsirisIHWND;
			class Direct3DDevice : public IGraphicsDevice {
				private:
                    #pragma region ID3D11
					ID3D11Device*   iD3D11Device;                     // the pointer to our Direct3D device interface
					ID3D11DeviceContext* iD3D11DeviceContext;         // the pointer to our Direct3D device context
					IDXGISwapChain* SwapChain;             // the pointer to the swap chain interface
					ID3D11RenderTargetView* iD3D11RenderTargetView;    // the pointer to our back buffer
					ID3D11Buffer* bufferVertex;               // the pointer to the vertex buffer	

					ID3D11Texture2D* DepthBuffer;
					ID3D11DepthStencilView*  DepthBufferView;       
					ID3D11DepthStencilState* DepthBufferState;
					ID3D11DepthStencilState* DisabledDepthBufferState;	

					ID3D11BlendState* EnabledAlphaBlendState;
					ID3D11BlendState* DisabledAlphaBlendState;

					ID3D11RasterizerState* RasterizerState;

					D3D_FEATURE_LEVEL featureLevel;
					D3D_DRIVER_TYPE driverType;
                    #pragma endregion

					bool vsync, windowed;


				public:
					Direct3DDevice(bool vsync, int width, int height, bool windowed);
					~Direct3DDevice();

					ReturnState Initialize(WNDPROC proc, std::string windowName);
					ReturnState Recover();
					ReturnState Release();

                private:
                    ReturnState CreateDeviceAndSwapChainD3D10(IDXGIAdapter1* adapter);
                    ReturnState CreateDeviceAndSwapChainD3D11();
					ReturnState CreateDepthBuffer();
                    ReturnState CreateBackBufferAndRenderTarget();
                    ReturnState CreateAlphaBlendState();
					ReturnState CreateRasterState();
					ReturnState CreateViewport();

                public:
                    ReturnState TurnZBufferOn();
                    ReturnState TurnZBufferOff();
                    ReturnState TurnAlphaBlendingOn();
                    ReturnState TurnAlphaBlendingOff();

                    #pragma region Getter/Setter
					ID3D11Device* GetDevice();
					ID3D11DeviceContext* GetDeviceContext();
					IDXGISwapChain* GetSwapChain();
					ID3D11RenderTargetView* GetRenderTargetView();
					ID3D11Buffer* GetBufferVertex();
					ID3D11Texture2D* GetDepthBuffer();
					ID3D11DepthStencilView*  GetDepthBufferView();
					ID3D11DepthStencilState* GetDepthBufferState();
					ID3D11DepthStencilState* GetDisabledDepthBufferState();		
                    ID3D11BlendState* GetEnabledAlphaBlendState();
                    ID3D11BlendState* GetDisabledAlphaBlendState();
					ID3D11RasterizerState* GetRasterizerState();
					bool IsVsync();
                    GraphicsDeviceType GetDeviceType();

					void SetSwapChain(IDXGISwapChain* value);
					void SetRenderTargetView(ID3D11RenderTargetView* value);
					void SetBufferVertex(ID3D11Buffer* value);
					void SetDepthBuffer(ID3D11Texture2D* value);
					void SetDepthBufferView(ID3D11DepthStencilView* value);
					void SetDepthBufferState(ID3D11DepthStencilState* value);
					void SetDisabledDepthBufferState(ID3D11DepthStencilState* value);			
                    void SetEnabledAlphaBlendState(ID3D11BlendState* value);
                    void SetDisabledAlphaBlendState(ID3D11BlendState* value);
					void SetRasterizerState(ID3D11RasterizerState* value);
				#pragma endregion

				#pragma region Helper
					ReturnState CreateStandardVertexBuffer(ID3D11Buffer** bufferPtr, unsigned int vertexCount, OVertex* vertices, bool dynamic = false);
					ReturnState CreateStandardIndexBuffer(ID3D11Buffer** bufferPtr, unsigned int indexCount, unsigned long* indices);
				#pragma endregion
			};
		}
	}
}

#endif /* DIRECT3DDEVICE_HPP_ */
