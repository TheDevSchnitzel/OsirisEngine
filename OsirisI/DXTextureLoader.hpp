#ifndef DXTEXTURELOADER_HPP_
#define DXTEXTURELOADER_HPP_

namespace OsirisI {
	namespace Resources {
		namespace Loader {
			class DXTextureLoader : public ILoader {
				public:
					DXTextureLoader();
					~DXTextureLoader();

                    OsirisI::Resources::Resource* Load(OsirisI::Utilities::HashedString path, OsirisI::Utilities::HashedString resourceName);
                    OsirisI::Resources::Resource* Load(char* data, long size, OsirisI::Utilities::HashedString resourceName);
					ReturnState Release();

				private:
			};
		}
	}
}
#endif /* DXTEXTURELOADER_HPP_ */
