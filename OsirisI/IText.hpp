#ifndef ITEXT_HPP_
#define ITEXT_HPP_

namespace OsirisI {
    namespace Resources {
        class Font;
    }
    namespace Graphics {
        namespace Actors {
            class IText : public ISprite {
                protected:
					IText(OsirisI::Utilities::HashedString name) : ISprite(name) {}
					std::string text;
					std::string oldText;
                    OsirisI::Resources::Font* font;
                    OsirisI::OVector4 color;
                    int pixelSize; 

                public:
                    std::string GetText() {
                        return this->text;
                    }
                    void SetText(std::string text) {
						this->oldText = this->text;
						this->text = text;
                    }

                    OsirisI::OVector4 GetColor() {
                        return this->color;
                    }
                    void SetColor(OsirisI::OVector4 color) {
                        this->color = color;
                    }
            };
        }
    }
}


#endif /* ITEXT_HPP_ */
