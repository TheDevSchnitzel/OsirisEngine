#ifndef IFONT_HPP_
#define IFONT_HPP_

namespace OsirisI {
    namespace Graphics {
        namespace Actors {
            class ITexture;
        }
    }
    namespace Resources {
        class Font {
            private:
                OsirisI::UV_Coordinates mapping[256];
                OsirisI::Utilities::HashedString name, containerPath;
                OsirisI::Graphics::Actors::ITexture* fontTexture;

            public:
                Font(OsirisI::Utilities::HashedString name, OsirisI::Utilities::HashedString containerPath, OsirisI::UV_Coordinates mapping[256], OsirisI::Graphics::Actors::ITexture* tex);
                ~Font();

                OsirisI::Utilities::HashedString GetName(){
                    return this->name;
                }
                OsirisI::Utilities::HashedString GetContainerPath() {
                    return this->containerPath;
                }

                OsirisI::UV_Coordinates GetCoordinatesUV(unsigned char c);
                OsirisI::Graphics::Actors::ITexture* GetFontTexture();
                OsirisI::ReturnState Initialize();
                OsirisI::ReturnState Update(float delta);
                OsirisI::ReturnState Recover();
                OsirisI::ReturnState Release();
        };
    }
}


#endif /*IFONT_HPP_*/