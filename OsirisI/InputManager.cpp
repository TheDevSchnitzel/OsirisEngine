#include "stdafx.h"

using namespace OsirisI::Events;
using namespace OsirisI::Graphics::Actors;
using namespace OsirisI::Graphics::Devices;
using namespace OsirisI::Utilities;

namespace OsirisI {
	namespace Manager {
#ifdef WINDOWS
		IDirectInputDevice8* InputManager::didKeyboard = nullptr;
		IDirectInputDevice8* InputManager::didMouse = nullptr;
		DIMOUSESTATE InputManager::lastMouseState;
		LPDIRECTINPUT8 InputManager::DirectInput;
#endif

		float InputManager::rotx = 0;
		float InputManager::rotz = 0;
		float InputManager::scaleX = 1.f;
		float InputManager::scaleY = 1.f;
		OMatrix4x4 InputManager::Rotationx;
		OMatrix4x4 InputManager::Rotationz;
		std::unordered_map<OsirisI::KeyboardKeys, float> InputManager::keybardKeyPressedSince;
		std::unordered_map<OsirisI::MouseButtons, float> InputManager::mouseKeyPressedSince;


		std::stack<OsirisI::Graphics::Actors::IActor*> InputManager::inputReactiveActors;

		ReturnState InputManager::Initialize()
		{
#ifdef WINDOWS
			OsirisIHWND* hwnd = ((OsirisIHWND*)((Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetWindow());
			HRESULT hr = DirectInput8Create(hwnd->GetHINSTANCE(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, NULL);
			if (FAILED(hr)) {
				LOG_ERROR("DirectInput8Create - HR: " << std::hex << hr);
			}

			hr = DirectInput->CreateDevice(GUID_SysKeyboard, &didKeyboard, NULL);
			if (FAILED(hr)) {
				LOG_ERROR("DirectInput8Create - HR: " << std::hex << hr);
			}
			hr = DirectInput->CreateDevice(GUID_SysMouse, &didMouse, NULL);
			if (FAILED(hr)) {
				LOG_ERROR("DirectInput8Create - HR: " << std::hex << hr);
			}

			hr = didKeyboard->SetDataFormat(&c_dfDIKeyboard);
			if (FAILED(hr)) {
				LOG_ERROR("DirectInput8Create - HR: " << std::hex << hr);
			}
			hr = didKeyboard->SetCooperativeLevel(hwnd->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			if (FAILED(hr)) {
				LOG_ERROR("DirectInput8Create - HR: " << std::hex << hr);
			}

			hr = didMouse->SetDataFormat(&c_dfDIMouse);
			if (FAILED(hr)) {
				LOG_ERROR("DirectInput8Create - HR: " << std::hex << hr);
			}
			hr = didMouse->SetCooperativeLevel(hwnd->GetHWND(), DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
			if (FAILED(hr)) {
				LOG_ERROR("DirectInput8Create - HR: " << std::hex << hr);
			}
#endif

			for (unsigned int i = 0; i != (int)KeyboardKeys::None; i++)
			{
				keybardKeyPressedSince.insert(std::pair<KeyboardKeys, float>((KeyboardKeys)i, -1));
			}

			return OS_OK;
		}

		void InputManager::PushReactiveActor(IActor* actor) {
			inputReactiveActors.push(actor);
		}

		void InputManager::PopReactiveActor() {
			inputReactiveActors.pop();
		}

		OsirisI::Graphics::Actors::IActor* InputManager::GetReactiveActor() {
			if (inputReactiveActors.size() > 0){
				return inputReactiveActors.top();
			}
			else {
				return nullptr;
			}
		}

		/*
		KeyboardKeys InputManager::GetF_Key(long wParam) {
			switch (wParam)
			{
				case VK_F1:
					return KeyboardKeys::F1;

				case VK_F2:
					return KeyboardKeys::F2;

				case VK_F3:
					return KeyboardKeys::F3;

				case VK_F4:
					return KeyboardKeys::F4;

				case VK_F5:
					return KeyboardKeys::F5;

				case VK_F6:
					return KeyboardKeys::F6;

				case VK_F7:
					return KeyboardKeys::F7;

				case VK_F8:
					return KeyboardKeys::F8;

				case VK_F9:
					return KeyboardKeys::F9;

				case VK_F10:
					return KeyboardKeys::F10;

				case VK_F11:
					return KeyboardKeys::F11;

				case VK_F12:
					return KeyboardKeys::F12;

				case VK_F13:
					return KeyboardKeys::F13;

				case VK_F14:
					return KeyboardKeys::F14;

				case VK_F15:
					return KeyboardKeys::F15;

				case VK_F16:
					return KeyboardKeys::F16;

				case VK_F17:
					return KeyboardKeys::F17;

				case VK_F18:
					return KeyboardKeys::F18;

				case VK_F19:
					return KeyboardKeys::F19;

				case VK_F20:
					return KeyboardKeys::F20;

				case VK_F21:
					return KeyboardKeys::F21;

				case VK_F22:
					return KeyboardKeys::F22;

				case VK_F23:
					return KeyboardKeys::F23;

				case VK_F24:
					return KeyboardKeys::F24;

				default:
					return KeyboardKeys::None;
			}
		}

		ReturnState InputManager::HandleInput(unsigned int message, long wParam, long lParam) {
		IEvent* iEvent = nullptr;
		KeyboardKeys kChar = KeyboardKeys::None;

		//switch (message) {
		//	case WM_CHAR: //Handle all normal characters which are human readable
		//		for (unsigned int i = 0; i < ((int)KeyboardKeys::None); i++) {
		//			if (KeyboardKeysNames[i][0] == toupper(wParam & 0xFF)) {
		//				kChar = ((KeyboardKeys)i);
		//			}
		//		}

		//		if (isupper(wParam)) {
		//			iEvent = new OsirisI_KeyboardInputEvent(wParam & 0xFF, kChar, KeyboardKeys::Shift);
		//		}
		//		else {
		//			iEvent = new OsirisI_KeyboardInputEvent(wParam & 0xFF, kChar);
		//		}
		//		break;

		//	case WM_KEYDOWN:
		//		if (MapVirtualKey(wParam, MAPVK_VK_TO_CHAR) == '\0') { //Handle only non readable characters
		//			KeyboardKeys key1 = KeyboardKeys::None, key2 = KeyboardKeys::None, key3 = KeyboardKeys::None;

		//			LOG_DEBUG("Message: 0x" << std::hex << message << ", wParam: 0x" << std::hex << wParam << ", lParam: 0x" << std::hex << lParam);
		//			LOG_DEBUG("W - CTRL(0x8): " << (wParam & MK_CONTROL) << ", SHIFT(0x4): " << (wParam & MK_SHIFT) << ", ALT: " << (HIBYTE(GetKeyState(VK_MENU)) & 0x80));
		//			LOG_DEBUG("L - CTRL(0x8): " << (lParam & MK_CONTROL) << ", SHIFT(0x4): " << (lParam & MK_SHIFT) << ", ALT: " << (HIBYTE(GetKeyState(VK_MENU)) & 0x80));

		//
		//			if (wParam == VK_DOWN) {
		//				key1 = KeyboardKeys::ArrowDown;
		//			}
		//			else if (wParam == VK_RIGHT) {
		//				key1 = KeyboardKeys::ArrowRight;
		//			}
		//			else if (wParam == VK_UP) {
		//				key1 = KeyboardKeys::ArrowUp;
		//			}
		//			else if (wParam == VK_LEFT) {
		//				key1 = KeyboardKeys::ArrowLeft;
		//			}
		//			else if (wParam == VK_RETURN) {
		//				key1 = KeyboardKeys::Return;
		//			}
		//			else if (wParam == VK_ESCAPE) {
		//				key1 = KeyboardKeys::ESC;
		//			}
		//			else if (wParam & MK_SHIFT)
		//			{
		//				if (wParam & MK_CONTROL && wParam & MK_SHIFT)
		//				{
		//					//Shift+Ctrl
		//					key1 = KeyboardKeys::Ctrl, key2 = KeyboardKeys::Shift;
		//				}
		//				else if (wParam & MK_SHIFT && HIBYTE(GetKeyState(VK_MENU)) & 0x80)
		//				{
		//					//Alt + Shift
		//					key1 = KeyboardKeys::Alt, key2 = KeyboardKeys::Shift;
		//				}
		//				else if (wParam & MK_CONTROL && HIBYTE(GetKeyState(VK_MENU)) & 0x80)
		//				{
		//					//Ctrl + Alt
		//					key1 = KeyboardKeys::Ctrl, key2 = KeyboardKeys::Alt;
		//				}
		//				else if (HIBYTE(GetKeyState(VK_MENU)) & 0x80)
		//				{
		//					//Alt
		//					key1 = KeyboardKeys::Alt;
		//				}
		//				else
		//				{
		//					//Just Shift
		//					key1 = KeyboardKeys::Shift;
		//				}

		//				if (key2 == KeyboardKeys::None) key2 = GetF_Key(wParam);
		//				else if (key3 == KeyboardKeys::None) key3 = GetF_Key(wParam);
		//			}
		//			else if (wParam & MK_CONTROL) {
		//				key1 = KeyboardKeys::Ctrl;
		//			}
		//			else {
		//				key1 = GetF_Key(wParam);
		//			}
		//			iEvent = new OsirisI_KeyboardInputEvent('\0', key1, key2, key3);
		//		}
		//		break;

		//	case WM_KEYUP:
		//		break;

		//	case WM_LBUTTONDBLCLK:
		//		iEvent = new OsirisI_MouseInputEvent(lParam & 0xFFFF, lParam >> 16, MouseActions::DoubleClick, MouseButtons::Left);
		//		break;

		//	case WM_LBUTTONDOWN:
		//		iEvent = new OsirisI_MouseInputEvent(lParam & 0xFFFF, lParam >> 16, MouseActions::Down, MouseButtons::Left);
		//		break;

		//	case WM_LBUTTONUP:
		//		iEvent = new OsirisI_MouseInputEvent(lParam & 0xFFFF, lParam >> 16, MouseActions::Up, MouseButtons::Left);
		//		break;

		//	case WM_MBUTTONDBLCLK:
		//		iEvent = new OsirisI_MouseInputEvent(lParam & 0xFFFF, lParam >> 16, MouseActions::DoubleClick, MouseButtons::Middle);
		//		break;

		//	case WM_MBUTTONDOWN:
		//		iEvent = new OsirisI_MouseInputEvent(lParam & 0xFFFF, lParam >> 16, MouseActions::Down, MouseButtons::Middle);
		//		break;

		//	case WM_MBUTTONUP:
		//		iEvent = new OsirisI_MouseInputEvent(lParam & 0xFFFF, lParam >> 16, MouseActions::Up, MouseButtons::Middle);
		//		break;

		//	case WM_RBUTTONDBLCLK:
		//		iEvent = new OsirisI_MouseInputEvent(lParam & 0xFFFF, lParam >> 16, MouseActions::DoubleClick, MouseButtons::Right);
		//		break;

		//	case WM_RBUTTONDOWN:
		//		iEvent = new OsirisI_MouseInputEvent(lParam & 0xFFFF, lParam >> 16, MouseActions::Down, MouseButtons::Right);
		//		break;

		//	case WM_RBUTTONUP:
		//		iEvent = new OsirisI_MouseInputEvent(lParam & 0xFFFF, lParam >> 16, MouseActions::Up, MouseButtons::Right);
		//		break;

		//	case WM_MOUSEMOVE:
		//		iEvent = new OsirisI_MouseInputEvent(lParam & 0xFFFF, lParam >> 16, MouseActions::None, MouseButtons::None);
		//		break;

		//	default:
		//		return OS_EVENT_NOT_HANDLED;
		//}

		if (iEvent == nullptr) {
		return OS_EVENT_NOT_HANDLED;
		}
		else {
		std::shared_ptr<IEvent> sharedEvent = std::shared_ptr<IEvent>(iEvent);

		IActor* tmp = GetReactiveActor();
		if (tmp != nullptr) tmp->HandleUserInput(sharedEvent);

		OS_CHECKSTATE(EventManager::FireQueueEvent(sharedEvent));
		return OS_OK;
		}
		}
		*/

		ReturnState InputManager::Release() {
			ReturnState ret = OS_OK;

#ifdef WINDOWS
			HRESULT hr = didKeyboard->Unacquire();
			if (FAILED(hr)){
				ret = 0x21;
				LOG_ERROR("didKeyboard->Unacquire failed - HR: " << std::hex << hr);
			}

			hr = didMouse->Unacquire();
			if (FAILED(hr)){
				ret = 0x21;
				LOG_ERROR("didMouse->Unacquire failed - HR: " << std::hex << hr);
			}

			DirectInput->Release();
#endif

			return OS_OK;
		}

		ReturnState InputManager::DetectInput(double time)
		{
#ifdef WINDOWS
			OsirisIHWND* hwnd = ((OsirisIHWND*)((Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetWindow());
			DIMOUSESTATE mouseCurrState;
			BYTE keyboardState[256];

			HRESULT hr = didKeyboard->Acquire();
			if (FAILED(hr) && hr != 0x80070005) {
				LOG_ERROR("didKeyboard->Acquire failed - HR: " << std::hex << hr);
			}
			else if (hr == 0x80070005) {
				return OS_OK;
			}

			hr = didMouse->Acquire();
			if (FAILED(hr) && hr != 0x80070005) {
				LOG_ERROR("didMouse->Acquire failed - HR: " << std::hex << hr);
			}
			else if (hr == 0x80070005) {
				return OS_OK;
			}

			hr = didMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
			if (FAILED(hr)) {
				LOG_ERROR("didMouse->GetDeviceState failed - HR: " << std::hex << hr);
			}

			hr = didKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
			if (FAILED(hr)) {
				LOG_ERROR("didKeyboard->GetDeviceState failed - HR: " << std::hex << hr);
			}

			auto toggleKeyState = [&](unsigned int keyStateValue, KeyboardKeys osKey) {
				int type = -1;
				char c = '\0';

				if (osKey >= KeyboardKeys::A &&osKey <= KeyboardKeys::Z || osKey >= KeyboardKeys::NUM0 && osKey <= KeyboardKeys::NUM9 || osKey >= KeyboardKeys::TOP0 && osKey <= KeyboardKeys::TOP9)
				{
					c = KeyboardKeysValues[(int)osKey][0];
				}

				if (keyboardState[keyStateValue] & 0x80) {
					if (keybardKeyPressedSince[osKey] != -1) {
						keybardKeyPressedSince[osKey] += time;
						if (keybardKeyPressedSince[osKey] >= 0.2f) {
							type = 0;
						}
					}
					else {
						type = 1;
						keybardKeyPressedSince[osKey] = time;
					}
				}
				else {
					if (keybardKeyPressedSince[osKey] != -1)
					{
						type = 2;
						keybardKeyPressedSince[osKey] = -1;
					}
				}


				KeyboardKeys k1 = KeyboardKeys::None, k2 = KeyboardKeys::None, k3 = KeyboardKeys::None;

				for (std::unordered_map<OsirisI::KeyboardKeys, float>::iterator it = keybardKeyPressedSince.begin(); it != keybardKeyPressedSince.end(); ++it) {
					if (it->second != -1) {
						if (k1 == KeyboardKeys::None) k1 = it->first;
						else if (k2 == KeyboardKeys::None) k2 = it->first;
						else if (k3 == KeyboardKeys::None) k3 = it->first;
						else break;
					}
					else if (it->first == osKey && type == 2)  {
						if (k1 == KeyboardKeys::None) k1 = it->first;
						else if (k2 == KeyboardKeys::None) k2 = it->first;
						else if (k3 == KeyboardKeys::None) k3 = it->first;
						else break;
					}
				}

				std::shared_ptr<IEvent> sharedEvent = nullptr;

				switch (type) {
					case 0:
						sharedEvent = std::shared_ptr<IEvent>(new OsirisI_KeyboardInputEvent_KeyPressed(c, k1, k2, k3));
						break;

					case 1:
						sharedEvent = std::shared_ptr<IEvent>(new OsirisI_KeyboardInputEvent_KeyDown(c, k1, k2, k3));
						break;

					case 2:
						sharedEvent = std::shared_ptr<IEvent>(new OsirisI_KeyboardInputEvent_KeyUp(c, k1, k2, k3));
						break;

					default: return (ReturnState)OS_EVENT_NOT_HANDLED;
				}

				IActor* tmp = GetReactiveActor();
				if (tmp != nullptr) tmp->HandleUserInput(sharedEvent);

				OS_CHECKSTATE(EventManager::FireQueueEvent(sharedEvent));
				return (ReturnState)OS_OK;
			};

			toggleKeyState(DIK_RALT, KeyboardKeys::RAlt);
			toggleKeyState(DIK_LALT, KeyboardKeys::LAlt);
			toggleKeyState(DIK_RCONTROL, KeyboardKeys::RCtrl);
			toggleKeyState(DIK_LCONTROL, KeyboardKeys::LCtrl);
			toggleKeyState(DIK_RSHIFT, KeyboardKeys::RShift);
			toggleKeyState(DIK_LSHIFT, KeyboardKeys::LShift);

			toggleKeyState(DIK_F1, KeyboardKeys::F1);
			toggleKeyState(DIK_F2, KeyboardKeys::F2);
			toggleKeyState(DIK_F3, KeyboardKeys::F3);
			toggleKeyState(DIK_F4, KeyboardKeys::F4);
			toggleKeyState(DIK_F5, KeyboardKeys::F5);
			toggleKeyState(DIK_F6, KeyboardKeys::F6);
			toggleKeyState(DIK_F7, KeyboardKeys::F7);
			toggleKeyState(DIK_F8, KeyboardKeys::F8);
			toggleKeyState(DIK_F9, KeyboardKeys::F9);
			toggleKeyState(DIK_F10, KeyboardKeys::F10);
			toggleKeyState(DIK_F11, KeyboardKeys::F11);
			toggleKeyState(DIK_F12, KeyboardKeys::F12);

			toggleKeyState(DIK_ESCAPE, KeyboardKeys::ESC);
			toggleKeyState(DIK_SPACE, KeyboardKeys::Space);
			toggleKeyState(DIK_RETURN, KeyboardKeys::Return);
			toggleKeyState(DIK_BACKSPACE, KeyboardKeys::Backspace);
			toggleKeyState(DIK_HOME, KeyboardKeys::Home);
			toggleKeyState(DIK_END, KeyboardKeys::End);
			toggleKeyState(DIK_DELETE, KeyboardKeys::Delete);
			toggleKeyState(DIK_INSERT, KeyboardKeys::Insert);
			toggleKeyState(DIK_PGUP, KeyboardKeys::PageUp);
			toggleKeyState(DIK_PGDN, KeyboardKeys::PageDown);

			toggleKeyState(DIK_LEFT, KeyboardKeys::ArrowLeft);
			toggleKeyState(DIK_RIGHT, KeyboardKeys::ArrowRight);
			toggleKeyState(DIK_UP, KeyboardKeys::ArrowUp);
			toggleKeyState(DIK_DOWN, KeyboardKeys::ArrowDown);

#pragma region Numbers
			toggleKeyState(DIK_0, KeyboardKeys::TOP0);
			toggleKeyState(DIK_1, KeyboardKeys::TOP1);
			toggleKeyState(DIK_2, KeyboardKeys::TOP2);
			toggleKeyState(DIK_3, KeyboardKeys::TOP3);
			toggleKeyState(DIK_4, KeyboardKeys::TOP4);
			toggleKeyState(DIK_5, KeyboardKeys::TOP5);
			toggleKeyState(DIK_6, KeyboardKeys::TOP6);
			toggleKeyState(DIK_7, KeyboardKeys::TOP7);
			toggleKeyState(DIK_8, KeyboardKeys::TOP8);
			toggleKeyState(DIK_9, KeyboardKeys::TOP9);

			toggleKeyState(DIK_NUMPAD0, KeyboardKeys::NUM0);
			toggleKeyState(DIK_NUMPAD1, KeyboardKeys::NUM1);
			toggleKeyState(DIK_NUMPAD2, KeyboardKeys::NUM2);
			toggleKeyState(DIK_NUMPAD3, KeyboardKeys::NUM3);
			toggleKeyState(DIK_NUMPAD4, KeyboardKeys::NUM4);
			toggleKeyState(DIK_NUMPAD5, KeyboardKeys::NUM5);
			toggleKeyState(DIK_NUMPAD6, KeyboardKeys::NUM6);
			toggleKeyState(DIK_NUMPAD7, KeyboardKeys::NUM7);
			toggleKeyState(DIK_NUMPAD8, KeyboardKeys::NUM8);
			toggleKeyState(DIK_NUMPAD9, KeyboardKeys::NUM9);
#pragma endregion

			toggleKeyState(DIK_A, KeyboardKeys::A);
			toggleKeyState(DIK_B, KeyboardKeys::B);
			toggleKeyState(DIK_C, KeyboardKeys::C);
			toggleKeyState(DIK_D, KeyboardKeys::D);
			toggleKeyState(DIK_E, KeyboardKeys::E);
			toggleKeyState(DIK_F, KeyboardKeys::F);
			toggleKeyState(DIK_G, KeyboardKeys::G);
			toggleKeyState(DIK_H, KeyboardKeys::H);
			toggleKeyState(DIK_I, KeyboardKeys::I);
			toggleKeyState(DIK_J, KeyboardKeys::J);
			toggleKeyState(DIK_K, KeyboardKeys::K);
			toggleKeyState(DIK_L, KeyboardKeys::L);
			toggleKeyState(DIK_M, KeyboardKeys::M);
			toggleKeyState(DIK_N, KeyboardKeys::N);
			toggleKeyState(DIK_O, KeyboardKeys::O);
			toggleKeyState(DIK_P, KeyboardKeys::P);
			toggleKeyState(DIK_Q, KeyboardKeys::Q);
			toggleKeyState(DIK_R, KeyboardKeys::R);
			toggleKeyState(DIK_S, KeyboardKeys::S);
			toggleKeyState(DIK_T, KeyboardKeys::T);
			toggleKeyState(DIK_U, KeyboardKeys::U);
			toggleKeyState(DIK_V, KeyboardKeys::V);
			toggleKeyState(DIK_W, KeyboardKeys::W);
			toggleKeyState(DIK_X, KeyboardKeys::X);
			toggleKeyState(DIK_Y, KeyboardKeys::Y);
			toggleKeyState(DIK_Z, KeyboardKeys::Z);





			/*		if (mouseCurrState.lX != mouseLastState.lX)
					{
					scaleX -= (mouseCurrState.lX * 0.001f);
					}
					if (mouseCurrState.lY != mouseLastState.lY)
					{
					scaleY -= (mouseCurrState.lY * 0.001f);
					}

					if (rotx > 6.28)
					rotx -= 6.28;
					else if (rotx < 0)
					rotx = 6.28 + rotx;

					if (rotz > 6.28)
					rotz -= 6.28;
					else if (rotz < 0)
					rotz = 6.28 + rotz;

					mouseLastState = mouseCurrState;*/
#endif

			return OS_OK;
		}
	}
}
