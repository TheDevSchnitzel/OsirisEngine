#ifndef DIRECTX11RENDERER_HPP_
#define DIRECTX11RENDERER_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Renderer {
			class DirectX11Renderer : public IRenderer {
				private:
					OsirisI::Graphics::Devices::Direct3DDevice* direct3DDevice;

				public:
					ReturnState BeginRendering();
					ReturnState Render(RenderInfo* info);
					ReturnState EndRendering();
					ReturnState TransformCamera(OVector3* rotation, OVector3* lookAt, OVector3* up, OMatrix4x4* viewMatrix, OVector3* position);
					OMatrix4x4 DX3DMatrixPositionTransformAndRotationYawPitchRoll(OMatrix4x4 worldMatrix, OVector3 position, OVector3 rotationSpeed);

                    ReturnState RenderCamera();

					void SetDirect3DDevice(OsirisI::Graphics::Devices::Direct3DDevice* direct3DDevice);
					OsirisI::Graphics::Devices::Direct3DDevice* GetDirect3DDevice();
			};
		}
	}
}

#endif /* DIRECTX11RENDERER_HPP_ */
