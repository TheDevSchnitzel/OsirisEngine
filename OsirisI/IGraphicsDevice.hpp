#ifndef IGRAPHICSDEVICE_HPP_
#define IGRAPHICSDEVICE_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Devices {
			class IWindow;
            enum GraphicsDeviceType {DirectX11, OpenGL};
			class IGraphicsDevice {
				protected:
					IWindow* window;
                    static OsirisI::OMatrix4x4 WorldMatrix, ProjectionMatrix, OrthoMatrix;
					int width, height;

				public:
					static IGraphicsDevice *g_graphicDevice; //Needed for multiple purposes

					virtual ~IGraphicsDevice() {}
					virtual ReturnState Release() = 0;

                    virtual GraphicsDeviceType GetDeviceType() = 0;

					IWindow* GetWindow() {
						return this->window;
					}

                    int GetWidth() {
                        return this->width;
                    }
                    int GetHeight() {
                        return this->height;
                    }

                    static void GetProjectionMatrix(OsirisI::OMatrix4x4& projectionMatrix) {
                        projectionMatrix = ProjectionMatrix;
                    }

                    static void GetWorldMatrix(OsirisI::OMatrix4x4& worldMatrix) {
                        worldMatrix = WorldMatrix;
                    }

                    static void GetOrthoMatrix(OsirisI::OMatrix4x4& orthoMatrix) {
                        orthoMatrix = OrthoMatrix;
                    }

                    static void SetProjectionMatrix(OsirisI::OMatrix4x4 projectionMatrix) {
                        ProjectionMatrix = projectionMatrix;
                    }

                    static void SetWorldMatrix(OsirisI::OMatrix4x4 worldMatrix) {
                        WorldMatrix = worldMatrix;
                    }

                    static void SetOrthoMatrix(OsirisI::OMatrix4x4 orthoMatrix) {
                        OrthoMatrix = orthoMatrix;
                    }
            };
		}
	}
}

#endif /* IGRAPHICSDEVICE_HPP_ */
