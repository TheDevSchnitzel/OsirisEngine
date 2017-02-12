#ifndef SHADERINFOFACTORY_HPP_
#define SHADERINFOFACTORY_HPP_

#include "ShaderInfo.hpp"

namespace OsirisI {
	namespace Graphics {
		namespace Shader {
			class ShaderInfoFactory {
				public:
                    static std::vector<OsirisI::Graphics::Actors::ITexture*> textures; //0
					static std::vector<std::function<char*(unsigned int*)>> lambdas; //1

					static std::vector<char*> customStructs; //2
					static std::vector<unsigned int> customStructsSize;
					
					static std::vector<unsigned int*> order; //unsigned int array [2]
                                        
                    static ShaderTypes affectedShaderType;
					static long affectedShader;
                    static bool noWorldRotation;

                    static void SetAffectedShaderType(ShaderTypes type);
					static void SetAffectedShader(long shaderNameHash);
					static void AddCustomStruct(char* structData, unsigned int size);
                    static void AddTexture(OsirisI::Graphics::Actors::ITexture* tex);
					static void AddLambda(std::function<char*(unsigned int*)> lmd);
					static void NoWorldRotation(bool val);

					static ShaderInfo* CreateShaderInformation();
					static void LoadShaderInformation(ShaderInfo* info);
			};
		}
	}
}
#endif /* SHADERINFOFACTORY_HPP_ */
