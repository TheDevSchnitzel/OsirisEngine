#ifndef COMPONENTNODE_HPP_
#define COMPONENTNODE_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Scenes {
			class ComponentNode : public ISceneNode {
				private:
					OsirisI::Graphics::Components::IComponent* component;

				public:
					ComponentNode(OsirisI::Graphics::Components::IComponent* component);
					~ComponentNode();

					ReturnState Initialize();
					ReturnState Update(float delta);
					ReturnState Recover();
					ReturnState Release();
                    std::vector<OsirisI::Graphics::Renderer::RenderInfo*> GetRenderInfos();
			};
		}
	}
}

#endif /*COMPONENTNODE_HPP_*/
