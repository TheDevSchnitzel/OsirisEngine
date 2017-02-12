#ifndef OSIRISIENGINEEVENTLISTENER_HPP_
#define OSIRISIENGINEEVENTLISTENER_HPP_

namespace OsirisI {
	namespace Events {
		class OsirisIEngineEventListener : public IEventListener {
			private:

			protected:

			public:
				OsirisIEngineEventListener();
				~OsirisIEngineEventListener();

				OsirisI::ReturnState Initialize();
				OsirisI::ReturnState HandleEvent(std::shared_ptr<OsirisI::Events::IEvent> eventObj);
		};
	}
}

#endif /*OSIRISIENGINEEVENTLISTENER_HPP_*/
