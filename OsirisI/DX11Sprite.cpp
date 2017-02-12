#include "stdafx.h"

using namespace OsirisI::Manager;
using namespace OsirisI::Utilities;
using namespace OsirisI::Resources;
using namespace OsirisI::Graphics::Devices;
using namespace OsirisI::Graphics::Renderer;
using namespace OsirisI::Graphics::Shader;

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
			DX11Sprite::DX11Sprite(OVector3 pos, OVector2 size, HashedString name, HashedString containerPath) : ISprite(name) {
				this->texture = nullptr;
				this->repeat = false;             
                this->gameModel = new DX11GameModel(pos, name);
				this->containerPath = containerPath;
				positionX = static_cast<unsigned int>(pos.x), positionY = static_cast<unsigned int>(pos.y), prevPositionX = 99999999, prevPositionY = 99999999;
				topLeftU = 0.0f, topLeftV = 0.0f, bottomRightU = 1.0f, bottomRightV = 1.0f, vertexCount = 0;
                this->size = size;
                this->prevSize = OVector2(0.f,0.f);
                this->isColorOnly = false;
			}
			DX11Sprite::DX11Sprite(OVector3 pos, OVector2 size, HashedString name, OVector4 color) : ISprite(name) {
				this->texture = nullptr;
				this->repeat = false;
				this->name = name;
                this->gameModel = new DX11GameModel(OVector3(pos.x, pos.y, pos.z), name);
				this->containerPath = containerPath;
				positionX = static_cast<unsigned int>(pos.x), positionY = static_cast<unsigned int>(pos.y), prevPositionX = 99999999, prevPositionY = 99999999;
				topLeftU = 0.0f, topLeftV = 0.0f, bottomRightU = 1.0f, bottomRightV = 1.0f, vertexCount = 0, indexCount = 0;
                this->size = size;
                this->prevSize = OVector2(0.f,0.f);
                this->isColorOnly = true;
                this->color = color;
			}
			DX11Sprite::~DX11Sprite() {
				Release();
			}
            
			ReturnState DX11Sprite::Update(float time) {
                UpdateVerticesPositions();
				return OS_OK;
			}

			ReturnState DX11Sprite::Recover() {
				return OS_OK;
			}

			ReturnState DX11Sprite::Release() {
                OS_CHECKSTATE(this->gameModel->Release());
                LOG_DEBUG("DX11Sprite at: " << std::hex << this << " released");
				return OS_OK;
			}

			ReturnState DX11Sprite::Initialize() {                
                OsirisI::Graphics::Devices::Direct3DDevice* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice);
				this->vertexCount = 6, this->indexCount = 6;

				OVertex* vertices = new OVertex[vertexCount];	
				memset(vertices, 0, (sizeof(OVertex) * vertexCount));

				unsigned long* indices = new unsigned long[indexCount];
				for(unsigned int i = 0; i < indexCount; i++) indices[i] = i;
                
                DX11Mesh* dx11Mesh = new DX11Mesh(name);
                OS_CHECKSTATE(dx11Mesh->CreateVertexAndIndexBuffer(vertices, indices, vertexCount, indexCount, true));

				OS_SAFEDELETE_ARRAY(vertices);
				OS_SAFEDELETE_ARRAY(indices);

                if (!this->isColorOnly) {
                    ResourcePtr<ITexture*>* res = ((ResourcePtr<ITexture*>*)ResourceManager::GetResource<ITexture>(this->containerPath));
                    if (res == nullptr) {
                        LOG_ERROR("Resource not found/loaded/cached: [" << this->containerPath << "] - [" << this->name.GetString() << "]");
                    }
                    this->texture = (DX11Texture*)res->GetValue();
                }
                this->gameModel->AddMesh(dx11Mesh);

                //Perform an update on the vertex positions
                UpdateVerticesPositions();
                
                if (this->isColorOnly) {
                    ShaderInfoFactory::SetAffectedShader(HashedString("ColorShader").GetHash());
                    ShaderInfoFactory::AddCustomStruct(((char*)&this->color), sizeof(OVector4));
                }
                else {
                    ShaderInfoFactory::AddTexture(this->texture);
                    ShaderInfoFactory::SetAffectedShader(HashedString("TextureShader").GetHash());
                }

                ShaderInfoFactory::SetAffectedShaderType(DX11_PIXEL_SHADER);
                RenderInfoFactory::AddShaderInfo(ShaderInfoFactory::CreateShaderInformation());
                RenderInfoFactory::AddActor(this);
                RenderInfoFactory::TurnZBufferOff(true);
                this->renderInfo = RenderInfoFactory::CreateRenderInformation();

                LOG_DEBUG("DX11Sprite at: " << std::hex << this << " initialized");
				return OS_OK;
			}
            
            OsirisI::Graphics::Renderer::RenderInfo* DX11Sprite::GetRenderInfo() {          
                return this->renderInfo;
            }

            void DX11Sprite::UpdateVerticesPositions() {
                OsirisI::Graphics::Devices::Direct3DDevice* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice);

                float left = 0.f, right = 0.f, top = 0.f, bottom = 0.f;
                D3D11_MAPPED_SUBRESOURCE mappedResource;
                HRESULT hresult;


                //If Sprite hasn´t moved -> return
                if((this->positionX == this->prevPositionX) && (this->positionY == this->prevPositionY) && this->size == this->prevSize) return;
                this->prevPositionX = this->positionX;
                this->prevPositionY = this->positionY;
                this->prevSize = this->size;

                // Calculate the screen coordinates of the left side of the bitmap.
                left = (float)((dev->GetWidth() / 2) * -1) + (float)positionX;

                // Calculate the screen coordinates of the right side of the bitmap.
                right = left + ((this->size.x == 0.0f) ? static_cast<float>((((this->texture != nullptr) ? this->texture->GetWidth() : 0) * (this->bottomRightU - this->topLeftU))) : this->size.x);

                // Calculate the screen coordinates of the top of the bitmap.
                top = (float)(dev->GetHeight() / 2) - (float)positionY;

                // Calculate the screen coordinates of the bottom of the bitmap.
                bottom = top - ((this->size.y == 0.0f) ? static_cast<float>((((this->texture != nullptr) ? this->texture->GetHeight() : 0) * (this->bottomRightV - this->topLeftV))) : this->size.y);



                
                DX11Mesh* dxMesh = (DX11Mesh*)this->gameModel->GetMeshes()[0];
                OVertex* vertices = dxMesh->GetVertices();
                
                // Triangle-1
                vertices[0].SetPosition(OVector3(left, top, 0.0f));  // Top left.
                vertices[0].SetTexture(OVector2(this->topLeftU, this->topLeftV));
                vertices[0].SetNormal(OVector3(0.f, 0.f, -1.0f));

                vertices[1].SetPosition(OVector3(right, bottom, 0.0f));  // Bottom right.
                vertices[1].SetTexture(OVector2(this->bottomRightU, this->bottomRightV));
                vertices[1].SetNormal(OVector3(0.f, 0.f, -1.0f));

                vertices[2].SetPosition(OVector3(left, bottom, 0.0f));  // Bottom left.
                vertices[2].SetTexture(OVector2(this->topLeftU, this->bottomRightV));
                vertices[2].SetNormal(OVector3(0.f, 0.f, -1.0f));

                // Triangle-2
                vertices[3].SetPosition(OVector3(left, top, 0.0f));  // Top left.
                vertices[3].SetTexture(OVector2(this->topLeftU, this->topLeftV));
                vertices[3].SetNormal(OVector3(0.f, 0.f, -1.0f));

                vertices[4].SetPosition(OVector3(right, top, 0.0f));  // Top right.
                vertices[4].SetTexture(OVector2(this->bottomRightU, this->topLeftV));
                vertices[4].SetNormal(OVector3(0.f, 0.f, -1.0f));

                vertices[5].SetPosition(OVector3(right, bottom, 0.0f));  // Bottom right.
                vertices[5].SetTexture(OVector2(this->bottomRightU, this->bottomRightV));
                vertices[5].SetNormal(OVector3(0.f, 0.f, -1.0f));

                // Lock the vertex buffer                
                hresult = dev->GetDeviceContext()->Map(dxMesh->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
                if(FAILED(hresult)){
                    LOG_ERROR("Vertexbuffer mapping failed - HRESULT: " << std::hex << hresult);
                    return;
                }

                // Copy data into vertex buffer.
                memcpy((OVertex*)mappedResource.pData, (void*)vertices, (sizeof(OVertex) * this->vertexCount));

                // Unlock the vertex buffer
                dev->GetDeviceContext()->Unmap(dxMesh->GetVertexBuffer(), 0);

                OS_SAFEDELETE_ARRAY(vertices);
            }
        }
    }
}