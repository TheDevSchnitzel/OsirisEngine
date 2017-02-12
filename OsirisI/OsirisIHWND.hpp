#ifndef OSIRISIHWND_HPP_
#define OSIRISIHWND_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Devices {
			class OsirisIHWND : public IWindow {
				private: 
					HWND hWnd;
					WNDCLASSEXA wndClassEx;
					WNDPROC wndProc;
					HINSTANCE hInstance;

				public:
					OsirisIHWND(int width, int height, bool windowed);
					~OsirisIHWND(); 

					ReturnState Initialize();
					ReturnState Release();

					HWND GetHWND() {
						return this->hWnd; 
					}
					void SetHWND(HWND hWnd) {
						this->hWnd = hWnd;
					}

					WNDCLASSEXA* GetWNDCLASSEXA() { 
						return &wndClassEx;
					}
					void SetWNDCLASSEXA(WNDCLASSEXA wndClassEx) {
						this->wndClassEx = wndClassEx;
					}

					HINSTANCE GetHINSTANCE() { 
						return hInstance;
					}
					void SetHINSTANCE(HINSTANCE hInstance) {
						this->hInstance = hInstance;
					} 

					WNDPROC GetWNDPROC() { 
						return this->wndProc;
					}
					void SetWNDPROC(WNDPROC wndProc) {
						this->wndProc = wndProc;
					} 
			};
		}
	}
}

#endif /* OSIRISIHWND_HPP_ */
