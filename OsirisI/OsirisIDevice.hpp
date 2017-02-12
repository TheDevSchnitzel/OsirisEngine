#ifndef OSIRISDEVICE_HPP_
#define OSIRISDEVICE_HPP_

namespace OsirisI {
	class OsirisIDevice {
		private:
			Graphics::Renderer::IRenderer *renderer;
			Graphics::Devices::IGraphicsDevice *graphicDevice;
            Networking::OsirisI_Server *server;

			bool vsync, windowed;
			int width, height;

			static bool stopMainLoop;

		public:
			OsirisIDevice(int width, int height, bool windowed, bool vsync = false);
			~OsirisIDevice();
		
			/** Device initialization.
				Initialization for a DirectX - Device 
			*/
			bool InitializeDX(std::string windowName);

			/** Device initialization.
				Initialization for a OpenGL - Device 
			*/
			bool InitializeGL(std::string windowName);
			bool Run();
			bool Recover();
			bool Release();

			static OsirisI_DeviceType DeviceType;
			static OsirisIDevice* g_OsirisIDevice;
			std::mutex lock;

		private:
			bool SetupInternal();
	};
}

#endif /* OSIRISDEVICE_HPP_ */
