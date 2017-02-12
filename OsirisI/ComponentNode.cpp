#include "stdafx.h"

using namespace OsirisI::Graphics::Renderer;
using namespace OsirisI::Utilities;

namespace OsirisI {
	namespace Graphics {
		namespace Scenes {
			ComponentNode::ComponentNode(OsirisI::Graphics::Components::IComponent* component) {
                if (component->GetName().GetString() == "") throw("Component has no name");
                this->component = component;
			}
			ComponentNode::~ComponentNode() {
			}

			ReturnState ComponentNode::Initialize() {
                OS_CHECKSTATE(component->Initialize());
                return OS_OK;
			}
			ReturnState ComponentNode::Update(float delta) {
				return component->Update(delta);
			}
			ReturnState ComponentNode::Recover() {
				return component->Recover();
			}
			ReturnState ComponentNode::Release() {
				return component->Release();
			}
                        
			std::vector<RenderInfo*> ComponentNode::GetRenderInfos() {                
                std::vector<RenderInfo*> vecRenderInfo;
                vecRenderInfo.push_back(component->GetRenderInfo());
                return vecRenderInfo;
			}
		}
	}
}