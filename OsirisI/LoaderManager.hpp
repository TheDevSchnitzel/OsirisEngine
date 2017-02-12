#ifndef LOADERMANAGER_HPP_
#define LOADERMANAGER_HPP_

#include "ILoader.hpp"

namespace OsirisI {
	namespace Manager {
		class LoaderManager {
			private:
				static std::map<size_t, OsirisI::Resources::Loader::ILoader*> loaderMapping;

			public:
				template <class T>
				static bool RegisterLoader(OsirisI::Resources::Loader::ILoader* loader) {		
					if(loaderMapping.find(typeid(T).hash_code()) != loaderMapping.end()) return false;
					loaderMapping.insert(std::pair<size_t, OsirisI::Resources::Loader::ILoader*>(typeid(T).hash_code(), loader));
					return true;
				}

				template <class T>
				static OsirisI::Resources::Loader::ILoader* GetLoader() {
					if(loaderMapping.find(typeid(T).hash_code()) == loaderMapping.end()) return nullptr;
					return loaderMapping[typeid(T).hash_code()];
				}

                template <class T>
                static OsirisI::Resources::Resource* Load(OsirisI::Utilities::HashedString path, OsirisI::Utilities::HashedString resourceName) {
                    OsirisI::Resources::Loader::ILoader* loader = GetLoader<T>();
                    if(loader == nullptr) return nullptr; //0x1A
                    return loader->Load(path, resourceName);
                }

				//template <class T>
				//static OsirisI::Resources::Resource* Load(std::string path, bool fromFileSystem) {
				//	OsirisI::Resources::Loader::ILoader* loader = GetLoader<T>();
				//	if(loader == nullptr) return nullptr; //0x1A
				//	return loader->Load(path, fromFileSystem);
				//}

				//template <class T>
				//static OsirisI::Resources::Resource* Load(std::string name, char* rawData, unsigned int dataSize) {
				//	OsirisI::Resources::Loader::ILoader* loader = GetLoader<T>();
				//	if(loader == nullptr) return nullptr; //0x1A
				//	return loader->Load(name, rawData, dataSize);
				//}
		};
	}
}

#endif /* LOADERMANAGER_HPP_ */
