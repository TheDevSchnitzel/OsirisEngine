#include "stdafx.h"
using namespace OsirisI;
using namespace OsirisI::Resources;
using namespace OsirisI::Graphics;
using namespace OsirisI::Graphics::Actors;
using namespace OsirisI::Manager;
using namespace OsirisI::Utilities;
using namespace OsirisI::Graphics::Devices;

namespace OsirisI {
    namespace Resources {
        namespace Loader {
            FontLoader::FontLoader() {
            }
            FontLoader::~FontLoader() {
            }

            Resource* FontLoader::Load(HashedString path, HashedString resourceName) {
                ResourcePtr<char*>* resource = dynamic_cast<ResourcePtr<char*>*>(ResourceManager::GetResourceDirectly(path.GetString()));
                if (resource == nullptr) return nullptr;

                char *buf = resource->GetValue();
                UV_Coordinates r[256];

                for (int i = 0; i < 256; i++) {
                    float leftUpU = 0, rightDownU = 0, leftUpV = 0, rightDownV = 0, texLength = 0;

                    char* fc = new char[24];
                    memcpy(fc, &buf[i * 24], 24);

                    UV_Coordinates* c = (UV_Coordinates*)fc;

                    r[i] = UV_Coordinates(c->LeftUpU, c->RightDownU, c->LeftUpV, c->RightDownV, c->TexLength, c->TexHeight);
                    OS_SAFEDELETE_ARRAY(fc);
                }
                char* tmp = new char[resource->GetSize() - (256 * 24)];
                memcpy(tmp, &buf[256 * 24], (resource->GetSize() - (256 * 24)));

                ResourcePtr<ITexture*>* tex = (ResourcePtr<ITexture*>*)LoaderManager::GetLoader<ITexture>()->Load(tmp, (resource->GetSize() - (256 * 24)), resourceName);
                if (tex == nullptr) return nullptr;
               
                Font* f = new Font(resourceName, path, r, tex->GetValue());
                return new ResourcePtr<Font*>(sizeof(UV_Coordinates)* 256 + resourceName.GetByteSize() + path.GetByteSize(), f, resourceName);
            }

            OsirisI::Resources::Resource* FontLoader::Load(char* data, long size, OsirisI::Utilities::HashedString resourceName) {
                return nullptr;
            }
            ReturnState FontLoader::Release() {
                return OS_OK;
            }
        }
    }
}
