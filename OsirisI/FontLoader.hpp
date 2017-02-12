#ifndef FONTLOADER_HPP_
#define FONTLOADER_HPP_

namespace OsirisI {
    namespace Resources {
        namespace Loader {
            class FontLoader : public ILoader {
            public:
                FontLoader();
                ~FontLoader();

                OsirisI::Resources::Resource* Load(OsirisI::Utilities::HashedString path, OsirisI::Utilities::HashedString resourceName);
                OsirisI::Resources::Resource* Load(char* data, long size, OsirisI::Utilities::HashedString resourceName);
                OsirisI::ReturnState Release();

            private:
            };
        }
    }
}
#endif /* FONTLOADER_HPP_ */
