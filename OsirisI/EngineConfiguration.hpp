#ifndef ENGINECONFIGURATION_HPP_
#define ENGINECONFIGURATION_HPP_

namespace OsirisI {
	namespace Resources {
		class EngineConfiguration {
			private:
				static std::map<std::string, std::map<std::string, std::string>> config;

			public:
				static OsirisI::ReturnState Initialize();
				static std::string GetProperty(std::string section, std::string key);
		};
	}
}


#endif /* ENGINECONFIGURATION_HPP_ */