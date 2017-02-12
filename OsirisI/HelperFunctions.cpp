#include "stdafx.h"

namespace OsirisI {
	namespace Utilities {
		std::vector<std::string>& HelperFunctions::split(const std::string &s, char delim, std::vector<std::string> &elems) {
			std::stringstream ss(s);
			std::string item;
			while (std::getline(ss, item, delim)) {
				elems.push_back(item);
			}
			return elems;
		}

		std::vector<std::string> HelperFunctions::split(const std::string &s, char delim) {
			std::vector<std::string> elems;
			return split(s, delim, elems);
		}

		void HelperFunctions::replace(std::string& str, const std::string& from, const std::string& to) {
			if (from.empty()) return;
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
				str.replace(start_pos, from.length(), to);
				start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
			}
		}

		bool HelperFunctions::EndsWith(std::string const &fullString, std::string const &ending) {
			if (fullString.length() >= ending.length()) {
				return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
			}
			else {
				return false;
			}
		}
		bool HelperFunctions::StringContains(std::string str, std::string sequence) {
			return (((int)(str.find(sequence)) > 0) ? true : false);
		}
		int HelperFunctions::StringCharacterOccurrences(std::string str, char character) {
			return std::count(str.begin(), str.end(), character);
		}

		std::string  HelperFunctions::ToLower(std::string str){
			std::transform(str.begin(), str.end(), str.begin(), ::tolower);
			return str;
		}

		void HelperFunctions::TrimLeft(std::string &s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		}
		void HelperFunctions::TrimRight(std::string &s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		}
		void HelperFunctions::Trim(std::string &s) {
			TrimRight(s);
			TrimLeft(s);
		}

		unsigned int HelperFunctions::GetFileSize(std::string filePath) {
			std::fstream file(filePath);
			file.seekg(SEEK_END);
			unsigned int size = static_cast<unsigned int>(file.tellg());
			file.close();
			return size;
		}
	}
}
