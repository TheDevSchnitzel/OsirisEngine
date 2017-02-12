#include "stdafx.h"

using namespace OsirisI::Utilities;

namespace OsirisI {
	namespace Resources {
		ZIPResourceFile::ZIPResourceFile(std::string filePath, HashedString name) {
			this->resourceFile = nullptr;
			this->filePath = filePath;
			this->name = name;
		}

		ZIPResourceFile::~ZIPResourceFile() {
			Release();
		}

		ReturnState ZIPResourceFile::Initialize() {
			this->resourceFile = unzOpen(this->filePath.c_str());
			if(this->resourceFile == NULL || this->resourceFile == nullptr) return 0x11;


			if ( unzGetGlobalInfo( this->resourceFile, &this->global_info ) != UNZ_OK ) {
				unzClose( this->resourceFile );
				return 0x12;
			}


			// Buffer to hold data read from the zip file.
			//char read_buffer[ READ_SIZE ];

			// Loop to extract all files
			for ( unsigned int i = 0; i < this->global_info.number_entry; ++i ) {
				// Get info about current file.
				unz_file_info file_info;
				char filename[_MAX_PATH];
				if ( unzGetCurrentFileInfo( this->resourceFile, &file_info, filename, _MAX_PATH, NULL, 0, NULL, 0 ) != UNZ_OK ) {
					unzClose( this->resourceFile );
					return 0x12;
				}


				// Check if this entry is a directory or file.
				const size_t filename_length = strlen( filename );
				unz_file_pos filePosition;
				if(unzGetFilePos(this->resourceFile, &filePosition) == UNZ_OK) this->resourceMapping.insert(std::pair<std::string, unz_file_pos>(filename, filePosition));


				// Go the the next entry listed in the zip file.
				if ( ( i+1 ) < this->global_info.number_entry ) {
					if ( unzGoToNextFile( this->resourceFile ) != UNZ_OK ) {
						unzClose( this->resourceFile );
						return 0x12;
					}
				}
			}
			return OS_OK;
		}

		unsigned int ZIPResourceFile::GetResourceSize(std::string path) {	
			if(unzGoToFilePos(this->resourceFile, &this->resourceMapping[path])  == UNZ_OK) {
				unz_file_info file_info;
				char filename[_MAX_PATH];
				if ( unzGetCurrentFileInfo( this->resourceFile, &file_info, filename, _MAX_PATH, NULL, 0, NULL, 0 ) == UNZ_OK ) {
					return file_info.uncompressed_size;
				}
			}
			return 0;
		}

		Resource* ZIPResourceFile::GetResource(std::string path) {
			if(this->resourceMapping.find(path) == this->resourceMapping.end()) return NULL;


			this->resFile_mutex.lock();

			if(unzGoToFilePos(this->resourceFile, &this->resourceMapping[path])  == UNZ_OK) {
				if ( unzOpenCurrentFile( this->resourceFile ) == UNZ_OK ) {
					unsigned int fileSize = GetResourceSize(path);
					char* buffer = new char[fileSize];

					if( unzReadCurrentFile( this->resourceFile, buffer, fileSize) == fileSize) {				
						unzCloseCurrentFile( this->resourceFile );
						this->resFile_mutex.unlock();

						Resource* res = new ResourcePtr<char*>(fileSize, buffer, HashedString(path));
						//SAFEDELETE_ARRAY(buffer);
						return res;
					}
					else {
						OS_SAFEDELETE_ARRAY(buffer);
						unzCloseCurrentFile( this->resourceFile );
						this->resFile_mutex.unlock();
						return NULL;
					}
				}
			}
			this->resFile_mutex.unlock();

			return OS_OK;
		}

		ReturnState ZIPResourceFile::Recover() {
			return OS_OK;
		}

		ReturnState ZIPResourceFile::Release() {
			if(unzClose(this->resourceFile) == UNZ_OK) return OS_OK;	
			return 0x13;
		}

		bool ZIPResourceFile::IsFileReady() {
			if(this->resourceFile != NULL && this->resourceFile != nullptr) return true;
			return false;
		}
	}
}
