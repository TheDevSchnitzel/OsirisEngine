#ifndef CAMERAACTOR_HPP_
#define CAMERAACTOR_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Renderer {
			class IRenderer;
		}

		namespace Actors {
			class CameraActor : public MovingActor {
				private:
					OsirisI::OMatrix4x4 viewMatrix;
					OsirisI::Graphics::Renderer::IRenderer* renderer;

				protected:

				public:
					CameraActor(OsirisI::Utilities::HashedString name);
					~CameraActor();

					ReturnState Initialize();
					ReturnState Update(float delta);
					ReturnState Recover();
					ReturnState Release();
					OsirisI::Graphics::Renderer::RenderInfo* GetRenderInfo();
			
					void GetViewMatrix(OsirisI::OMatrix4x4&);
					void SetViewMatrix(OsirisI::OMatrix4x4 viewMatrix);

					void SetRenderer(OsirisI::Graphics::Renderer::IRenderer* renderer);
					OsirisI::Graphics::Renderer::IRenderer* GetRenderer();
			};
		}
	}
}

#endif /* CAMERAACTOR_HPP_ */