#ifndef IEVENT_HPP_
#define IEVENT_HPP_

namespace OsirisI {
    namespace Events {
        struct IEvent {
            private:

            protected:

            public:
            virtual ~IEvent() {};
            

            virtual OsirisI::Utilities::HashedString GetName() = 0;
            virtual long GetNameHash() = 0;
        };
    }
}

#endif /* IEVENT_HPP_ */
