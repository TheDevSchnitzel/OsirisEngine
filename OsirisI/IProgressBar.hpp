#ifndef IPROGRESSBAR_HPP_
#define IPROGRESSBAR_HPP_

namespace OsirisI {
    namespace Graphics {
        namespace Components {
            class IProgressBar : public IComponent {
                private:
				protected:
					IProgressBar(OsirisI::Utilities::HashedString name) : IComponent(name) {}
					OsirisI::Graphics::Actors::ISprite *progressBarActive, *progressBarInactive;
					float maxValue = 100, currentValue = 0;

                public:
            };
        }
    }
}

#endif /* IPROGRESSBAR_HPP_ */
