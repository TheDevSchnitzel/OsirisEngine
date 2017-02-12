#include "stdafx.h"

using namespace OsirisI::Graphics::Renderer;

namespace OsirisI {
	namespace Graphics {
		namespace Scenes {
			RootNode::RootNode() {
			}
			RootNode::~RootNode() {
			}

			ReturnState RootNode::Initialize() {
				return OS_OK;
			}

			ReturnState RootNode::BeginSceneRendering() {
				return this->renderer->BeginRendering();
			}
            ReturnState RootNode::Update(float delta) {
                this->ISceneNode::Update(delta);
				return OS_OK;
			}
			ReturnState RootNode::Recover() {
				return OS_OK;
			}
			ReturnState RootNode::EndSceneRendering() {		
				return this->renderer->EndRendering();
			}
			ReturnState RootNode::Release() {
				return OS_OK;
			}

			void RootNode::SetRenderer(IRenderer* renderer) {
				this->renderer = renderer;
			}
			IRenderer* RootNode::GetRenderer() {
				return this->renderer;
			}

			std::vector<RenderInfo*> RootNode::GetRenderInfos() {
                std::vector<RenderInfo*> renderInfos;
                for(unsigned int i = 0; i < childNodes.size(); i++) {
                    std::vector<RenderInfo*> childRI = childNodes[i]->GetRenderInfos();
                    renderInfos.insert(renderInfos.end(), childRI.begin(), childRI.end());                        
                }
                return renderInfos;
            }
		}
	}
}
