#ifndef PERSISTOR_HPP_
#define PERSISTOR_HPP_

namespace OsirisI {
	namespace Utilities {
		enum PersistorType { FILE_IN = 0, FILE_OUT, MEMORY };

		class Persistor {
		private:
			std::ofstream outStream;
			std::ifstream inStream;
			std::strstream memStream;
			PersistorType type;

		public:
			Persistor(PersistorType type, std::string file = "");

			~Persistor();

			template <class T>
			OsirisI::ReturnState Persist(const T& data, size_t dataSize) {
				if (type == PersistorType::FILE_OUT) {
					outStream.write((char*)&data, dataSize);
				}
				else if (type == PersistorType::MEMORY) {
					memStream.write((char*)&data, dataSize);
				}
				else {
					return OS_WRONG_OR_UNKNOWN_PERSISTOR_TYPE;
				}
				return OS_OK;
			}

			template <class T>
			OsirisI::ReturnState Unpersist(T& data, size_t dataSize) {
				if (type == PersistorType::FILE_IN) {
					inStream.read((char*)&data, dataSize);
				}
				else if (type == PersistorType::MEMORY) {
					memStream.read((char*)&data, dataSize);
				}
				else {
					return OS_WRONG_OR_UNKNOWN_PERSISTOR_TYPE;
				}
				return OS_OK;
			}

			int GetMemoryStreamSize();

			char* GetMemoryStreamData();

			OsirisI::ReturnState WriteMemoryStreamToFile(std::string file);
		};
	}
}
#endif /* PERSISTOR_HPP_ */
