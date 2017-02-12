#include "stdafx.h"

using namespace OsirisI::Utilities;

namespace OsirisI {
	namespace Graphics {
        namespace Devices {
            IGraphicsDevice *IGraphicsDevice::g_graphicDevice; //Needed for multiple purposes
            OsirisI::OMatrix4x4 IGraphicsDevice::WorldMatrix, IGraphicsDevice::ProjectionMatrix, IGraphicsDevice::OrthoMatrix;

            Direct3DDevice::Direct3DDevice(bool vsync, int width, int height, bool windowed) {
                this->iD3D11Device = nullptr;
                this->iD3D11DeviceContext = nullptr;
                this->SwapChain = nullptr;
                this->iD3D11RenderTargetView = nullptr;
                this->bufferVertex = nullptr;

                this->DepthBuffer = nullptr;
                this->DepthBufferView = nullptr;
                this->DepthBufferState = nullptr;
                this->DisabledDepthBufferState = nullptr;

                this->EnabledAlphaBlendState = nullptr;
                this->DisabledAlphaBlendState = nullptr;

                this->RasterizerState = nullptr;
				
                this->vsync = vsync;
                this->windowed = windowed;
                this->width = width;
                this->height = height;
/*
                IGraphicsDevice::WorldMatrix;
                IGraphicsDevice::ProjectionMatrix;
                IGraphicsDevice::OrthoMatrix;
*/
                g_graphicDevice = this;
            }

            Direct3DDevice::~Direct3DDevice() {
            }

            ReturnState Direct3DDevice::Initialize(WNDPROC proc, std::string windowName) {
                this->window = new OsirisIHWND(this->width, this->height, this->windowed);
				((OsirisIHWND*)this->window)->SetWNDPROC(proc);
				this->window->SetWindowName(windowName);

				OS_CHECKSTATE(this->window->Initialize());
                OS_CHECKSTATE(CreateDeviceAndSwapChainD3D11()); //Ruft intern OS_CHECKSTATE(CreateDeviceAndSwapChainD3D10()) auf
                OS_CHECKSTATE(CreateDepthBuffer());
                OS_CHECKSTATE(CreateBackBufferAndRenderTarget());
                OS_CHECKSTATE(CreateAlphaBlendState());
                OS_CHECKSTATE(CreateRasterState());
                OS_CHECKSTATE(CreateViewport());

                // Setup the projection matrix.
				float fieldOfView = (float)XM_PI / 4.0f;
                float screenAspect = static_cast<float>(this->width) / static_cast<float>(this->height);

                const float screenDepth = 1000.0f; // ScreenDepth
                const float screenNear = 0.1f;     // ScreenNear
                //D3DXMATRIX tmpProjectionMatrix, tmpWorldMatrix, tmpOrthoMatrix;
				XMMATRIX tmpProjectionMatrix, tmpWorldMatrix, tmpOrthoMatrix;

                // Create the projection matrix for 3D rendering.
                //D3DXMatrixPerspectiveFovLH(&tmpProjectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);
				tmpProjectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

                // Initialize the world matrix to the identity matrix.
                //D3DXMatrixIdentity(&tmpWorldMatrix);
				tmpWorldMatrix = XMMatrixIdentity();

                // Create an orthographic projection matrix for 2D rendering.
                //D3DXMatrixOrthoLH(&tmpOrthoMatrix, static_cast<float>(this->width), static_cast<float>(this->height), screenNear, screenDepth);
				tmpOrthoMatrix = XMMatrixOrthographicLH(static_cast<float>(this->width), static_cast<float>(this->height), screenNear, screenDepth);

				XMFLOAT4X4 tmp;
				XMStoreFloat4x4(&tmp, tmpProjectionMatrix);
				this->ProjectionMatrix = OMatrix4x4(tmp.m);

				XMStoreFloat4x4(&tmp, tmpWorldMatrix);
				this->WorldMatrix = OMatrix4x4(tmp.m);

				XMStoreFloat4x4(&tmp, tmpOrthoMatrix);
				this->OrthoMatrix = OMatrix4x4(tmp.m);

                return OS_OK;
            }

            ReturnState Direct3DDevice::Recover() {
                return 0x1;
            }

            ReturnState Direct3DDevice::Release() {
                if (this->iD3D11Device != nullptr) this->iD3D11Device->Release();
                if (this->iD3D11DeviceContext != nullptr) this->iD3D11DeviceContext->Release();
                if (this->SwapChain != nullptr) this->SwapChain->Release();

                if (this->DepthBuffer != nullptr) this->DepthBuffer->Release();
                if (this->DepthBufferState != nullptr) this->DepthBufferState->Release();
                if (this->DepthBufferView != nullptr) this->DepthBufferView->Release();
                if (this->DisabledDepthBufferState != nullptr) this->DisabledDepthBufferState->Release();

                if (this->EnabledAlphaBlendState != nullptr) this->EnabledAlphaBlendState->Release();
                if (this->DisabledAlphaBlendState != nullptr) this->DisabledAlphaBlendState->Release();
                if (this->iD3D11RenderTargetView != nullptr) this->iD3D11RenderTargetView->Release();

                if (this->RasterizerState != nullptr) this->RasterizerState->Release();

				if (this->window != nullptr) this->window->Release();



                iD3D11Device = nullptr;
                iD3D11DeviceContext = nullptr;
                SwapChain = nullptr;
                iD3D11RenderTargetView = nullptr;
                bufferVertex = nullptr;

                DepthBuffer = nullptr;
                DepthBufferView = nullptr;
                DepthBufferState = nullptr;
                DisabledDepthBufferState = nullptr;

                EnabledAlphaBlendState = nullptr;
                DisabledAlphaBlendState = nullptr;

                RasterizerState = nullptr;

				window = nullptr;
                return OS_OK;
            }

            ReturnState Direct3DDevice::CreateDeviceAndSwapChainD3D11() {
                D3D_DRIVER_TYPE driverTypes[] = {
                    D3D_DRIVER_TYPE_HARDWARE, 
                    D3D_DRIVER_TYPE_WARP,
                    D3D_DRIVER_TYPE_REFERENCE, 
                    D3D_DRIVER_TYPE_SOFTWARE
                };
                D3D_FEATURE_LEVEL featureLevels[] = {
                    D3D_FEATURE_LEVEL_11_0,
                    D3D_FEATURE_LEVEL_10_1,
                    D3D_FEATURE_LEVEL_10_0
                };
                unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);
                unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);


                // create a struct to hold information about the swap chain
                DXGI_SWAP_CHAIN_DESC scd;

                // clear out the struct for use
                ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

                // fill the swap chain description struct
                scd.BufferCount = 1;                                    // one back buffer
                scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;     // use 32-bit color
				scd.BufferDesc.Width = window->GetWidth();
				scd.BufferDesc.Height = window->GetHeight();

#pragma region IDXGI Usage
				IDXGIFactory1* factory = nullptr;
				IDXGIAdapter1* adapter = nullptr;
                IDXGIOutput* adapterOutput = nullptr;
                unsigned int numModes = 0, i = 0, numerator = 0, denominator = 0;
                DXGI_MODE_DESC* displayModeList = nullptr;
				HRESULT hResult = S_OK;

				#pragma region GetAdapter/Outputs
				if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory))) return 0x8;

                std::string adapterName = "";
                // Use the factory to create an adapter for the primary graphics interface (video card).
                for (unsigned int i = 0; SUCCEEDED(factory->EnumAdapters1(i, &adapter)); i++) {
                    DXGI_ADAPTER_DESC adapterDesc;
                    adapter->GetDesc(&adapterDesc);
                    adapterName = Converter::WStringToString(adapterDesc.Description);
                    
					std::size_t found = adapterName.find("NVIDIA");
                    if (found != std::string::npos) break;

                    found = adapterName.find("AMD");
                    if (found != std::string::npos) break;

					#ifdef DEBUG
						found = adapterName.find("Intel");
						if (found != std::string::npos) break;
					#endif
                }
                if (adapter == nullptr) {
                    return 0x8;
                }
                else {
                    LOG_INFO("Adapter " << adapterName << " loaded...");
                }
				
				// Enumerate the primary adapter output (monitor).
				hResult = adapter->EnumOutputs(0, &adapterOutput);
				if (FAILED(hResult)) {

					IDXGIFactory* factory0 = nullptr;
					IDXGIAdapter* adapter0 = nullptr;
					if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory0))) return 0x8;
					std::string adapterName1 = "";
					// Use the factory to create an adapter for the primary graphics interface (video card).
					for (unsigned int i = 0; SUCCEEDED(factory0->EnumAdapters(i, &adapter0)); i++) {
						DXGI_ADAPTER_DESC adapterDesc;
						adapter0->GetDesc(&adapterDesc);
						adapterName1 = Converter::WStringToString(adapterDesc.Description);

						std::size_t found = adapterName1.find("NVIDIA");
						if (found != std::string::npos) break;

						found = adapterName1.find("AMD");
						if (found != std::string::npos) break;

						#ifdef DEBUG
							found = adapterName.find("Intel");
							if (found != std::string::npos) break;
						#endif
					}
					if (adapter0 == nullptr) {
						return 0x8;
					}
					else {
						LOG_INFO("Adapter " << adapterName1 << " loaded...");
					}


					// Enumerate the primary adapter output (monitor).
					hResult = adapter0->EnumOutputs(0, &adapterOutput);
					if (FAILED(hResult)) {
						LOG_ERROR("adapter0->EnumOutputs HRESULT: " << std::hex << hResult);
						return 0x8;
					}
				}
				#pragma endregion
				

                // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
                hResult = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
                if (FAILED(hResult)){
                    LOG_ERROR("adapterOutput->GetDisplayModeList HRESULT: " << std::hex << hResult);
                    return 0x8;
                }

                // Create a list to hold all the possible display modes for this monitor/video card combination.
                displayModeList = new DXGI_MODE_DESC[numModes];
                if (!displayModeList){
                    LOG_ERROR("!displayModeList");
                    return 0x8;
                }

                // Now fill the display mode list structures.
                hResult = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
                if (FAILED(hResult)) {
                    LOG_ERROR("adapterOutput->GetDisplayModeList HRESULT: " << std::hex << hResult);
                    return 0x8;
                }

#pragma region Get RefreshRate Numerator/Denominator
                if (this->vsync) {
                    for (i = 0; i < numModes; i++) {
						if (displayModeList[i].Width == window->GetWidth()) {
							if (displayModeList[i].Height == window->GetHeight())	{
                                numerator = displayModeList[i].RefreshRate.Numerator;
                                denominator = displayModeList[i].RefreshRate.Denominator;
                            }
                        }
                    }

                    //Set Numerator/Denominator
                    scd.BufferDesc.RefreshRate.Numerator = numerator;
                    scd.BufferDesc.RefreshRate.Denominator = denominator;
                }
                else{
                    scd.BufferDesc.RefreshRate.Numerator = 0;
                    scd.BufferDesc.RefreshRate.Denominator = 1;
                }
#pragma endregion
                
#pragma endregion

                scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
				scd.OutputWindow = ((OsirisIHWND*)this->window)->GetHWND();              // the window to be used
                scd.SampleDesc.Count = 4;                               // how many multisamples
                scd.Windowed = TRUE;                                    // windowed/full-screen mode

                // Turn multisampling off.
                scd.SampleDesc.Count = 1;
                scd.SampleDesc.Quality = 0;

                // Set the scan line ordering and scaling to unspecified.
                scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
                scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

                // Discard the back buffer contents after presenting.
                scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

                // Don't set the advanced flags.
                scd.Flags = 0;


                unsigned int creationFlags = 0;

                // create a device, device context and swap chain using the information in the scd struct
                HRESULT result;
                for (unsigned int driver = 0; driver < totalDriverTypes; ++driver) {
                    try {
                        result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[driver], NULL, creationFlags, featureLevels, totalFeatureLevels, D3D11_SDK_VERSION, &scd, &this->SwapChain, &this->iD3D11Device, &this->featureLevel, &this->iD3D11DeviceContext);
                    }
                    catch(std::exception& ex) {
                        LOG_ERROR("D3D11CreateDeviceAndSwapChain error: " << ex.what());
                    }

                    if (SUCCEEDED(result)) {
                        this->driverType = driverTypes[driver];
                        break;
                    }
                }
                
                #pragma region ReleaseAdapter
                    // Release the display mode list.
                    OS_SAFEDELETE_ARRAY(displayModeList);

                    // Release the adapter output.
                    adapterOutput->Release();
                    adapterOutput = nullptr;

                    // Release the adapter.
                    adapter->Release();
                    adapter = nullptr;

                    // Release the factory.
                    factory->Release();
                    factory = nullptr;
                #pragma endregion

                if (FAILED(result)){
					MessageBox(((OsirisIHWND*)this->window)->GetHWND(), L"Not able to initialize D3D...", L"ERROR", 0);
                    return 0x9;
                }

                LOG_INFO("D3D11CreateDeviceAndSwapChain - DriverType: " << this->driverType);
                LOG_INFO("D3D11CreateDeviceAndSwapChain - FeatureLevel: " << this->featureLevel);
                return OS_OK;
            }
            ReturnState Direct3DDevice::CreateDepthBuffer() {
            #pragma region Create the depth buffer texture
                D3D11_TEXTURE2D_DESC texd;
                ZeroMemory(&texd, sizeof(texd));

                texd.Width = this->window->GetWidth();
                texd.Height = this->window->GetHeight();
                texd.ArraySize = 1;
                texd.MipLevels = 1;
                texd.SampleDesc.Count = 1;
                texd.SampleDesc.Quality = 0;
                texd.Usage = D3D11_USAGE_DEFAULT;
                texd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
                texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
                texd.CPUAccessFlags = 0;
                texd.MiscFlags = 0;

                HRESULT result = this->iD3D11Device->CreateTexture2D(&texd, NULL, &this->DepthBuffer);
                if (FAILED(result)) return 0x5;
#pragma endregion

            #pragma region Create and set the DepthStencilState
                D3D11_DEPTH_STENCIL_DESC dsd;
                ZeroMemory(&dsd, sizeof(dsd));

                // Set up the description of the stencil state.
                dsd.DepthEnable = true;
                dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                dsd.DepthFunc = D3D11_COMPARISON_LESS;

                dsd.StencilEnable = true;
                dsd.StencilReadMask = 0xFF;
                dsd.StencilWriteMask = 0xFF;

                // Stencil operations if pixel is front-facing.
                dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
                dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

                // Stencil operations if pixel is back-facing.
                dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
                dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

                result = this->iD3D11Device->CreateDepthStencilState(&dsd, &this->DepthBufferState);
                if (FAILED(result)) return 0x6;
                this->iD3D11DeviceContext->OMSetDepthStencilState(this->DepthBufferState, 1);
#pragma endregion

            #pragma region Create the DepthStencilView
                D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
                ZeroMemory(&dsvd, sizeof(dsvd));

                dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
                dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
                dsvd.Texture2D.MipSlice = 0;

                result = this->iD3D11Device->CreateDepthStencilView(this->DepthBuffer, &dsvd, &this->DepthBufferView);
                if (FAILED(result)) return 0x7;
#pragma endregion

            #pragma region Create the DeactivatedDepthStencilState
                D3D11_DEPTH_STENCIL_DESC deac_dsd;
                // Clear the second depth stencil state before setting the parameters.
                ZeroMemory(&deac_dsd, sizeof(deac_dsd));

                // Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
                // that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
                deac_dsd.DepthEnable = false;
                deac_dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                deac_dsd.DepthFunc = D3D11_COMPARISON_LESS;
                deac_dsd.StencilEnable = true;
                deac_dsd.StencilReadMask = 0xFF;
                deac_dsd.StencilWriteMask = 0xFF;
                deac_dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                deac_dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
                deac_dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                deac_dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
                deac_dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                deac_dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
                deac_dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                deac_dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

                // Create the state using the device.
                result = this->iD3D11Device->CreateDepthStencilState(&deac_dsd, &this->DisabledDepthBufferState);
                if (FAILED(result)) return 0x6;
#pragma endregion
                return OS_OK;
            }
            ReturnState Direct3DDevice::CreateBackBufferAndRenderTarget() {
                // get the address of the back buffer
                ID3D11Texture2D *pBackBuffer;
                this->SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

                // use the back buffer address to create the render target
                HRESULT result = this->iD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &this->iD3D11RenderTargetView);
                if (FAILED(result)) return 0x4;
                
                pBackBuffer->Release();
                pBackBuffer = 0;

                // set the render target as the back buffer
                this->iD3D11DeviceContext->OMSetRenderTargets(1, &this->iD3D11RenderTargetView, this->DepthBufferView);
                return OS_OK;
            }
            ReturnState Direct3DDevice::CreateAlphaBlendState() {
                D3D11_BLEND_DESC blendStateDesc;
                ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

                // Create an alpha enabled blend state description.
                blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
                blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
                blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
                blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
                blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
                blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
                blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

                // Create the blend state using the description.
                HRESULT result = this->iD3D11Device->CreateBlendState(&blendStateDesc, &this->EnabledAlphaBlendState);
                if (FAILED(result)) return 0x2;

                // Modify the description to create an alpha disabled blend state description.
                blendStateDesc.RenderTarget[0].BlendEnable = FALSE;

                // Create the blend state using the description.
                result = this->iD3D11Device->CreateBlendState(&blendStateDesc, &this->DisabledAlphaBlendState);
                if (FAILED(result)) return 0x2;
                float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
                this->iD3D11DeviceContext->OMSetBlendState(this->DisabledAlphaBlendState, blendFactor, 0xffffffff);

                return OS_OK;
            }
            ReturnState Direct3DDevice::CreateRasterState() {
                D3D11_RASTERIZER_DESC rasterDesc;

                // Setup the raster description which will determine how and what polygons will be drawn.
                rasterDesc.AntialiasedLineEnable = false;
                rasterDesc.CullMode = D3D11_CULL_BACK;
                rasterDesc.DepthBias = 0;
                rasterDesc.DepthBiasClamp = 0.0f;
                rasterDesc.DepthClipEnable = true;
                rasterDesc.FillMode = D3D11_FILL_SOLID;
                rasterDesc.FrontCounterClockwise = false;
                rasterDesc.MultisampleEnable = true;
                rasterDesc.ScissorEnable = false;
                rasterDesc.SlopeScaledDepthBias = 0.0f;

                // Create the rasterizer state from the description we just filled out.
                HRESULT result = this->iD3D11Device->CreateRasterizerState(&rasterDesc, &this->RasterizerState);
                if (FAILED(result)) return 0x3;

                // Now set the rasterizer state.
                this->iD3D11DeviceContext->RSSetState(this->RasterizerState);

                return OS_OK;
            }
            ReturnState Direct3DDevice::CreateViewport() {
                D3D11_VIEWPORT viewport;
                ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

                viewport.TopLeftX = 0.0f;
                viewport.TopLeftY = 0.0f;
                viewport.MinDepth = 0.0f;
                viewport.MaxDepth = 1.0f;
                viewport.Width = static_cast<float>(this->window->GetWidth());
                viewport.Height = static_cast<float>(this->window->GetHeight());

                this->iD3D11DeviceContext->RSSetViewports(1, &viewport);

                return OS_OK;
            }
                     
            ReturnState Direct3DDevice::TurnZBufferOn() {
                iD3D11DeviceContext->OMSetDepthStencilState(GetDepthBufferState(), 1);	
                return OS_OK;
            }
            ReturnState Direct3DDevice::TurnZBufferOff() {
                iD3D11DeviceContext->OMSetDepthStencilState(GetDisabledDepthBufferState(), 1);
                return OS_OK;
            }
            ReturnState Direct3DDevice::TurnAlphaBlendingOn() {
                float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                iD3D11DeviceContext->OMSetBlendState(GetEnabledAlphaBlendState(), blendFactor, 0xffffffff);
                return OS_OK;
            }
            ReturnState Direct3DDevice::TurnAlphaBlendingOff() {
                float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                iD3D11DeviceContext->OMSetBlendState(GetDisabledAlphaBlendState(), blendFactor, 0xffffffff);
                return OS_OK;
            }


            #pragma region Getter/Setter
            ID3D11Device* Direct3DDevice::GetDevice() {
				return iD3D11Device;
			}
			ID3D11DeviceContext* Direct3DDevice::GetDeviceContext() {
				return iD3D11DeviceContext;
			}
			IDXGISwapChain* Direct3DDevice::GetSwapChain() {
				return this->SwapChain;
			}

			ID3D11RenderTargetView* Direct3DDevice::GetRenderTargetView() {
				return this->iD3D11RenderTargetView;
			}

			ID3D11Buffer* Direct3DDevice::GetBufferVertex() {
				return this->bufferVertex;
			}

			ID3D11Texture2D* Direct3DDevice::GetDepthBuffer() {
				return this->DepthBuffer;
			}

			ID3D11DepthStencilView*  Direct3DDevice::GetDepthBufferView() {
				return this->DepthBufferView;
			}

			ID3D11DepthStencilState* Direct3DDevice::GetDepthBufferState() {
				return this->DepthBufferState;
			}

			ID3D11DepthStencilState* Direct3DDevice::GetDisabledDepthBufferState() {
				return this->DisabledDepthBufferState;
			}

			ID3D11BlendState* Direct3DDevice::GetEnabledAlphaBlendState() {
				return this->EnabledAlphaBlendState;
			}
			ID3D11BlendState* Direct3DDevice::GetDisabledAlphaBlendState() {
				return this->DisabledAlphaBlendState;
            }
          
			ID3D11RasterizerState* Direct3DDevice::GetRasterizerState() {
				return this->RasterizerState;
			}
            
			bool Direct3DDevice::IsVsync() {
				return this->vsync;
			}
            
            GraphicsDeviceType Direct3DDevice::GetDeviceType() {
                return DirectX11;
            }

			void Direct3DDevice::SetSwapChain(IDXGISwapChain* value) {
				this->SwapChain = value;
			}

			void Direct3DDevice::SetRenderTargetView(ID3D11RenderTargetView* value) {
				this->iD3D11RenderTargetView = value;
			}

			void Direct3DDevice::SetBufferVertex(ID3D11Buffer* value) {
				this->bufferVertex = value;
			}

			void Direct3DDevice::SetDepthBuffer(ID3D11Texture2D* value) {
				this->DepthBuffer = value;
			}

			void Direct3DDevice::SetDepthBufferView(ID3D11DepthStencilView* value) {
				this->DepthBufferView = value;
			}

			void Direct3DDevice::SetDepthBufferState(ID3D11DepthStencilState* value) {
				this->DepthBufferState = value;
			}

			void Direct3DDevice::SetDisabledDepthBufferState(ID3D11DepthStencilState* value) {
				this->DisabledDepthBufferState = value;
			}

			void Direct3DDevice::SetEnabledAlphaBlendState(ID3D11BlendState* value) {
				this->EnabledAlphaBlendState = value;
			}
			void Direct3DDevice::SetDisabledAlphaBlendState(ID3D11BlendState* value) {
				this->DisabledAlphaBlendState = value;
			}
           
			void Direct3DDevice::SetRasterizerState(ID3D11RasterizerState* value) {
				this->RasterizerState = value;
			}
#pragma endregion

            #pragma region Helpers
			ReturnState Direct3DDevice::CreateStandardVertexBuffer(ID3D11Buffer** bufferPtr, unsigned int vertexCount, OVertex* vertices, bool dynamic) {
				D3D11_BUFFER_DESC vertexBufDesc;
				D3D11_SUBRESOURCE_DATA vertexSubResData;

				if(dynamic) {
					vertexBufDesc.Usage = D3D11_USAGE_DYNAMIC;
					vertexBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				}
				else {
					vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
					vertexBufDesc.CPUAccessFlags = 0;
				}
				vertexBufDesc.ByteWidth = sizeof(OVertex) * vertexCount;
				vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vertexBufDesc.MiscFlags = 0;
				vertexBufDesc.StructureByteStride = 0;

				//Give verticesPtr to the subresource
				vertexSubResData.pSysMem = vertices;
				vertexSubResData.SysMemPitch = 0;
				vertexSubResData.SysMemSlicePitch = 0;


				HRESULT result = GetDevice()->CreateBuffer(&vertexBufDesc, &vertexSubResData, bufferPtr);
				if(FAILED(result)) return 0x15;
				return 0;
			}

			ReturnState Direct3DDevice::CreateStandardIndexBuffer(ID3D11Buffer** bufferPtr, unsigned int indexCount, unsigned long* indices) {
				D3D11_BUFFER_DESC indexBufDesc;
				D3D11_SUBRESOURCE_DATA indexSubResData;

				indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
				indexBufDesc.ByteWidth = sizeof(unsigned long) * indexCount;
				indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				indexBufDesc.CPUAccessFlags = 0;
				indexBufDesc.MiscFlags = 0;
				indexBufDesc.StructureByteStride = 0;

				//Give indicesPtr to the subresource
				indexSubResData.pSysMem = indices;
				indexSubResData.SysMemPitch = 0;
				indexSubResData.SysMemSlicePitch = 0;

				HRESULT result = GetDevice()->CreateBuffer(&indexBufDesc, &indexSubResData, bufferPtr);
				if(FAILED(result)) return 0x14;
				return 0;
			}
#pragma endregion
		}
	}
}
