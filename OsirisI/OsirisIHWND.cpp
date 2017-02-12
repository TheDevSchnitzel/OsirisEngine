#include "stdafx.h"

using namespace OsirisI::Utilities;

namespace OsirisI {
	namespace Graphics {
		namespace Devices {
			OsirisIHWND::OsirisIHWND(int width, int height, bool windowed) {
				this->SetWidth(width);
				this->SetHeight(height);
				this->SetWindowed(windowed);
				this->hWnd = nullptr;
			}

			OsirisIHWND::~OsirisIHWND() {
			}

			ReturnState OsirisIHWND::Initialize() {		
				WNDCLASSEXA wndClassEx;
				ZeroMemory(&wndClassEx, sizeof(WNDCLASSEXA));		
				wndClassEx.cbSize = sizeof(WNDCLASSEXA);
				wndClassEx.style = CS_HREDRAW | CS_VREDRAW;
				wndClassEx.lpfnWndProc = wndProc;
				wndClassEx.hInstance = GetModuleHandle(0);
				wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
				wndClassEx.hbrBackground = (HBRUSH)COLOR_WINDOW;
				std::string str1 = this->GetWindowName();
                wndClassEx.lpszClassName = LPCSTR("SpaceLegends"); //str1.c_str();
				this->SetWNDCLASSEXA(wndClassEx);
				this->hInstance = wndClassEx.hInstance;

				SetLastError(0);
				RegisterClassExA(this->GetWNDCLASSEXA());
				DWORD error = GetLastError();
				if(error != 0) {
					LOG_ERROR("RegisterClassExA failed - DWORD_CODE: " << error);
					return 0xA;
				}

                unsigned int posX = 0, posY = 0;
                if (!this->isWindowed()) {
                    DEVMODE dmScreenSettings;
                    // If full screen set the screen to maximum size of the users desktop and 32bit.
                    memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
                    dmScreenSettings.dmSize = sizeof(dmScreenSettings);
                    dmScreenSettings.dmPelsWidth = (unsigned long)this->GetWidth();
                    dmScreenSettings.dmPelsHeight = (unsigned long)this->GetHeight();
                    dmScreenSettings.dmBitsPerPel = 32;
                    dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

                    // Change the display settings to full screen.
                    ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
                }
                else {
                    posX = (GetSystemMetrics(SM_CXSCREEN) - this->GetWidth())  / 2;
                    posY = (GetSystemMetrics(SM_CYSCREEN) - this->GetHeight()) / 2;
                }

                /*RECT wr = { 0, 0, this->GetWidth(), this->GetHeight() };
                AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
                error = GetLastError();
                if (error != 0) {
                    LOG_ERROR("AdjustWindowRect failed - DWORD_CODE: " << error);
                    return 0xB;
                }*/
                
                this->hWnd = CreateWindowExA(WS_EX_APPWINDOW, LPCSTR("SpaceLegends"), LPCSTR("SpaceLegends"), WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, this->GetWidth(), this->GetHeight(), NULL, NULL, this->hInstance, NULL);
				error = GetLastError(); 
				if(this->hWnd == 0) {
					LOG_ERROR("CreateWindowExA failed - DWORD_CODE: " << error);
					return 0xC;
				}
				SetLastError(0);


				int nCmdShow = SW_SHOW;
				ShowWindow(this->hWnd, nCmdShow);
				error = GetLastError();
				if(error != 0) {
					LOG_ERROR("ShowWindow failed - DWORD_CODE: " << error);
					return 0xD;
				}

				UpdateWindow(this->hWnd);
				error = GetLastError();
				if(error != 0) {
					LOG_ERROR("UpdateWindow failed - DWORD_CODE: " << error);
					return 0xE;
				}

				SetFocus(this->hWnd);
				error = GetLastError();
				if(error != 0) {
					LOG_ERROR("SetFocus failed - DWORD_CODE: " << error);
					return 0xF;
				}

				return OS_OK;
			}

			ReturnState OsirisIHWND::Release() {
				return OS_OK;
			}
		}
	}
}
