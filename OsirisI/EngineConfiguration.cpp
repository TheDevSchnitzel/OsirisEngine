#include "stdafx.h"

using namespace OsirisI;
using namespace OsirisI::Resources;
using namespace OsirisI::Utilities;

std::map<std::string, std::map<std::string, std::string>> EngineConfiguration::config;

ReturnState EngineConfiguration::Initialize() {
	std::vector<std::string> configFile = FileIOHelper::GetTextLinesFromFile("OS_Config.ini");
	std::string currentSection = "";

	for (unsigned int i = 0; i < configFile.size(); i++) {
		std::string line = configFile[i];

		if (line[0] == '[' && line[line.length() - 1] == ']') {
			//New section
			currentSection = line.substr(1, line.length() - 2);
			if (config.find(currentSection) == config.end()) {
				config.insert(std::pair<std::string, std::map<std::string, std::string>>(currentSection, std::map<std::string, std::string>()));
			}
			else {
				LOG_ERROR("INI-File is malformed - Section is not unique! -> Line " << i);
				return OS_INITIALIZATION_FAILED;
			}
		}
		else if (line.length() == 0) {
		}
		else if (line[0] != ';') {
			//No comment lines
			std::vector<std::string> elements = HelperFunctions::split(line, '=');

			if (elements.size() != 2) {
				LOG_ERROR("INI-File is malformed - Not a valid equitation! -> Line " << i);
				return OS_INITIALIZATION_FAILED;
			}
			else {
				config[currentSection].insert(std::pair<std::string, std::string>(elements[0], elements[1]));
			}
		}
	}

	return OS_OK;
}

std::string EngineConfiguration::GetProperty(std::string section, std::string key) {
	if (config.find(section) != config.end()) {
		if (config[section].find(key) != config[section].end()) {
			return config[section][key];
		}
	}

	return "";
}
