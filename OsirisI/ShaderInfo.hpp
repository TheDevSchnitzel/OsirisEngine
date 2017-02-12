#ifndef SHADERINFO_HPP_
#define SHADERINFO_HPP_

#include <functional>
#include <memory>

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
			class ITexture;
		}
		namespace Shader {	
			enum ShaderTypes {ENUM_START = 0, DX11_COMPUTE_SHADER, DX11_VERTEX_SHADER, DX11_HULL_SHADER, DX11_DOMAIN_SHADER, DX11_GEOMETRY_SHADER, DX11_PIXEL_SHADER, ENUM_END};
			struct ShaderInfo { //Generated struct which is used for the renderer
				OsirisI::Graphics::Actors::ITexture** textures; //0
				std::function<char*(unsigned int*)>* lambdas; //1  char*(unsigned int* size) -> Data(pointer for datasize)

				char** customStructs; //2
				unsigned int* customStructsSize;

				unsigned int** order;
				long shaderNameHash;
                bool noWorldRotation;
				ShaderTypes affectedShaderType;
                
				    unsigned int texCount, customStructsCount, lambdaCount, orderCount;

				ShaderInfo() {
					order = nullptr;
					customStructs = nullptr;
					customStructsSize = nullptr;
					textures = nullptr;
					lambdas = nullptr;
					shaderNameHash = 0;
                    noWorldRotation = false;

                    texCount = 0, customStructsCount = 0, lambdaCount = 0, orderCount = 0;
                }
                ShaderInfo( const ShaderInfo& other ) : shaderNameHash( other.shaderNameHash), affectedShaderType(other.affectedShaderType), texCount( other.texCount), customStructsCount( other.customStructsCount), lambdaCount( other.lambdaCount), orderCount( other.orderCount), noWorldRotation(other.noWorldRotation) {
                    order = nullptr;
                    customStructs = nullptr;
                    customStructsSize = nullptr;
                    textures = nullptr;
                    lambdas = nullptr;

                    if(customStructsCount > 0) {
                        customStructs = new char*[customStructsCount];
                        customStructsSize = new unsigned int[customStructsCount];
                    }
                    if (texCount > 0) textures = new OsirisI::Graphics::Actors::ITexture*[texCount];
                    if(lambdaCount > 0) lambdas = new std::function<char*(unsigned int*)>[lambdaCount];
                    

                    if(orderCount > 0) order = new unsigned int*[orderCount];
                    for(unsigned int i = 0; i < orderCount; i++) {
                        order[i] = new unsigned int[2];
                        order[i][0] = other.order[i][0];
                        order[i][1] = other.order[i][1];
                    }


                    for(unsigned int i = 0; i < customStructsCount; i++) {
                        customStructs[i] = other.customStructs[i];
                        customStructsSize[i] = other.customStructsSize[i];
                    }
                    for(unsigned int i = 0; i < texCount; i++) textures[i] = other.textures[i];
                    for(unsigned int i = 0; i < lambdaCount; i++) lambdas[i] = other.lambdas[i];
                }
				~ShaderInfo() {
                    Release();
				}

                ShaderInfo& operator= (ShaderInfo const& rhs) {
                    texCount = rhs.texCount;
                    customStructsCount = rhs.customStructsCount;
                    lambdaCount = rhs.lambdaCount;
                    orderCount = rhs.orderCount;
                    affectedShaderType = rhs.affectedShaderType;
                    shaderNameHash = rhs.shaderNameHash;
                    noWorldRotation = rhs.noWorldRotation;

                    Release();

					order = nullptr;
					customStructs = nullptr;
					customStructsSize = nullptr;
					textures = nullptr;
					lambdas = nullptr;
                    
                    if(customStructsCount > 0) {
                        customStructs = new char*[customStructsCount];
                        customStructsSize = new unsigned int[customStructsCount];
                    }
                    if (texCount > 0) textures = new OsirisI::Graphics::Actors::ITexture*[texCount];
                    if(lambdaCount > 0) lambdas = new std::function<char*(unsigned int*)>[lambdaCount];
                    
                    if(orderCount > 0) order = new unsigned int*[orderCount];
                    for(unsigned int i = 0; i < orderCount; i++) {
                        order[i] = new unsigned int[2];
                        order[i][0] = rhs.order[i][0];
                        order[i][1] = rhs.order[i][1];
                    }

                    for(unsigned int i = 0; i < customStructsCount; i++) {
                        customStructs[i] = rhs.customStructs[i];
                        customStructsSize[i] = rhs.customStructsSize[i];
                    }
                    for (unsigned int i = 0; i < texCount; i++) textures[i] = rhs.textures[i];
                    for (unsigned int i = 0; i < lambdaCount; i++) lambdas[i] = rhs.lambdas[i];

                    return *this;
                }

                void Release() {
                    if(textures != nullptr) { 
                        delete[] textures; //Only delete the array, instances must be kept in memory!
                        textures = nullptr;
                    }
                    if (customStructs != nullptr) {
                        for (unsigned int i = 0; i < customStructsCount; i++) {
                            delete[] customStructs[i];
                            customStructs[i] = nullptr;
                        }
                        delete[] customStructs;
                    }
                    if(customStructsSize != nullptr) {
                        delete[] customStructsSize;
                        customStructsSize = nullptr;
                    }
                    if(lambdas != nullptr) {
                        delete[] lambdas;
                        lambdas = nullptr;
                    }
                    
                    if(order != nullptr) {
                        for(unsigned int i = 0; i < orderCount; i++) {
                            if(order[i] != nullptr) delete[] order[i];
                            order[i] = nullptr;
                        }
                        delete[] order;
                        order = nullptr;
                    }
                }
			};

		}
	}
}

#endif /* SHADERINFO_HPP_ */
