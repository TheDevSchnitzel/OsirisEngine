#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

namespace OsirisI {
	namespace Manager {
		class ResourceManager {
			protected:	
				static std::map<OsirisI::Utilities::HashedString, OsirisI::Resources::IResourceFile*> resourceFiles;		
				static std::map<OsirisI::Utilities::HashedString, OsirisI::Resources::ResourceCache*> cachesName;
				static std::map<std::size_t, OsirisI::Resources::ResourceCache*> cachesType;

			public:
				template <class T>
				static OsirisI::Resources::ResourceCache* GetCache() {		
					if(cachesType.find(typeid(T).hash_code()) == cachesType.end()) return nullptr;
					return cachesType[typeid(T).hash_code()];
				}
				static OsirisI::Resources::ResourceCache* GetCache(OsirisI::Utilities::HashedString name);

				static ReturnState RegisterResourceFile(OsirisI::Resources::IResourceFile* file);
				static ReturnState RegisterResourceFile(OsirisI::Utilities::HashedString, std::string filePath);
				static ReturnState RemoveResourceFile(OsirisI::Resources::IResourceFile* file);
				static ReturnState RemoveResourceFile(OsirisI::Utilities::HashedString);

				template <class T>
				static ReturnState CreateCache(OsirisI::Utilities::HashedString name, unsigned int size) {
					if(cachesName.find(name) != cachesName.end()) return 0x1F;
					if(cachesType.find(typeid(T).hash_code()) != cachesType.end()) return 0x1F;
					OsirisI::Resources::ResourceCache* cache = new OsirisI::Resources::ResourceCache(size, name);

					if(cache->Initialize() != 0x0) return 0x22;
					cachesName.insert(std::pair<OsirisI::Utilities::HashedString,OsirisI::Resources::ResourceCache*>(name, cache));
					cachesType.insert(std::pair<size_t,OsirisI::Resources::ResourceCache*>(typeid(T).hash_code(), cache));
					return OS_OK;
				}/*
				static ReturnState CreateCache(OsirisI::Utilities::HashedString, unsigned int size);*/

				template <class T>
				static OsirisI::Resources::Resource* GetResource(OsirisI::Utilities::HashedString resPath) {
					OsirisI::Resources::ResourceCache* cache = GetCache<T>();
					if(cache == nullptr) return nullptr;
					
                    OsirisI::Resources::ResourcePtr<T*>* res = static_cast<OsirisI::Resources::ResourcePtr<T*>*>(cache->GetResource(resPath));
                    if(res == nullptr) {
                        std::string rep1 = resPath.GetString();
                        OsirisI::Utilities::HelperFunctions::replace(rep1, "\\", "/");
                        std::vector<std::string> split1 = OsirisI::Utilities::HelperFunctions::split(rep1, '/');
                        std::vector<std::string> split2 = OsirisI::Utilities::HelperFunctions::split(split1[split1.size()-1], '.');

                        res = ((OsirisI::Resources::ResourcePtr<T*>*)ResourceManager::LoadAndCache<T>(OsirisI::Utilities::HashedString(resPath), OsirisI::Utilities::HashedString(split2[split2.size()-1] + "_AUTO_LOAD")));
                    }
                    return res;
				}
				static OsirisI::Resources::Resource* GetResource(OsirisI::Utilities::HashedString resPath, OsirisI::Utilities::HashedString cacheName);
				
				/**
				GetResourceDirectly loads the specified resource without caching it.
				Furthermore the resource is not given to any type-loader, so it just contains a char array with raw data.
				*/
				static OsirisI::Resources::Resource* GetResourceDirectly(std::string resPath);

				template <class T>
				static OsirisI::Resources::Resource* LoadAndCache(OsirisI::Utilities::HashedString path, OsirisI::Utilities::HashedString resourceName) {
					OsirisI::Resources::ResourceCache* c = GetCache<T>();
					if(c->CreateNewResource<T>(path, resourceName) == OS_OK) { //CreateNewResource calls LoadManager::Load internally
						return c->GetResource(path);
					}
					else {
						return nullptr;
					}
				}

				//template <class T>
				//static OsirisI::Resources::Resource* LoadAndCache(std::string resourceName, char* rawData, unsigned int dataSize) {
				//	OsirisI::Resources::ResourceCache* c = GetCache<T>();
				//	if(c->CreateNewResource(resourceName, rawData, dataSize) == 0x0) { //CreateNewResource calls LoadManager::Load internally
				//		return c->GetResource(path);
				//	}
				//	else {
				//		return nullptr;
				//	}
				//}

				template <class T>
				static OsirisI::Resources::Resource* LoadWithoutCache(char* rawData, unsigned int dataSize) {
					OsirisI::Resources::Resource* res = LoaderManager::Load<T>(rawData, dataSize);
					if(res == nullptr) return nullptr;
					return res;
				}

				template <class T>
				static ReturnState ReleaseCache() {
					if(cachesType.find(typeid(T).hash_code()) != cachesType.end()) return OS_OBJECT_ALREADY_EXISTS;
					cachesType[typeid(T).hash_code()]->Release();
					cachesType.erase(typeid(T).hash_code());
					return OS_OK;
				}
				
				static ReturnState Persist(OsirisI::Utilities::Persistor* persistor);
				static ReturnState Unpersist(OsirisI::Utilities::Persistor* persistor);

				static ReturnState ReleaseCache(OsirisI::Utilities::HashedString name);
				static ReturnState Release();
		};
	}
}

#endif /* RESOURCEMANAGER_HPP_ */