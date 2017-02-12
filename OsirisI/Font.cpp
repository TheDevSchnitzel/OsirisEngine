#include "stdafx.h"

using namespace OsirisI::Manager;
using namespace OsirisI::Utilities;
using namespace OsirisI::Resources;
using namespace OsirisI::Graphics::Actors;

namespace OsirisI {
    namespace Resources {
        Font::Font(HashedString name, HashedString containerPath, UV_Coordinates mapping[256], OsirisI::Graphics::Actors::ITexture* tex){
            this->name = name;
            this->containerPath = containerPath;
            this->fontTexture = tex;

            for (int i = 0; i < 256; i++) {
                this->mapping[i].LeftUpU = mapping[i].LeftUpU;
                this->mapping[i].LeftUpV = mapping[i].LeftUpV;
                this->mapping[i].RightDownU = mapping[i].RightDownU;
                this->mapping[i].RightDownV = mapping[i].RightDownV;
                this->mapping[i].TexLength = mapping[i].TexLength;
                this->mapping[i].TexHeight = mapping[i].TexHeight;
            }
        }
        Font::~Font() {
        }
        
        OsirisI::Graphics::Actors::ITexture* Font::GetFontTexture() {
            return this->fontTexture;
        }

        UV_Coordinates Font::GetCoordinatesUV(unsigned char c) {
            unsigned int index = c;
            return mapping[index];
        }

        ReturnState Font::Initialize() {
            return OS_OK;
        }
        ReturnState Font::Update(float delta) {
            return OS_OK;
        }
        ReturnState Font::Recover() {
            return OS_OK;
        }
        ReturnState Font::Release() {            
            return this->fontTexture->Release();
        }
    }
}
