#include "stdafx.h"

using namespace OsirisI::Manager;
using namespace OsirisI::Utilities;
using namespace OsirisI::Resources::Loader;

namespace OsirisI {
	namespace Resources {
		ResourceCache::ResourceCache(unsigned int cacheSize, HashedString name) {
			this->cacheSize = cacheSize;
			this->name = name;
		}

		ResourceCache::~ResourceCache() {
		}

		ReturnState ResourceCache::Initialize() {
			return OS_OK;
		}
        			
		/*template<class T>
		ReturnState ResourceCache::CreateNewResource(HashedString resourceName, char* rawData, unsigned int dataSize) {
			resMapMutex.lock();
			if(this->resources.find(resourceName) != this->resources.end()) {		
				resMapMutex.unlock();
				return 0x16;
			}	
			resMapMutex.unlock();
			
			Resource* resource = LoaderManager::Load<T>(rawData, dataSize);

			if(resource != nullptr) {
				resMapMutex.lock();
				if(this->resources.find(resourceName) == this->resources.end()) this->resources.insert(std::pair<std::string,Resource*>(resourceName, resource));
				else resource->Release();
				resMapMutex.unlock();
				return 0;
			}
			else return 0x17;
		}*/

		ReturnState ResourceCache::ReleaseResource(HashedString name) {
			resMapMutex.lock();
			ReturnState state = this->resources[name]->Release();
			this->resources.erase(name);
			resMapMutex.unlock();
			return state;
		}

		Resource* ResourceCache::GetResource(HashedString name) {
			resMapMutex.lock();
			if(this->resources.find(name) == this->resources.end()) {
				resMapMutex.unlock();
				return nullptr;
			}
			Resource* res = this->resources[name];
			resMapMutex.unlock();
			return res;
		}

		ReturnState ResourceCache::Release() {
			resMapMutex.lock();
			ReturnState state = OS_OK;
			for(std::map<HashedString,Resource*>::iterator it = this->resources.begin(); it != this->resources.end(); ++it) {
				state = it->second->Release();
			}
			this->resources.clear();
			resMapMutex.unlock();
			delete this;
			return state;
		}
	}
}
