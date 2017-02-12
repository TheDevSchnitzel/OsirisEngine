#include "stdafx.h"

using namespace OsirisI;
using namespace OsirisI::Graphics;
using namespace OsirisI::Graphics::Actors;

namespace OsirisI {
	namespace Graphics {
		namespace Shader {
			std::vector<ITexture*> ShaderInfoFactory::textures;
			std::vector<std::function<char*(unsigned int*)>> ShaderInfoFactory::lambdas;
				
            std::vector<char*> ShaderInfoFactory::customStructs;
			std::vector<unsigned int> ShaderInfoFactory::customStructsSize;

            std::vector<unsigned int*> ShaderInfoFactory::order;

            ShaderTypes ShaderInfoFactory::affectedShaderType;
            long ShaderInfoFactory::affectedShader;
            bool ShaderInfoFactory::noWorldRotation;

            void ShaderInfoFactory::NoWorldRotation(bool val) {
                noWorldRotation = val;
            }

            void ShaderInfoFactory::SetAffectedShaderType(ShaderTypes type) {
                affectedShaderType = type;
            }

            void ShaderInfoFactory::SetAffectedShader(long shaderNameHash) {
                affectedShader = shaderNameHash;
            }
			
            void ShaderInfoFactory::AddCustomStruct(char* structData, unsigned int size) {
				unsigned int* orders = new unsigned int[2];
                orders[0] = 2;
                orders[1] = customStructs.size();
				order.push_back(orders);

				customStructsSize.push_back(size);
				customStructs.push_back(structData);                
			}
            void ShaderInfoFactory::AddTexture(ITexture* tex) {
				unsigned int* orders = new unsigned int[2];
                orders[0] = 0;
                orders[1] = textures.size();
				order.push_back(orders);

				textures.push_back(tex);
			}
			void ShaderInfoFactory::AddLambda(std::function<char*(unsigned int*)> lmd) {
				unsigned int* orders = new unsigned int[2];
                orders[0] = 1;
                orders[1] = lambdas.size();
				order.push_back(orders);

				lambdas.push_back(lmd);
			}


			ShaderInfo* ShaderInfoFactory::CreateShaderInformation() {
				ShaderInfo* info = new ShaderInfo();

                info->affectedShaderType = affectedShaderType;
                info->shaderNameHash = affectedShader;
                info->noWorldRotation = noWorldRotation;
                
				info->texCount = textures.size();
				info->lambdaCount = lambdas.size();
				info->customStructsCount = customStructs.size();
				info->orderCount = order.size();

                if(info->texCount > 0) {
                    info->textures = new ITexture*[info->texCount];
                    for(unsigned int i = 0; i < info->texCount; i++) {
                        info->textures[i] = textures[i];
                    }
                    textures.clear();
                }

                if(info->lambdaCount > 0) {
                    info->lambdas = new std::function<char*(unsigned int*)>[info->lambdaCount];
                    for(unsigned int i = 0; i < info->lambdaCount; i++) {
                        info->lambdas[i] = lambdas[i];
                    }
                    lambdas.clear();
                }

                if(info->customStructsCount > 0) {
                    info->customStructs = new char*[info->customStructsCount];
                    for(unsigned int i = 0; i < info->customStructsCount; i++) {
                        info->customStructs[i] = customStructs[i];
                    }
                    customStructs.clear();
                }

                if(info->customStructsCount > 0) {
                    info->customStructsSize = new unsigned int[info->customStructsCount];
                    for(unsigned int i = 0; i < info->customStructsCount; i++) {
                        info->customStructsSize[i] = customStructsSize[i];
                    }
                    customStructsSize.clear();
                }

                if(info->orderCount > 0) {
                    info->order = new unsigned int*[info->orderCount];
                    for(unsigned int i = 0; i < info->orderCount; i++) {
                        info->order[i] = new unsigned int[2];
                        info->order[i][0] = order[i][0];
                        info->order[i][1] = order[i][1];
                    }
                    order.clear();
                }
				return info;
			}

            void ShaderInfoFactory::LoadShaderInformation(ShaderInfo* info) {
				for(unsigned int i = 0; i < info->texCount; i++) {
					textures.push_back(info->textures[i]);
				}
				for(unsigned int i = 0; i < info->customStructsCount; i++) {
					customStructs.push_back(info->customStructs[i]);
					customStructsSize.push_back(info->customStructsSize[i]);
				}
				for(unsigned int i = 0; i < info->lambdaCount; i++) {
					lambdas.push_back(info->lambdas[i]);
				}
				for(unsigned int i = 0; i < info->orderCount; i++) {
					unsigned int orderSourceNum[2] = {info->order[i][0], info->order[i][1]};
					order.push_back(orderSourceNum);
				}

                affectedShaderType = info->affectedShaderType;
                affectedShader = info->shaderNameHash;
                noWorldRotation = info->noWorldRotation;
			}
		}
	}
}
