#include "stdafx.h"

namespace OsirisI {
	namespace Graphics {
		namespace Scenes {
			RootNode* IScene::GetRootNode() {
				return rootNode;
			}
			void IScene::SetRootNode(RootNode* node) {
				this->rootNode = node;
			}

			ActorNode* IScene::GetCameraNode() {
				return cameraNode;
			}
			void IScene::SetCameraNode(ActorNode* node) {
				this->rootNode->AddChildNode(node);
				this->cameraNode = node;
			}
		}
	}
}
