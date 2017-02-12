#include "stdafx.h"

namespace OsirisI {
	namespace Utilities {
		std::fstream* FileIOHelper::binaryFileStream;

		/*std::vector<std::string> FileIOHelper::GetTextLinesFromFile(std::string inputFile) {
			std::fstream fin;
			fin.open(inputFile, std::ios::in);
			if(fin.fail() || fin.bad()) return;

			std::string text = "";	
			std::vector<std::string> tmpVec;		
			while(std::getline(fin, text)) tmpVec.push_back(text);

			return tmpVec;
		}*/

		unsigned int FileIOHelper::GetFileSize(std::string filePath) {
			std::fstream file(filePath);
			file.seekg(SEEK_END);
			unsigned int size = static_cast<unsigned int>(file.tellg());
			file.close();
			return size;
		}

		void FileIOHelper::WriteBinaryToFile(std::string outputFile, char* data, unsigned long datalength) {
			std::ofstream outfile (outputFile,std::ofstream::binary);
			outfile.write (data, datalength);
			outfile.close();
		}

		void FileIOHelper::InitializeBinaryFileWrite(std::string outputFile) {
			binaryFileStream = new std::fstream();
			binaryFileStream->open(outputFile, std::ios::out);
			if(!binaryFileStream->good()) {
				//ERROR
			}
		}

		void FileIOHelper::InitializeBinaryFileRead(std::string outputFile) {
			binaryFileStream = new std::fstream();
			binaryFileStream->open(outputFile, std::ios::in | std::ios::binary);
			if(!binaryFileStream->good()) {
				//ERROR
			}
		}

		std::fstream* FileIOHelper::GetBinaryFileStream() {
			return binaryFileStream;
		}
		void FileIOHelper::ReleaseBinaryFileStream() {
			binaryFileStream->close();
		}
	}
}
