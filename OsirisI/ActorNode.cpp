#include "stdafx.h"

using namespace OsirisI::Graphics::Renderer;
using namespace OsirisI::Utilities;

namespace OsirisI {
	namespace Graphics {
		namespace Scenes {
			ActorNode::ActorNode(OsirisI::Graphics::Actors::IActor* actor) {
                if (actor->GetName().GetString() == "") throw("Actor has no name");
                this->actor = actor;
			}
			ActorNode::~ActorNode() {
			}

			ReturnState ActorNode::Initialize() {
                OS_CHECKSTATE(actor->Initialize());
                return OS_OK;
			}
			ReturnState ActorNode::Update(float delta) {
				return actor->Update(delta);
			}
			ReturnState ActorNode::Recover() {
				return actor->Recover();
			}
			ReturnState ActorNode::Release() {
				return actor->Release();
			}
                        
			std::vector<RenderInfo*> ActorNode::GetRenderInfos() {                
                std::vector<RenderInfo*> vecRenderInfo;
                vecRenderInfo.push_back(actor->GetRenderInfo());
                return vecRenderInfo;
			}
			
			OsirisI::Graphics::Actors::IActor* ActorNode::GetActor() {
				return this->actor;
			}
		}
	}
}