#ifndef RESOURCECACHE_HPP_
#define RESOURCECACHE_HPP_

namespace OsirisI {
	namespace Resources {
		class Resource;

		class ResourceCache {
			protected:
				std::mutex resMapMutex;
				unsigned int cacheSize, allocatedMemory;
				std::map<OsirisI::Utilities::HashedString, Resource*> resources;
				OsirisI::Utilities::HashedString name;

			public:
				OsirisI::Utilities::HashedString GetName() { 
					return this->name; 
				}

				ResourceCache(unsigned int cacheSize, OsirisI::Utilities::HashedString name);
				~ResourceCache();

				ReturnState Initialize();
			
                template<class T>
                ReturnState CreateNewResource(OsirisI::Utilities::HashedString path, OsirisI::Utilities::HashedString resourceName) {
                    resMapMutex.lock();
                    if(this->resources.find(path) != this->resources.end()) {		
                        resMapMutex.unlock();
                        return OS_CACHE_RESOURCE_ALREADY_LOADED; 
                    }	
                    resMapMutex.unlock();

                    OsirisI::Resources::Resource* resource = OsirisI::Manager::LoaderManager::Load<T>(path, resourceName);

                    if(resource != nullptr) {
                        resMapMutex.lock();
                        if(this->resources.find(path) == this->resources.end()) this->resources.insert(std::pair<OsirisI::Utilities::HashedString, OsirisI::Resources::Resource*>(path, resource));
                        else resource->Release();
                        resMapMutex.unlock();
                        return OS_OK;
                    }
                    else {
                        return OS_CACHE_RESOURCE_NOT_LOADED;
                    }
                }

				/*template<class T>
				ReturnState CreateNewResource(std::string resourceName, char* rawData, unsigned int dataSize);
				*/

				ReturnState ReleaseResource(OsirisI::Utilities::HashedString name);

				Resource* GetResource(OsirisI::Utilities::HashedString name);
				ReturnState Release();
		};
	}
}
#endif /* RESOURCECACHE_HPP_ */
