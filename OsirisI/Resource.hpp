#ifndef RESOURCE_HPP_
#define RESOURCE_HPP_

namespace OsirisI {
	namespace Resources {
		class Resource {
			protected:
				OsirisI::Utilities::HashedString resourcePath;
				OsirisI::Utilities::HashedString name;
				unsigned int size;

			public:
				OsirisI::Utilities::HashedString GetName() {
					return this->name;
				}
			
				unsigned int GetSize() {			
					return this->size;
				}

				void SetSize(unsigned int size) {
					this->size = size;
				}

				virtual ReturnState Recover() = 0;
				virtual ReturnState Release() = 0;
		};

		/*
		class ResourceValue : public Resource {
		private:
		long releaseResult;

		public:
		~ResourceValue() {}	
		template <class T>
		ResourceValue(unsigned int size, T rhs, bool isArray, bool isReleasable) {
		SetValue<T>(size, rhs);
		this->bArray = isArray;
		this->bReleasable = isReleasable;
		if(this->bReleasable) {
		this->release = std::function<ReturnState()>([=]() -> ReturnState {return rhs.Release();});
		}
		else {
		this->release = std::function<ReturnState()>([=]() -> ReturnState {return 0x0});
		}
		}

		template<class T>
		T MultiStage(T rhs, int com) {
		static T value;
		if(com==0) value = rhs;
		else if(com==1) return value;
		return value;
		}

		template<class T>
		T GetValue() {
		return MultiStage(((T)T()), 1);
		}	

		template<class T>
		void SetValue(unsigned int size, T rhs) {
		this->size = size;
		MultiStage<T>(rhs, 0);
		}

		ReturnState Release() {
		return release();
		}
		};
		*/

		template <class T>
		class ResourcePtr : public Resource {
			private:
				//std::shared_ptr<T> value;
	            T value;
		        bool isArray;

			public:
				~ResourcePtr() {
					if (value != nullptr) {
						OS_CHECKSTATE_LOG(Release());
					}
				}

			    ResourcePtr(unsigned int size, T rhs, OsirisI::Utilities::HashedString resourceName, bool isArray = false) { //std::shared_ptr<T> rhs, OsirisI::Utilities::HashedString resourceName) {
					this->size = size; 
					this->value = rhs;
					this->name = resourceName;
			        this->isArray = isArray;
				}

				//std::shared_ptr<T> GetValue() {
				bool IsArray() {
					return isArray;
				}

				T GetValue() {
					return value;
				}

				void SetValue(T ptr) { //std::shared_ptr<T> rhs) {
					value = rhs;
				}  

				ReturnState Recover() {
					if (value != nullptr) {
						OS_CHECKSTATE(Release());
					}

					OsirisI::Resources::ResourcePtr<T>* resource = static_cast<OsirisI::Resources::ResourcePtr<T>*>(OsirisI::Manager::LoaderManager::Load<T>(this->resourcePath, this->name));
					
					this->size = resource->GetSize();
					this->name = resource->GetName();
					this->isArray = resource->IsArray();

					this->value = (T)new char[this->size];
					memcpy_s(this->value, this->size, resource->GetValue(), this->size);

					delete resource;

					return OS_OK;
				}

				ReturnState Release() {				
					if(isArray) OS_SAFEDELETE_ARRAY(value);
				    else OS_SAFEDELETE(value);
					//LOG_WARNING("RESOURCE WIRD NICHT GELÖSCHT! - LEAK");
					return OS_OK;
				}
		};
	}
}
#endif /* RESOURCE_HPP_ */
