#ifndef IResourceFile_HPP_
#define IResourceFile_HPP_

namespace OsirisI {
	namespace Resources {
		class Resource;

		class IResourceFile {
			protected:
				unsigned long long fileSize;
				std::string filePath;
				OsirisI::Utilities::HashedString name;
			
				char* dataBuffer; 
				unsigned long entryCount;

			public:
				virtual ~IResourceFile(){};

				std::string GetFilePath() { 
					return this->filePath; 
				}

				OsirisI::Utilities::HashedString GetName() { 
					return this->name; 
				}
				
				virtual ReturnState Initialize() = 0;

				virtual unsigned int GetResourceSize(std::string path) = 0;
				virtual Resource* GetResource(std::string path) = 0;
			
				virtual ReturnState Recover() = 0;
				virtual ReturnState Release() = 0;

				virtual bool IsFileReady() = 0;
		};
	}
}
#endif /* IResourceFile_HPP_ */
