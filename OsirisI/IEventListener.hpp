#ifndef IEVENTLISTENER_HPP_
#define IEVENTLISTENER_HPP_

namespace OsirisI {
	namespace Events {
		class IEventListener {
			private:
				OsirisI::Utilities::HashedString name;

			protected:
				std::list<long> registeredEvents;

			public:
				virtual ~IEventListener() {}
				virtual ReturnState Initialize() = 0;
				virtual ReturnState HandleEvent(std::shared_ptr<OsirisI::Events::IEvent> eventObj) = 0;
				
				void SetName(OsirisI::Utilities::HashedString name) {
					this->name = name;
				}
				OsirisI::Utilities::HashedString GetName() {
					return this->name;
				}

				long GetNameHash() {
					return this->name.GetHash();
				}
		};
	}
}
#endif /* IEVENTLISTENER_HPP_ */
