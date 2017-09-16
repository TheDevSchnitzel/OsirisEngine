#include "stdafx.h"

using namespace OsirisI;
using namespace OsirisI::Utilities;
using namespace OsirisI::Graphics::Actors;
using namespace OsirisI::Graphics::Scenes;

namespace OsirisI {
	namespace Debug {

		DebugScene::DebugScene(HashedString name) : IScene(name) {
			SetRootNode(new RootNode());
			isVisible = false;
		}

		DebugScene::~DebugScene() {
		}

		OsirisI::Graphics::Actors::IText* DebugScene::AddText(std::string text) {
			texts.push_back(text);

			int position = texts.size() - 1;
			DX11Text* textTest = new DX11Text(OVector3(0, position * 20, 0), HashedString("DBGText"), HashedString("mainRes:Font/Consolas_17.OIFont"), text);

			textTest->SetColor(OVector4(0, 120, 255, 1));
			OS_CHECKSTATE_LOG(textTest->Initialize());
			
			rootNode->AddChildNode(new ActorNode(textTest));
			return textTest;
		}

		void DebugScene::ToggleVisibility() 
		{
			isVisible = !isVisible;
		}


		RootNode* DebugScene::GetRootNode()
		{
			return isVisible ? rootNode : nullptr;
		}

		ReturnState DebugScene::PrepareForActivation() {
			return OS_OK;
		}

		ReturnState DebugScene::Update(float delta) {
			if (rootNode != nullptr) {
				OS_CHECKSTATE(rootNode->Update(delta));
			}

			if (cameraNode != nullptr) {
				OS_CHECKSTATE(cameraNode->Update(delta));
			}
		}

		ReturnState DebugScene::Recover() {
			return OS_OK;
		}

		ReturnState DebugScene::Release() {
			return OS_OK;
		}

	}
}
