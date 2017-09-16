#ifndef DEBUGSCENE_HPP_
#define DEBUGSCENE_HPP_

#include <map>
#include "IScene.hpp"

namespace OsirisI {
	namespace Debug {
		class DebugScene : public  OsirisI::Graphics::Scenes::IScene{
		private:
			std::vector<std::string> texts;
			bool isVisible;

		public:
			DebugScene(OsirisI::Utilities::HashedString name);
			~DebugScene();

			OsirisI::Graphics::Actors::IText* AddText(std::string text);
			void ToggleVisibility();
			
			OsirisI::Graphics::Scenes::RootNode* GetRootNode();

			ReturnState PrepareForActivation();
			ReturnState Update(float delta);
			ReturnState Recover();
			ReturnState Release();
		};
	}
}

#endif /* DEBUGSCENE_HPP_ */
