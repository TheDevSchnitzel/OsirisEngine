#ifndef INPUTMANAGER_HPP_
#define INPUTMANAGER_HPP_

////////////////NEEDED FOR USER, OTHERWISE THESE HEADERS ARE MISSING IN NORMAL BUILD////////////////
#include "OsirisI_InputEvents.hpp"
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace OsirisI {
    namespace Manager {
        class InputManager {
			private:
#ifdef WINDOWS
				static IDirectInputDevice8* didKeyboard;
				static IDirectInputDevice8* didMouse;
				static DIMOUSESTATE lastMouseState;
				static LPDIRECTINPUT8 DirectInput;
#endif


				static float rotx;
				static float rotz;
				static float scaleX;
				static float scaleY;

				static OMatrix4x4 Rotationx;
				static OMatrix4x4 Rotationz;
				static std::unordered_map<OsirisI::KeyboardKeys, float> keybardKeyPressedSince;
				static std::unordered_map<OsirisI::MouseButtons, float> mouseKeyPressedSince;

				/*Stack structure for actors that should react to input => only the top one will react, a new actor is pushed e.g. when a subwindow in a menu is opened*/
				static std::stack<OsirisI::Graphics::Actors::IActor*> inputReactiveActors;
				//static OsirisI::KeyboardKeys GetF_Key(long wParam);
				
            protected:            

			public:
				static void PushReactiveActor(OsirisI::Graphics::Actors::IActor* actor);
				static void PopReactiveActor();
				static OsirisI::Graphics::Actors::IActor* GetReactiveActor();

                //static ReturnState HandleInput(unsigned int message, long wParam, long lParam);
				static ReturnState DetectInput(double time);
				static ReturnState Initialize(); 
				static ReturnState Release();
        };
    }
}

#endif /* INPUTMANAGER_HPP_ */
