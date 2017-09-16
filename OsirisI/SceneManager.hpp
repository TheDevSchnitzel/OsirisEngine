#ifndef SCENEMANAGER_HPP_
#define SCENEMANAGER_HPP_

#include <map>
#include "IScene.hpp"
#include "DebugScene.hpp"
#include "IRenderer.hpp"

namespace OsirisI {
	namespace Manager {
		class SceneManager {
			private:
				static std::map<long, Graphics::Scenes::IScene*> scenes;
                static Graphics::Scenes::IScene* activeScene;
				static Debug::DebugScene* systemOverlayScene;
                static OsirisI::Graphics::Renderer::IRenderer* renderer;
                static std::mutex lock;

			public:
                static ReturnState Initialize(OsirisI::Graphics::Renderer::IRenderer* renderer);

				static ReturnState AddScene(Graphics::Scenes::IScene* scene);
				static ReturnState RemoveScene(Graphics::Scenes::IScene* scene);
				static ReturnState RemoveScene(long sceneID);

				//No need to deactivate Scenes -> View can not exist without scene therefore there is only the possibility of a swap/activation
				static ReturnState ActivateScene(Graphics::Scenes::IScene* scene);
				static ReturnState ActivateScene(long sceneID);

				static Debug::DebugScene* GetSystemOverlayScene();
				static void SetSystemOverlayScene(Debug::DebugScene* scene);

				static ReturnState Update(float delta);
				static ReturnState Render();
				static ReturnState Release();
		};
	}
}

#endif /* SCENEMANAGER_HPP_ */
