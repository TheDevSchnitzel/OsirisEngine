#include "stdafx.h"

using namespace OsirisI::Graphics::Renderer;
using namespace OsirisI::Utilities;

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
			CameraActor::CameraActor(HashedString name) : MovingActor(name) {
				position = OVector3(0.f, 0.f, 0.f);
				rotation = OVector3(0.f, 0.f, 0.f);
				this->renderInfo = nullptr;
				this->gameModel = nullptr;
			}

			CameraActor::~CameraActor() {
			}

			ReturnState CameraActor::Initialize() {
				return OS_OK;
			}

			ReturnState CameraActor::Update(float delta) {				
				return MovingActor::Update(delta);
			}

			ReturnState CameraActor::Recover() {
				return OS_OK;
			}

			ReturnState CameraActor::Release() {
				return OS_OK;
			}		

			void CameraActor::GetViewMatrix(OMatrix4x4& viewMatrix) {
				viewMatrix = this->viewMatrix;
			}

			void CameraActor::SetViewMatrix(OMatrix4x4 viewMatrix) {
				this->viewMatrix = viewMatrix;
			}

			void CameraActor::SetRenderer(OsirisI::Graphics::Renderer::IRenderer* renderer) {
				this->renderer = renderer;
			}

			OsirisI::Graphics::Renderer::IRenderer* CameraActor::GetRenderer() {
				return this->renderer;
			}

			RenderInfo* CameraActor::GetRenderInfo() {
				return this->renderInfo;
			}
		}
	}
}

