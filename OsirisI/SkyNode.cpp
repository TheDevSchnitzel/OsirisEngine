#include "stdafx.h"

using namespace OsirisI::Graphics::Renderer;
using namespace OsirisI::Graphics::Actors;

namespace OsirisI {
	namespace Graphics {
		namespace Scenes {
			SkyNode::SkyNode() {
			}
			SkyNode::~SkyNode() {
			}

			ReturnState SkyNode::Initialize() {
                unsigned int vertexCount = 36;
                OVertex* vertices = new OVertex[vertexCount];	
				memset(vertices, 0, (sizeof(OVertex) * vertexCount));

				unsigned long* indices = new unsigned long[vertexCount];
				for(unsigned int i = 0; i < vertexCount; i++) indices[i] = i;
                
                DX11Mesh* dx11Mesh = new DX11Mesh(name);
                OS_CHECKSTATE(dx11Mesh->CreateVertexAndIndexBuffer(vertices, indices, vertexCount, vertexCount, true));

				OS_SAFEDELETE_ARRAY(vertices);
				OS_SAFEDELETE_ARRAY(indices);
				return OS_OK;
			}
            
            ReturnState SkyNode::Update(float delta) {
                this->ISceneNode::Update(delta);
				return OS_OK;
			}
			
            ReturnState SkyNode::Recover() {
				return skyBox->Recover();
			}

			ReturnState SkyNode::Release() {
				return skyBox->Release();
			}
            
			std::vector<RenderInfo*> SkyNode::GetRenderInfos() {
                std::vector<RenderInfo*> renderInfos;
                renderInfos.push_back(skyBox->GetRenderInfo());
                return renderInfos;
            }
		}
	}
}
