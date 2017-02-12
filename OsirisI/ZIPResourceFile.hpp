#ifndef ZIPRESOURCEFILE_HPP_
#define ZIPRESOURCEFILE_HPP_

#include <zlib/zip.h>
#include <zlib/unzip.h>
#pragma comment (lib, "zdll.lib" )
#pragma comment (lib, "zlib.lib" )
#include <mutex>

//Data from zip specifications
//4 byte -> unsigned int
//2 byte -> unsigned short
/*
#pragma pack(1)
struct ZIPLocalFileHeader {
enum ZIP_SIGNATURE { SIGNATURE = 0x04034b50 };
unsigned int   localFileHeaderSignature;
unsigned short versionNeededToExtract;
unsigned short generalPurposeBitFlag;
unsigned short compressionMethod;
unsigned short lastModFileTime;
unsigned short lastModFileDate;
unsigned int   crc32;
unsigned int   compressedSize;
unsigned int   uncompressedSize;
unsigned short fileNameLength;
unsigned short extraFieldLength;
};
struct ZIPCentralDirectoryEND {
enum ZIP_SIGNATURE { SIGNATURE = 0x06054b50 };
unsigned int   centralDirSignature;
unsigned short diskNumber;
unsigned short diskNumberStartOffset;
unsigned short totalNumberEntriesInCentralDirOnDisk;
unsigned short totalNumberEntriesInCentralDir;
unsigned int   sizeOfCentralDirectory;
unsigned int   centralDirectoryStartOffset;
unsigned short zipFileCommentLength;
};
struct ZIPCentralDirectoryHeader {
enum ZIP_SIGNATURE { SIGNATURE = 0x02014b50 };
unsigned int   centralFileHeaderSignature;
unsigned short versionMadeBy;
unsigned short versionNeededToExtract;
unsigned short generalPurposeBitFlag;
unsigned short compressionMethod;
unsigned short lastModFileTime;
unsigned short lastModFileDate;
unsigned int   crc32;
unsigned int   compressedSize;
unsigned int   uncompressedSize;
unsigned short fileNameLength;
unsigned short extraFieldLength;
unsigned short fileCommentLength;
unsigned short diskNumberStart;
unsigned short internalFileAttributes;
unsigned int   externalFileAttributes;
unsigned int   relativeOffsetOfLocalHeader;

char *GetFileName()   const { return (char *)(this + 1); }
char *GetExtraField() const { return GetFileName() + fileNameLength; }
char *GetZIPComment() const { return GetExtraField() + extraFieldLength; }
};
#pragma pack()
*/

namespace OsirisI {
	namespace Resources {
		class ZIPResourceFile : public IResourceFile {
			protected:
				//const ZIPCentralDirectoryHeader **centralDirHeader;  
				unzFile resourceFile;
				unz_global_info global_info;
				std::map<std::string, unz_file_pos> resourceMapping;
				std::mutex resFile_mutex;

			public:
				ZIPResourceFile(std::string filePath, OsirisI::Utilities::HashedString name);
				~ZIPResourceFile();

				ReturnState Initialize();
				unsigned int GetResourceSize(std::string path);
				Resource* GetResource(std::string path);

				ReturnState Recover();
				ReturnState Release();

				bool IsFileReady();

			private:
		};
	}
}
#endif /* ZIPRESOURCEFILE_HPP_ */
