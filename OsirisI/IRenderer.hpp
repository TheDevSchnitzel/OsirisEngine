#ifndef IRENDERER_HPP_
#define IRENDERER_HPP_

namespace OsirisI {
	struct OMatrix4x4;
	struct OVector3;
	namespace Graphics {
		namespace Renderer {
			struct RenderInfo;
			class IRenderer {
				protected:
                    OsirisI::Graphics::Scenes::ISceneNode* rootNode;
			        OsirisI::Graphics::Scenes::ISceneNode* cameraNode;     
                    OMatrix4x4 baseViewMatrix;

				public:
					virtual ReturnState BeginRendering() = 0;
					virtual ReturnState Render(RenderInfo* info) = 0;
					virtual ReturnState EndRendering() = 0;
					virtual ReturnState TransformCamera(OVector3* rotation, OVector3* lookAt, OVector3* up, OMatrix4x4* viewMatrix, OVector3* position) = 0;
			        virtual ReturnState RenderCamera() = 0;

                    void SetRootNode(OsirisI::Graphics::Scenes::ISceneNode* rootNode) {
                        this->rootNode = rootNode;
                    }
                    void SetCameraNode(OsirisI::Graphics::Scenes::ISceneNode* cameraNode) {
                        this->cameraNode = cameraNode;
                    }
                    OsirisI::Graphics::Scenes::ISceneNode* GetCameraNode() {
                        return this->cameraNode;
                    }

                    void GetBaseViewMatrix(OsirisI::OMatrix4x4& BaseViewMatrix) {
                        BaseViewMatrix = baseViewMatrix;
                    }

                    void SetBaseViewMatrix(OsirisI::OMatrix4x4 BaseViewMatrix) {
                        baseViewMatrix = BaseViewMatrix;
                    }

            };
		}
	}
}

#endif /* IRENDERER_HPP_ */
