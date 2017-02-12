#include "stdafx.h"

using namespace OsirisI::Resources;
using namespace OsirisI::Graphics;
using namespace OsirisI::Manager;
using namespace OsirisI::Utilities;
using namespace OsirisI::Graphics::Devices;
using namespace OsirisI::Graphics::Actors;

namespace OsirisI {
	namespace Resources {
		namespace Loader {
			DXTextureLoader::DXTextureLoader() {
			}
			DXTextureLoader::~DXTextureLoader() {
			}

			Resource* DXTextureLoader::Load(HashedString path, HashedString resourceName) {
                if(IGraphicsDevice::g_graphicDevice->GetDeviceType() != DirectX11) return nullptr;

				ID3D11Device* device = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDevice();
				ID3D11DeviceContext *devCon = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDeviceContext();
				ID3D11ShaderResourceView* texture;
                ITexture* iTexture = nullptr;
				unsigned int size = 0;
				bool errorOccured = false;

                ResourcePtr<char*>* resource = dynamic_cast<ResourcePtr<char*>*>(ResourceManager::GetResourceDirectly(path.GetString()));
					if(resource == nullptr) return nullptr;

					HRESULT result = S_OK;
					if (HelperFunctions::EndsWith( path.GetString(),".dds")) {
						result = CreateDDSTextureFromMemory(device, (uint8_t*)resource->GetValue(), (size_t)resource->GetSize(), NULL, &texture, NULL);
					}
					else {
						result = CreateWICTextureFromMemory(device, devCon, (const uint8_t*)resource->GetValue(), (size_t)resource->GetSize(), NULL, &texture, NULL);
					}
					if(FAILED(result)) {
						errorOccured = true;
					}
					else {
                        iTexture = (new DX11Texture(texture, resourceName, path));/*, [=](ITexture* tex) {
							ResourceManager::GetCache<ITexture>()->ReleaseResource(resourceName); 
						});*/
						size = resource->GetSize();
					}
				if(errorOccured) {
                    LOG_DEBUG("DXTextureLoader_Error: {" << "[path]: " << path << ", [resourceName]: " << resourceName.GetString() << "}");
					return nullptr;
				}

				if(iTexture == nullptr) return nullptr;

				//Get Height and Width
				ID3D11Resource *tmpResource = nullptr;
				ID3D11Texture2D *tmpTexture = nullptr;
				D3D11_TEXTURE2D_DESC desc;
				texture->GetResource(&tmpResource);
				tmpResource->QueryInterface<ID3D11Texture2D>(&tmpTexture);	
				tmpTexture->GetDesc(&desc);	
				iTexture->SetHeight(desc.Height);
				iTexture->SetWidth(desc.Width);	

				return new ResourcePtr<ITexture*>(size, iTexture, path);
			}


            Resource* DXTextureLoader::Load(char* data, long size, HashedString resourceName) {
                if (IGraphicsDevice::g_graphicDevice->GetDeviceType() != DirectX11) return nullptr;

                ID3D11Device* device = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDevice();
                ID3D11ShaderResourceView* texture;
                ITexture* iTexture = nullptr;
                bool errorOccured = false;

                if (data == nullptr) return nullptr;
				
				
				HRESULT result = S_OK;
				if (resourceName.GetString().find("dds") == 0) {
					result = CreateDDSTextureFromMemory(device, (uint8_t*)data, (size_t)size, NULL, &texture, NULL);
				}
				else {
					result = CreateWICTextureFromMemory(device, (uint8_t*)data, (size_t)size, NULL, &texture, NULL);
				}

                if (FAILED(result)) {
                    errorOccured = true;
                }
                else {
					
                    iTexture = (new DX11Texture(texture, resourceName, HashedString("FromData")));
                    /*, [=](ITexture* tex) {
                    ResourceManager::GetCache<ITexture>()->ReleaseResource(resourceName);
                    });*/
                }
                if (errorOccured) {
                    LOG_DEBUG("DXTextureLoader_Error: {" << "[Data], [resourceName]: " << resourceName.GetString() << "}");
                    return nullptr;
                }

                if (iTexture == nullptr) return nullptr;

                //Get Height and Width
                ID3D11Resource *tmpResource = nullptr;
                ID3D11Texture2D *tmpTexture = nullptr;
                D3D11_TEXTURE2D_DESC desc;
                texture->GetResource(&tmpResource);
                tmpResource->QueryInterface<ID3D11Texture2D>(&tmpTexture);
                tmpTexture->GetDesc(&desc);
                iTexture->SetHeight(desc.Height);
                iTexture->SetWidth(desc.Width);

                return new ResourcePtr<ITexture*>(size, iTexture, HashedString("FromData"));
            }

			ReturnState DXTextureLoader::Release() {
				return OS_OK;
			}
		}
	}
}
