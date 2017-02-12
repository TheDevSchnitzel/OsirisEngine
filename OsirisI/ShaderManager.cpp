#include "stdafx.h"

using namespace OsirisI::Graphics::Shader;
using namespace OsirisI::Utilities;

namespace OsirisI {
	namespace Manager {
		std::map<long, IShader*> ShaderManager::shaders;

		IShader* ShaderManager::GetShader(HashedString shaderName) {
			std::map<long, IShader*>::iterator it = shaders.begin();

			for (; it != shaders.end(); ++it) {
				if (it->second->GetShaderName().GetString() == shaderName) {
					return it->second;
				}
			}
			return nullptr;
		}

		IShader* ShaderManager::GetShader(long shaderName) {
			if (shaders.find(shaderName) == shaders.end()) return nullptr;
			return shaders[shaderName];
		}


		/*
		** Creates and registers a new shader object
		*/
		ReturnState ShaderManager::CreateShader(OsirisI::Utilities::HashedString shaderName, std::string shaderPath, std::string shaderMainFunctionName, std::string shaderVersion, ShaderTypes type)
		{
			IShader* shader = GetShader(shaderName.GetHash());
			if (shader == nullptr) {

				switch (OsirisIDevice::DeviceType) {
					case OsirisI_DeviceType::DX11: {
						shader = new DX11Shader(shaderName);
						((DX11Shader*)shader)->CreateShader(shaderPath, shaderMainFunctionName, shaderVersion, type);
						OS_CHECKSTATE(RegisterShader(shader));
						break;
					}
					case OsirisI_DeviceType::OpenGL: {
						break;
					}
					default: {
						break;
					}
				}
			}
			else 
			{
				((DX11Shader*)shader)->CreateShader(shaderPath, shaderMainFunctionName, shaderVersion, type);
			}
			return OS_OK;
		}

        ReturnState ShaderManager::RegisterShader(OsirisI::Graphics::Shader::IShader* shader) {
            if(shaders.find(shader->GetShaderName().GetHash()) != shaders.end()) return OS_OBJECT_ALREADY_EXISTS;
            shaders.insert(std::pair<long, IShader*>(shader->GetShaderName().GetHash(), shader));
            return OS_OK;
        }
	}
}
