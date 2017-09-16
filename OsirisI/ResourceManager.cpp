#include "stdafx.h"

using namespace OsirisI::Resources;
using namespace OsirisI::Utilities;

namespace OsirisI {
	namespace Manager {
		std::map<HashedString,IResourceFile*> ResourceManager::resourceFiles;
		std::map<HashedString,ResourceCache*> ResourceManager::cachesName;
		std::map<size_t, OsirisI::Resources::ResourceCache*> ResourceManager::cachesType;

		ResourceCache* ResourceManager::GetCache(HashedString name) {
			if(cachesName.find(name) == cachesName.end()) return nullptr;
			return cachesName[name];
		}
		
		ReturnState ResourceManager::RegisterResourceFile(IResourceFile* file) {
			if(resourceFiles.find(file->GetName()) != resourceFiles.end()) return OS_OBJECT_ALREADY_EXISTS; 
			if(!file->IsFileReady()) return OS_OPEN_FILE_FAILED;

			resourceFiles.insert(std::pair<HashedString,IResourceFile*>(file->GetName(),file));
			return OS_OK;
		}

		ReturnState ResourceManager::RegisterResourceFile(HashedString name, std::string filePath) {
			if(resourceFiles.find(name) != resourceFiles.end()) return 0x1F;
			IResourceFile* file = new ZIPResourceFile(filePath, name);
			if(file->Initialize() != OS_OK) return OS_INITIALIZATION_FAILED;
			resourceFiles.insert(std::pair<HashedString ,IResourceFile*>(name,file));
			return OS_OK;
		}
				
		ReturnState ResourceManager::RemoveResourceFile(OsirisI::Resources::IResourceFile* file) {
			if(resourceFiles.find(file->GetName()) != resourceFiles.end()) return OS_OBJECT_ALREADY_EXISTS;
			ReturnState state = resourceFiles[file->GetName()]->Release();
			resourceFiles.erase(file->GetName());
			return state;
		}

		ReturnState ResourceManager::RemoveResourceFile(HashedString name) {
			if(resourceFiles.find(name) != resourceFiles.end()) return OS_OBJECT_ALREADY_EXISTS;
			ReturnState state = resourceFiles[name]->Release();
			resourceFiles.erase(name);
			return state;
		}

		/*ReturnState ResourceManager::CreateCache(HashedString name, unsigned int size) {
			if(cachesName.find(name) != cachesName.end()) return OS_OBJECT_ALREADY_EXISTS;
			ResourceCache* cache = new ResourceCache(size, name);

			if(cache->Initialize() != OS_OK) return OS_INITIALIZATION_FAILED; 
			cachesName.insert(std::pair<HashedString,ResourceCache*>(name, cache));
			return OS_OK;
		}*/
		
		Resource* ResourceManager::GetResourceDirectly(std::string resPath) {
			if (resourceFiles.size() == 0) return nullptr;

			std::vector<std::string> splitted = HelperFunctions::split(resPath, ':');
			if(splitted.size() == 1) {
				Resource* res = nullptr;
				for(std::map<HashedString,IResourceFile*>::iterator it = resourceFiles.begin(); it != resourceFiles.end(); ++it) {
					res = it->second->GetResource(splitted[0]);
					if(res != nullptr) return res;
				}
				return nullptr;
			}
			else if(splitted.size() == 2) return resourceFiles[HashedString(splitted[0])]->GetResource(splitted[1]);
			else return nullptr;
		}
				
		Resource* ResourceManager::GetResource(HashedString resName, HashedString cacheName) {
			ResourceCache* cache = GetCache(cacheName);
			if(cache == nullptr) return nullptr;
			return cache->GetResource(resName);
		}

		ReturnState ResourceManager::ReleaseCache(HashedString name) {
			if(cachesName.find(name) != cachesName.end()) return OS_OBJECT_ALREADY_EXISTS;
			cachesName[name]->Release();
			cachesName.erase(name);
			return OS_OK;
		}

		ReturnState ResourceManager::Release() {
			if(resourceFiles.size() > 0) {
				for(std::map<HashedString,IResourceFile*>::iterator it = resourceFiles.begin(); it != resourceFiles.end(); ++it) {
					if(it->second != NULL) it->second->Release();
				}
			}
			if(cachesName.size() > 0) {
                //All caches are registered in both maps, so it's enough to release them once
				for(std::map<HashedString,ResourceCache*>::iterator it = cachesName.begin(); it != cachesName.end(); ++it) { 
					if(it->second != NULL) it->second->Release();
				}
			}
			cachesName.clear();
			cachesType.clear();
			resourceFiles.clear();
			return OS_OK;
		}
	}
}
