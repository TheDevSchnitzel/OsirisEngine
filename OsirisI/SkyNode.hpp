#ifndef SKYNODE_HPP_
#define SKYNODE_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Scenes {
			class SkyNode : public ISceneNode {
				private:
					OsirisI::Graphics::Actors::IGameModel* skyBox;

				public:
					SkyNode();
					~SkyNode();

					ReturnState Initialize();
					ReturnState Update(float delta);
					ReturnState Recover();
					ReturnState Release();
										
					std::vector<OsirisI::Graphics::Renderer::RenderInfo*> GetRenderInfos();
			};
		}
	}
}

#endif /* SKYNODE_HPP_ */
