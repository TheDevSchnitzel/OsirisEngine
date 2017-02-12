#ifndef IWINDOW_HPP_
#define IWINDOW_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Devices {
			class IWindow {
				private:
					std::string windowName; 
					int width, height;
					bool windowed;

				public:
					std::string GetWindowName() { 
						return windowName;
					}
					void SetWindowName(std::string windowName) {
						this->windowName = windowName;
					}
		
					int GetWidth() {
						return this->width;
					}
					void SetWidth(int width) {
						this->width = width;
					}

					int	GetHeight() {
						return this->height;
					}
					void SetHeight(int height) {
						this->height = height;
					}	

					bool isWindowed() {
						return this->windowed;
					}
					void SetWindowed(bool windowed) {
						this->windowed = windowed;
					}	

					virtual ReturnState Initialize() = 0;
					virtual ReturnState Release() = 0;
			};
		}
	}
}

#endif /* IWINDOW_HPP_ */