#include "stdafx.h"

using namespace OsirisI::Utilities;
using namespace OsirisI::Graphics::Devices;

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
			DX11Mesh::DX11Mesh(HashedString name) : IMesh(name) {
				this->indexBuffer = nullptr;
				this->vertexBuffer = nullptr;
			}

            DX11Mesh::~DX11Mesh() {
            }

			unsigned int DX11Mesh::GetIndicesCount() {
				return this->indexCount;
			}

			ID3D11Buffer* DX11Mesh::GetVertexBuffer() {
				return this->vertexBuffer;
			}

			ID3D11Buffer* DX11Mesh::GetIndexBuffer() {
				return this->indexBuffer;
			}

			void DX11Mesh::SetVertexBuffer(ID3D11Buffer* buffer) {
				this->vertexBuffer = buffer;
			}

			void DX11Mesh::SetIndexBuffer(ID3D11Buffer* buffer) {
				this->indexBuffer = buffer;
			}
			ReturnState DX11Mesh::Update(float delta) {
				return OS_OK;
			}
			ReturnState DX11Mesh::Recover() { 
				return OS_OK;
			}
			ReturnState DX11Mesh::Release() { 
				if(this->indexBuffer != nullptr) this->indexBuffer->Release();
				if(this->vertexBuffer != nullptr) this->vertexBuffer->Release();
				return OS_OK;
			}

            OVertex* DX11Mesh::GetVertices() {   
				OsirisI::Graphics::Devices::Direct3DDevice* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice);   
                D3D11_MAPPED_SUBRESOURCE mappedResource;          
                HRESULT hResult = dev->GetDeviceContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
                if(FAILED(hResult)){
                    LOG_ERROR("Vertexbuffer mapping failed - HRESULT: " << std::hex << hResult);
                    return nullptr;
                }


                OVertex* vertices = new OVertex[(sizeof(OVertex) * this->indexCount)];

                // Copy data from vertex buffer.
                memcpy((void*)vertices, (OVertex*)mappedResource.pData, (sizeof(OVertex) * this->indexCount));

                // Unlock the vertex buffer
                dev->GetDeviceContext()->Unmap(this->vertexBuffer, 0);

                return vertices;
            }

            ReturnState DX11Mesh::CreateVertexAndIndexBuffer(OVertex* vertices, unsigned long* indices, unsigned int vertexCount, unsigned int indexCount, bool dynamic) {
				OsirisI::Graphics::Devices::Direct3DDevice* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice);
                OS_CHECKSTATE(dev->CreateStandardVertexBuffer(&this->vertexBuffer, vertexCount, vertices, dynamic));
                OS_CHECKSTATE(dev->CreateStandardIndexBuffer(&this->indexBuffer, indexCount, indices));
                this->indexCount = indexCount;

                return OS_OK;
            }

			ReturnState DX11Mesh::Persist(std::ostream &stream) {
				stream << this->containerPath;
				return OS_OK;
			}
			
			ReturnState DX11Mesh::Unpersist(std::istream &stream) {
				return OS_OK;
			}
		}
	}
}
