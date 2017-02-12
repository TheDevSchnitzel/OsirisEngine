#include "stdafx.h"

using namespace OsirisI::Utilities;

Persistor::Persistor(PersistorType type, std::string file) {
	this->type = type;

	if (type == PersistorType::FILE_IN) {
		inStream = std::ifstream(file, std::ios::binary);
	}
	else if (type == PersistorType::FILE_OUT) {
		outStream = std::ofstream(file, std::ios::binary);
	}
}

Persistor::~Persistor() {
	if (type == PersistorType::FILE_IN && inStream.is_open()) {
		inStream.close();
	}
	else if (type == PersistorType::FILE_OUT && outStream.is_open()) {
		outStream.close();
	}
}


int Persistor::GetMemoryStreamSize() {
	if (type == PersistorType::MEMORY) {
		memStream.seekg(0, std::ios_base::end);
		return memStream.tellg();
	}
	else {
		return OS_WRONG_OR_UNKNOWN_PERSISTOR_TYPE;
	}
}

char* Persistor::GetMemoryStreamData() {
	if (type == PersistorType::MEMORY) {
		int size = GetMemoryStreamSize();

		memStream.seekg(0);
		char* data = new char[size];
		memStream.read(data, size);
		return data;
	}
	else {
		return nullptr;
	}
}

ReturnState Persistor::WriteMemoryStreamToFile(std::string file) {
	char* data = GetMemoryStreamData();
	if (data == nullptr) {
		return OS_WRONG_OR_UNKNOWN_PERSISTOR_TYPE;
	}

	std::ofstream f(file, std::ios_base::binary);
	f.write(data, GetMemoryStreamSize());
	f.close();
	return OS_OK;
}
