#ifndef DX11MESH_HPP_
#define DX11MESH_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Renderer {
			struct RenderInfo;
		}
		namespace Actors {
			class DX11Mesh : public IMesh {
				private:
					ID3D11Buffer *vertexBuffer, *indexBuffer;
					unsigned long indexCount;

				public:
					DX11Mesh(OsirisI::Utilities::HashedString name);
					~DX11Mesh();
                    
                    OsirisI::ReturnState CreateVertexAndIndexBuffer(OsirisI::OVertex* vertices, unsigned long* indices, unsigned int vertexCount, unsigned int indexCount, bool dynamic = false);

					unsigned int GetIndicesCount();				
					ID3D11Buffer* GetVertexBuffer();
					ID3D11Buffer* GetIndexBuffer();
					void SetVertexBuffer(ID3D11Buffer* buffer);
					void SetIndexBuffer(ID3D11Buffer* buffer);
					                    
                    OsirisI::OVertex* GetVertices();

                    OsirisI::ReturnState Initialize();
                    OsirisI::ReturnState Update(float delta);
                    OsirisI::ReturnState Recover();
                    OsirisI::ReturnState Release();
					OsirisI::Graphics::Renderer::RenderInfo GetRenderInfo();
										
					OsirisI::ReturnState Persist(std::ostream &stream);
					OsirisI::ReturnState Unpersist(std::istream &stream);
			};
		}
	}
}

#endif /* DX11MESH_HPP_ */
