#ifndef SHADERMANAGER_HPP_
#define SHADERMANAGER_HPP_

namespace OsirisI {
	namespace Manager {
		class ShaderManager {
			private:
				static std::map<long, OsirisI::Graphics::Shader::IShader*> shaders;

			public:
				static OsirisI::Graphics::Shader::IShader* GetShader(OsirisI::Utilities::HashedString shaderName);
				static OsirisI::Graphics::Shader::IShader* GetShader(long shaderNameHash);
                
				static ReturnState CreateShader(OsirisI::Utilities::HashedString shaderName, std::string shaderPath, std::string shaderMainFunctionName, std::string shaderVersion, OsirisI::Graphics::Shader::ShaderTypes type);
				static ReturnState RegisterShader(OsirisI::Graphics::Shader::IShader* shader);
		};
	}
}

#endif /* SHADERMANAGER_HPP_ */
