#ifndef ACTORNODE_HPP_
#define ACTORNODE_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Scenes {
			class ActorNode : public ISceneNode {
				private:
					OsirisI::Graphics::Actors::IActor* actor;

				public:
					ActorNode(OsirisI::Graphics::Actors::IActor* actor);
					~ActorNode();

					ReturnState Initialize();
					ReturnState Update(float delta);
					ReturnState Recover();
					ReturnState Release();
                    std::vector<OsirisI::Graphics::Renderer::RenderInfo*> GetRenderInfos();

					OsirisI::Graphics::Actors::IActor* GetActor();
			};
		}
	}
}

#endif /*ACTORNODE_HPP_*/
