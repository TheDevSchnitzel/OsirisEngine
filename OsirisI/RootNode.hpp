#ifndef ROOTNODE_HPP_
#define ROOTNODE_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Renderer {
			class IRenderer;
		}
		namespace Scenes {
			class RootNode : public ISceneNode {
				private:
					OsirisI::Graphics::Renderer::IRenderer* renderer;

				public:
					RootNode();
					~RootNode();

					ReturnState Initialize();
					ReturnState Update(float delta);
					ReturnState BeginSceneRendering();
					ReturnState Recover();
					ReturnState EndSceneRendering();
					ReturnState Release();
					
					void SetRenderer(OsirisI::Graphics::Renderer::IRenderer* renderer);
					OsirisI::Graphics::Renderer::IRenderer* GetRenderer();
					
					std::vector<OsirisI::Graphics::Renderer::RenderInfo*> GetRenderInfos();
			};
		}
	}
}

#endif /* ROOTNODE_HPP_ */
