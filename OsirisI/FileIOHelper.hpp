#ifndef FILEIOHELPER_HPP_
#define FILEIOHELPER_HPP_

namespace OsirisI {
	namespace Utilities {
		class FileIOHelper {
			private:
				static std::fstream* binaryFileStream;

			public:
				static std::vector<std::string> GetTextLinesFromFile(std::string inputFile){
					std::fstream fin;
					fin.open(inputFile, std::ios::in);
					if (fin.fail() || fin.bad()) return std::vector<std::string>();

					std::string text = "";
					std::vector<std::string> tmpVec;
					bool first = true;

					while (std::getline(fin, text)) {
						if (first)  {
							std::string t = text;

							for (unsigned int i = 0; i < t.length(); i++) {
								if (t[i] > 0) {
									t = t.substr(i, t.length() - i);
									break;
								}
							}

							tmpVec.push_back(t);
							first = false;
							continue;
						}
						
						tmpVec.push_back(text);
					}

					

					return tmpVec;
				}
				static unsigned int GetFileSize(std::string filePath);

				static void WriteBinaryToFile(std::string outputFile, char* data, unsigned long datalength);


				static void InitializeBinaryFileWrite(std::string outputFile);
				static void InitializeBinaryFileRead(std::string outputFile);
				static std::fstream* GetBinaryFileStream();
				static void ReleaseBinaryFileStream();
		};
	}
}

#endif /* FILEIOHELPER_HPP_ */
