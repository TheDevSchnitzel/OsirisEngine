#include "stdafx.h"

using namespace OsirisI::Graphics::Renderer;
using namespace OsirisI::Graphics::Shader;
using namespace OsirisI::Utilities;

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
			DX11Texture::DX11Texture(ID3D11ShaderResourceView* texture, HashedString name, HashedString containerPath) : ITexture(name) {
				this->texture = texture;
                this->containerPath = containerPath;
			}

			DX11Texture::~DX11Texture(){
				Release();
			}

			ID3D11ShaderResourceView* DX11Texture::GetRawTexture() {
				return this->texture;
			}

			ReturnState DX11Texture::Release() {
				if(this->texture != nullptr){
					this->texture->Release();
                    this->texture = nullptr;
				}
				return OS_OK;
			}
			
			ReturnState DX11Texture::Recover() {
				return OS_OK;
			}

			ReturnState DX11Texture::Update(float delta) {
				return OS_OK;
			}
		}
	}
}
