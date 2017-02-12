#ifndef IOSIRISSCRIPT_HPP_
#define IOSIRISSCRIPT_HPP_

namespace OsirisI {
    namespace Scripting {
        class IOsirisScript {
            private:

            protected:
                char* script = nullptr;
                int numberOfRuns = 0, remainingRuns = 1;
				float intervalInSeconds = 0;

				/**
				Will be decremented to <= 0 thne script will be executed
				*/
				float currentIntervalState = 0;
				OsirisI::Utilities::HashedString scriptName;

            public:
            virtual ~IOsirisScript() {};
            
            virtual ReturnState Initialize() = 0;
            virtual ReturnState Update(float delta) = 0;
            virtual ReturnState Render() = 0;
			virtual ReturnState Release() {
				OS_CHECKSTATE(OsirisI::Manager::ScriptManager::RemoveScript(this));
				return OS_OK;
			}

			OsirisI::Utilities::HashedString GetName() {
				return scriptName;
			}

			void SetName(OsirisI::Utilities::HashedString name) {
				scriptName = name;
			}
        };
    }
}

#endif /* IOSIRISSCRIPT_HPP_ */
