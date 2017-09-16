#include "stdafx.h"

using namespace OsirisI::Graphics::Devices;
using namespace OsirisI::Graphics;
using namespace OsirisI::Resources;
using namespace OsirisI::Utilities;
using namespace OsirisI::Manager;
using namespace OsirisI::Graphics::Renderer;
using namespace OsirisI::Graphics::Shader;

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
			DX11Text::DX11Text(OVector3 pos, OsirisI::Utilities::HashedString name, OsirisI::Utilities::HashedString fontContainerPath, std::string text) : IText(name) {
				this->texture = nullptr;
				this->repeat = false;
				this->text = text;
				this->font = nullptr;
				this->gameModel = new DX11GameModel(pos, name);
				this->containerPath = fontContainerPath;
				this->color = OVector4(0.f, 255.f, 0.f, 0.f);
				positionX = static_cast<unsigned int>(pos.x), positionY = static_cast<unsigned int>(pos.y), prevPositionX = 99999999, prevPositionY = 99999999;
				topLeftU = 0.0f, topLeftV = 0.0f, bottomRightU = 1.0f, bottomRightV = 1.0f, vertexCount = 0;
				this->size = OVector2(0.f, 0.f);
			}

			DX11Text::~DX11Text() {
				Release();
			}

			ReturnState DX11Text::Initialize() {
				Direct3DDevice* dev = ((Direct3DDevice*)IGraphicsDevice::g_graphicDevice);

				if (this->font == nullptr) {
					OS_CHECKSTATE(CreateMesh());

					ResourcePtr<Font*>* res = ((ResourcePtr<Font*>*)ResourceManager::GetResource<Font>(this->containerPath));
					if (res == nullptr) {
						res = ((ResourcePtr<Font*>*)ResourceManager::LoadAndCache<Font>(HashedString(this->containerPath), HashedString(name)));
						if (res == nullptr) {
							return OS_INITIALIZATION_FAILED;
						}
					}
					this->font = res->GetValue();

					ShaderInfoFactory::AddTexture(this->font->GetFontTexture());
					ShaderInfoFactory::AddCustomStruct((char*)&this->color, sizeof(OVector4));
					ShaderInfoFactory::SetAffectedShader(HashedString("TextShader").GetHash());
					ShaderInfoFactory::SetAffectedShaderType(DX11_PIXEL_SHADER);
					RenderInfoFactory::AddShaderInfo(ShaderInfoFactory::CreateShaderInformation());
					RenderInfoFactory::AddActor(this);
					RenderInfoFactory::TurnZBufferOff(true);
					RenderInfoFactory::TurnAlphaBlendingOff(false);
					RenderInfoFactory::IsText(true);
					this->renderInfo = RenderInfoFactory::CreateRenderInformation();

					LOG_DEBUG("DX11Text at: " << std::hex << this << " initialized");
				}

				//Perform an update on the vertex positions
				OS_CHECKSTATE(UpdateVerticesPositions());
				return OS_OK;
			}

			ReturnState DX11Text::CreateMesh()
			{
				if (this->gameModel->GetMeshes().size() == 1) {
					DX11Mesh* mesh = reinterpret_cast<DX11Mesh*>(this->gameModel->GetMeshes()[0]);
					this->gameModel->RemoveMesh(name.GetHash());
					mesh->Release();
					OS_SAFEDELETE(mesh);
				}

				this->vertexCount = 6 * text.length(), this->indexCount = 6 * text.length();

				OVertex* vertices = new OVertex[vertexCount];
				memset(vertices, 0, (sizeof(OVertex)* vertexCount));

				unsigned long* indices = new unsigned long[indexCount];
				for (unsigned int i = 0; i < indexCount; i++) indices[i] = i;

				DX11Mesh* dx11Mesh = new DX11Mesh(name);
				OS_CHECKSTATE(dx11Mesh->CreateVertexAndIndexBuffer(vertices, indices, vertexCount, indexCount, true));

				OS_SAFEDELETE_ARRAY(vertices);
				OS_SAFEDELETE_ARRAY(indices);

				this->gameModel->AddMesh(dx11Mesh);
				return OS_OK;
			}

			ReturnState DX11Text::UpdateVerticesPositions() {
				OsirisI::Graphics::Devices::Direct3DDevice* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice);

#pragma region Build vertices	
				//Calculate drawing position
				int tmpX = ((dev->GetWidth() / 2) * -1) + this->positionX;
				float drawPosX = static_cast<float>(tmpX);
				float drawPosY = static_cast<float>((dev->GetHeight() / 2) - this->positionY);


				OVertex* vertices = new OVertex[this->vertexCount];
				memset(vertices, 0, (sizeof(OVertex)* this->vertexCount)); //Init vertices with zeros	


				// Get the number of letters in the sentence.
				unsigned int numLetters = this->text.length();
				unsigned int index = 0, character = 0, maxCharHeight = 0;


				for (unsigned int i = 0; i < 256; i++) {
					if (this->font->GetCoordinatesUV(i).TexHeight > maxCharHeight) maxCharHeight = static_cast<unsigned int>(this->font->GetCoordinatesUV(i).TexHeight);
				}

				// Draw each letter onto a quad.
				for (unsigned int i = 0; i < numLetters; i++) {
					character = (static_cast<unsigned int>(this->text.c_str()[i]));

					// If the letter is a space then just move over three pixels.
					if (character == 0)	drawPosX = drawPosX + 3.0f;
					else {
						float drawPosYtmp = ((this->font->GetCoordinatesUV(character).TexHeight == maxCharHeight) ? drawPosY : drawPosY - (maxCharHeight - this->font->GetCoordinatesUV(character).TexHeight));
						//std::cout << this->font->GetCoordinatesUV(character).TexHeight << "-" << maxCharHeight << "-" << drawPosY << std::endl;
						// First triangle in quad.
						vertices[index].SetPosition(OVector3(drawPosX, drawPosYtmp, 0.0f));  // Top left.
						vertices[index].SetTexture(OVector2(this->font->GetCoordinatesUV(character).LeftUpU, this->font->GetCoordinatesUV(character).LeftUpV));
						vertices[index].SetNormal(OVector3(0.f, 0.f, -1.0f));
						index++;

						vertices[index].SetPosition(OVector3((drawPosX + this->font->GetCoordinatesUV(character).TexLength), (drawPosY - maxCharHeight), 0.0f));  // Bottom right.
						vertices[index].SetTexture(OVector2(this->font->GetCoordinatesUV(character).RightDownU, this->font->GetCoordinatesUV(character).RightDownV));
						vertices[index].SetNormal(OVector3(0.f, 0.f, -1.0f));
						index++;

						vertices[index].SetPosition(OVector3(drawPosX, (drawPosY - maxCharHeight), 0.0f));  // Bottom left.
						vertices[index].SetTexture(OVector2(this->font->GetCoordinatesUV(character).LeftUpU, this->font->GetCoordinatesUV(character).RightDownV));
						vertices[index].SetNormal(OVector3(0.f, 0.f, -1.0f));
						index++;

						// Second triangle in quad.
						vertices[index].SetPosition(OVector3(drawPosX, drawPosYtmp, 0.0f));  // Top left.
						vertices[index].SetTexture(OVector2(this->font->GetCoordinatesUV(character).LeftUpU, this->font->GetCoordinatesUV(character).LeftUpV));
						vertices[index].SetNormal(OVector3(0.f, 0.f, -1.0f));
						index++;

						vertices[index].SetPosition(OVector3(drawPosX + this->font->GetCoordinatesUV(character).TexLength, drawPosYtmp, 0.0f));  // Top right.
						vertices[index].SetTexture(OVector2(this->font->GetCoordinatesUV(character).RightDownU, this->font->GetCoordinatesUV(character).LeftUpV));
						vertices[index].SetNormal(OVector3(0.f, 0.f, -1.0f));
						index++;

						vertices[index].SetPosition(OVector3((drawPosX + this->font->GetCoordinatesUV(character).TexLength), (drawPosY - maxCharHeight), 0.0f));  // Bottom right.
						vertices[index].SetTexture(OVector2(this->font->GetCoordinatesUV(character).RightDownU, this->font->GetCoordinatesUV(character).RightDownV));
						vertices[index].SetNormal(OVector3(0.f, 0.f, -1.0f));
						index++;

						// Update the x location for drawing by the size of the letter and one pixel.
						drawPosX = drawPosX + this->font->GetCoordinatesUV(character).TexLength + 1.0f;
					}
				}
#pragma endregion

				// Lock the vertex buffer so it can be written to.
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				ID3D11Buffer* vertexBuffer = ((DX11Mesh*)this->gameModel->GetMeshes()[0])->GetVertexBuffer();
				HRESULT result = dev->GetDeviceContext()->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				ReturnState retState = OS_OK;
				if (SUCCEEDED(result)) {

					// Get a pointer to the data in the vertex buffer.
					OVertex* verticesPtr = reinterpret_cast<OVertex*>(mappedResource.pData);

					// Copy the data into the vertex buffer.
					int size = (sizeof(OVertex)* this->vertexCount);
					memcpy_s(verticesPtr, size, vertices, size);
				}
				else {
					LOG_ERROR("ResourceMapping failed - HRESULT: " << std::hex << result);
					retState = OS_RESOURCEMAPPING_FAILED;
				}

				// Unlock the vertex buffer.
				dev->GetDeviceContext()->Unmap(vertexBuffer, 0);

				// Release the vertex array as it is no longer needed.
				OS_SAFEDELETE_ARRAY(vertices);

				return retState;
			}

			ReturnState DX11Text::Update(float delta) {
				if (this->text.compare(this->oldText) != 0) {
					if (this->text.length() != this->oldText.length()) {
						OS_CHECKSTATE(CreateMesh());
					}

					OS_CHECKSTATE(UpdateVerticesPositions());
					this->oldText = text;
				}

				return OS_OK;
			}

			ReturnState DX11Text::Recover() {
				return OS_OK;
			}

			ReturnState DX11Text::Release() {
				return OS_OK;
			}

			OsirisI::Graphics::Renderer::RenderInfo* DX11Text::GetRenderInfo() {
				return this->renderInfo;
			}
		}
	}
}






