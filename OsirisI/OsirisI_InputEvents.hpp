#ifndef MOUSEINPUTEVENT_HPP_
#define MOUSEINPUTEVENT_HPP_

namespace OsirisI {
    namespace Events {        
        struct OsirisI_MouseInputEvent : public IEvent {
            private:

            protected:
            MouseActions action;
            MouseButtons button;
            int posX, posY;

            public:
                static const OsirisI::Utilities::HashedString eventName;

            OsirisI_MouseInputEvent(int posX, int posY, MouseActions action, MouseButtons button) {
                this->action = action;
                this->button = button;
                this->posX = posX;
                this->posX = posX;
            }

            ~OsirisI_MouseInputEvent() {
            }

            MouseButtons GetButton(){
                return this->button;
            }

            MouseActions GetAction() {
                return this->action;
            }

            int GetMouseX() {
                return this->posX;
            }

            int GetMouseY() {
                return this->posY;
            }
            
            OsirisI::Utilities::HashedString GetName() {
                return eventName;
            }

            long GetNameHash() {
                return eventName.GetHash();
            }
        };
		       
        struct OsirisI_KeyboardInputEvent : public IEvent {
            private:

            protected:
            //If there is a character which is no control char it is stored here
            //If char is \0 none of the special keys is used
            char c;
            KeyboardKeys key1, key2, key3;

            public:            
                static const OsirisI::Utilities::HashedString eventName;

				OsirisI_KeyboardInputEvent(char c, KeyboardKeys key1, KeyboardKeys key2 = KeyboardKeys::None, KeyboardKeys key3 = KeyboardKeys::None) {
                this->c = c;
				this->key1 = key1;
				this->key2 = key2;
				this->key3 = key3;
            }
            ~OsirisI_KeyboardInputEvent() {
            }

            char GetChar() {
                return this->c;
            }

			KeyboardKeys GetKey1() {
				return this->key1;
            }

            KeyboardKeys GetKey2() {
				return this->key2;
            }

			KeyboardKeys GetKey3() {
				return this->key3;
            }

            OsirisI::Utilities::HashedString GetName() {
                return eventName;
            }

            long GetNameHash() {
                return eventName.GetHash();
            }
        };
		struct OsirisI_KeyboardInputEvent_KeyDown : OsirisI_KeyboardInputEvent{
			OsirisI_KeyboardInputEvent_KeyDown(char c, KeyboardKeys key1, KeyboardKeys key2 = KeyboardKeys::None, KeyboardKeys key3 = KeyboardKeys::None) : OsirisI_KeyboardInputEvent(c, key1, key2, key3) {}
		};
		struct OsirisI_KeyboardInputEvent_KeyPressed : OsirisI_KeyboardInputEvent{
			OsirisI_KeyboardInputEvent_KeyPressed(char c, KeyboardKeys key1, KeyboardKeys key2 = KeyboardKeys::None, KeyboardKeys key3 = KeyboardKeys::None) : OsirisI_KeyboardInputEvent(c, key1, key2, key3) {}
		};
		struct OsirisI_KeyboardInputEvent_KeyUp : OsirisI_KeyboardInputEvent{
			OsirisI_KeyboardInputEvent_KeyUp(char c, KeyboardKeys key1, KeyboardKeys key2 = KeyboardKeys::None, KeyboardKeys key3 = KeyboardKeys::None) : OsirisI_KeyboardInputEvent(c, key1, key2, key3) {}
		};
    }
}

#endif /* MOUSEINPUTEVENT_HPP_ */
