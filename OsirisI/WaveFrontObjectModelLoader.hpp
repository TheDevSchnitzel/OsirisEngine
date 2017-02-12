#ifndef WAVEFRONTOBJECTMODELLOADER_HPP_
#define WAVEFRONTOBJECTMODELLOADER_HPP_

namespace OsirisI {
	namespace Resources {
		namespace Loader {
			class WaveFrontObjectModelLoader : public ILoader {
				protected:

				public:
					WaveFrontObjectModelLoader();
					~WaveFrontObjectModelLoader();
                    
                    OsirisI::Resources::Resource* Load(OsirisI::Utilities::HashedString path, OsirisI::Utilities::HashedString resourceName);
                    OsirisI::Resources::Resource* Load(char* data, long size, OsirisI::Utilities::HashedString resourceName);
					ReturnState Release();	

				private:
					Resource* Load(OsirisI::Utilities::HashedString path, OsirisI::Utilities::HashedString resourceName, std::vector<std::string> objFile, std::vector<std::string> mtlFile);
			};
		}
	}
}


#endif /* WAVEFRONTOBJECTMODELLOADER_HPP_ */
