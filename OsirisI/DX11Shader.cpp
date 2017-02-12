#include "stdafx.h"

using namespace OsirisI::Resources;
using namespace OsirisI::Utilities;
using namespace OsirisI::Manager;
using namespace OsirisI::Graphics::Devices;
using namespace OsirisI::Graphics::Actors;



namespace OsirisI {
	namespace Graphics {
		namespace Shader {
			DX11Shader::DX11Shader(HashedString shaderName) {
				ID3D11Device* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDevice();
				ID3D11DeviceContext* devCon = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDeviceContext();

				this->shaderName = shaderName;
                this->computeShader = nullptr;
				this->vertexShader = nullptr;
				this->pixelShader = nullptr;
				this->hullShader = nullptr;
				this->domainShader = nullptr;
				this->geometryShader = nullptr;
				
				this->samplerStates.insert(std::pair<ShaderTypes,std::vector<ID3D11SamplerState*>>(DX11_COMPUTE_SHADER, std::vector<ID3D11SamplerState*>()));
				this->samplerStates.insert(std::pair<ShaderTypes,std::vector<ID3D11SamplerState*>>(DX11_VERTEX_SHADER, std::vector<ID3D11SamplerState*>()));
				this->samplerStates.insert(std::pair<ShaderTypes,std::vector<ID3D11SamplerState*>>(DX11_HULL_SHADER, std::vector<ID3D11SamplerState*>()));
				this->samplerStates.insert(std::pair<ShaderTypes,std::vector<ID3D11SamplerState*>>(DX11_DOMAIN_SHADER, std::vector<ID3D11SamplerState*>()));
				this->samplerStates.insert(std::pair<ShaderTypes,std::vector<ID3D11SamplerState*>>(DX11_GEOMETRY_SHADER, std::vector<ID3D11SamplerState*>()));
				this->samplerStates.insert(std::pair<ShaderTypes,std::vector<ID3D11SamplerState*>>(DX11_PIXEL_SHADER, std::vector<ID3D11SamplerState*>()));


                D3D11_BUFFER_DESC matrixBufferDesc;
                matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
                matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
                matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                matrixBufferDesc.MiscFlags = 0;
                matrixBufferDesc.StructureByteStride = 0;                
                OS_CHECKSTATE_THROW(CreateMatrixBuffer(matrixBufferDesc));
            }
            
            DX11Shader::DX11Shader(HashedString shaderName, D3D11_BUFFER_DESC matrixBufferDesc) {
                OS_CHECKSTATE_THROW(CreateMatrixBuffer(matrixBufferDesc));
            }

            ReturnState DX11Shader::CreateMatrixBuffer(D3D11_BUFFER_DESC bufferDesc) {
				ID3D11Device* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDevice();
				ID3D11DeviceContext* devCon = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDeviceContext();
                
                // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
                HRESULT hResult = dev->CreateBuffer(&bufferDesc, NULL, &this->matrixBuffer);
                if(FAILED(hResult)) {
                    LOG_ERROR("Shader construction failed, HRESULT: " << std::hex << hResult << ", shaderPTR: " << std::hex << this);
                    return OS_DIRECTXERROR;
                }

                return OS_OK;
            }

			DX11Shader::~DX11Shader() {
                if(this->computeShader != nullptr) this->computeShader->Release();
				if(this->vertexShader != nullptr) this->vertexShader->Release();
				if(this->hullShader != nullptr) this->hullShader->Release();
				if(this->domainShader != nullptr) this->domainShader->Release();
				if(this->geometryShader != nullptr) this->geometryShader->Release();
				if(this->pixelShader != nullptr) this->pixelShader->Release();
                this->computeShader = nullptr;
				this->vertexShader = nullptr;
				this->pixelShader = nullptr;
				this->hullShader = nullptr;
				this->domainShader = nullptr;
				this->geometryShader = nullptr;
			}

			ReturnState DX11Shader::CreateShader(std::string shaderPath, std::string shaderMainFunctionName, std::string shaderVersion, ShaderTypes type) {
				ID3D11Device* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDevice();
				ID3D11DeviceContext* devCon = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDeviceContext();

				ID3D10Blob* tmpBlob = NULL;
				DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
				ID3D10Blob* pErrorBlob = NULL;
				HRESULT hr = S_OK;
	
				if (shaderMainFunctionName != "" && shaderPath != "" && shaderVersion != "") {
					ResourcePtr<char*>* shaderCode = static_cast<ResourcePtr<char*>*>(ResourceManager::GetResourceDirectly(shaderPath));
					if(shaderCode == NULL) return 0x1A;
					
					hr = D3DCompile(shaderCode->GetValue(), shaderCode->GetSize(), shaderMainFunctionName.c_str(), 0, 0, shaderMainFunctionName.c_str(), shaderVersion.c_str(), dwShaderFlags, NULL, &tmpBlob, &pErrorBlob);
					if(FAILED(hr)) {
						if( pErrorBlob != NULL ) {
							LOG_ERROR("[0x25]Error during creation of shader: " << this->shaderName.GetString() << "\n" << (char*)pErrorBlob->GetBufferPointer());
						}
						if( pErrorBlob ) pErrorBlob->Release();
						if(shaderCode != NULL) shaderCode->Release();
						pErrorBlob = NULL;
						return 0x25;
					}

					switch(type) {
						case DX11_COMPUTE_SHADER: {
							hr = dev->CreateComputeShader(tmpBlob->GetBufferPointer(), tmpBlob->GetBufferSize(), NULL, &this->computeShader);
							break;
						}
						case DX11_VERTEX_SHADER: {
							hr = dev->CreateVertexShader(tmpBlob->GetBufferPointer(), tmpBlob->GetBufferSize(), NULL, &this->vertexShader);
							if(SUCCEEDED(hr)) {
								ReturnState tmpReturnState = CreateInputLayoutFromShader(tmpBlob);
								if(tmpReturnState != 0x0) {
									LOG_ERROR("[" << std::hex << tmpReturnState << "]Error in CreateInputLayoutFromShader: " << this->shaderName.GetString());
								}
							}
							break;
						}
						case DX11_HULL_SHADER: {
							hr = dev->CreateHullShader(tmpBlob->GetBufferPointer(), tmpBlob->GetBufferSize(), NULL, &this->hullShader);
							break;
						}
						case DX11_DOMAIN_SHADER: {
							hr = dev->CreateDomainShader(tmpBlob->GetBufferPointer(), tmpBlob->GetBufferSize(), NULL, &this->domainShader);
							break;
						}
						case DX11_GEOMETRY_SHADER: {
							hr = dev->CreateGeometryShader(tmpBlob->GetBufferPointer(), tmpBlob->GetBufferSize(), NULL, &this->geometryShader);
							break;
						}
						case DX11_PIXEL_SHADER: {
                            hr = dev->CreatePixelShader(tmpBlob->GetBufferPointer(), tmpBlob->GetBufferSize(), NULL, &this->pixelShader);
							break;
						}
						default: break;
					}

					if(FAILED(hr)) {
						LOG_ERROR("[0x25]Error during creation of shader: " << this->shaderName.GetString() << "\nHRESULT: " << std::hex << hr << "\nShaderType: " << type);
						tmpBlob->Release();
						tmpBlob = NULL;
						shaderCode->Release();
						return 0x25;
					}	

					tmpBlob->Release();
					tmpBlob = NULL;
					shaderCode->Release();
				}
				return OS_OK;
			}

            ReturnState DX11Shader::CreateSamplerState(D3D11_SAMPLER_DESC samplerDesc, ShaderTypes type) {
                ID3D11Device* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDevice();
                ID3D11DeviceContext* devCon = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDeviceContext();
               
                if(samplerStates.find(type) == samplerStates.end()) {
                    std::vector<ID3D11SamplerState*> tmpVector;
                    samplerStates.insert(std::pair<ShaderTypes,std::vector<ID3D11SamplerState*>>(type, tmpVector));
                }
				
                samplerStates[type].push_back(nullptr);
                HRESULT hr = dev->CreateSamplerState(&samplerDesc, &samplerStates[type][samplerStates[type].size()-1]);
                if(FAILED(hr)) {
                    LOG_ERROR("CreateSamplerState failed - Hr: " << std::hex << hr);
                    return OS_DIRECTXERROR;
                }
                
                return OS_OK;
            }

			ReturnState DX11Shader::SetActive() {
				ID3D11Device* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDevice();
				ID3D11DeviceContext* devCon = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDeviceContext();
						
                OS_CHECKSTATE(LoadShaderInfo());

				if(this->computeShader != nullptr) {
					devCon->CSSetShader(computeShader, NULL, 0);
					if(this->samplerStates[DX11_COMPUTE_SHADER].size() > 0) devCon->CSSetSamplers(0,this->samplerStates[DX11_COMPUTE_SHADER].size(),this->samplerStates[DX11_COMPUTE_SHADER].data());
				}
				//else devCon->CSSetShader(0,0,0); //Reset shader

				if(this->vertexShader != nullptr) {
					devCon->IASetInputLayout(this->inputLayout);
					devCon->VSSetShader(vertexShader, NULL, 0);
					
                    if(this->samplerStates[DX11_VERTEX_SHADER].size() > 0) devCon->VSSetSamplers(0,this->samplerStates[DX11_VERTEX_SHADER].size(),this->samplerStates[DX11_VERTEX_SHADER].data());
				}
				//else devCon->VSSetShader(0,0,0); //Reset shader

				if(this->hullShader != nullptr) {
					devCon->HSSetShader(hullShader, NULL, 0);
					if(this->samplerStates[DX11_HULL_SHADER].size() > 0) devCon->HSSetSamplers(0,this->samplerStates[DX11_HULL_SHADER].size(),this->samplerStates[DX11_HULL_SHADER].data());
				}
				//else devCon->HSSetShader(0,0,0); //Reset shader

				if(this->domainShader != nullptr) {
					devCon->DSSetShader(domainShader, NULL, 0);
					if(this->samplerStates[DX11_DOMAIN_SHADER].size() > 0) devCon->DSSetSamplers(0,this->samplerStates[DX11_DOMAIN_SHADER].size(),this->samplerStates[DX11_DOMAIN_SHADER].data());
				}
				//else devCon->DSSetShader(0,0,0); //Reset shader
				
				if(this->geometryShader != nullptr) {
					devCon->GSSetShader(geometryShader, NULL, 0);
					if(this->samplerStates[DX11_GEOMETRY_SHADER].size() > 0) devCon->GSSetSamplers(0,this->samplerStates[DX11_GEOMETRY_SHADER].size(),this->samplerStates[DX11_GEOMETRY_SHADER].data());
				}
				//else devCon->GSSetShader(0,0,0); //Reset shader

				if(this->pixelShader != nullptr) {
					devCon->PSSetShader(pixelShader, NULL, 0);
					if(this->samplerStates[DX11_PIXEL_SHADER].size() > 0) {
                        for(unsigned int i = 0; i < this->samplerStates[DX11_PIXEL_SHADER].size(); i++) {
                            devCon->PSSetSamplers(i,1,&this->samplerStates[DX11_PIXEL_SHADER][i]);
                        }
                    }
				}
				//else devCon->PSSetShader(0,0,0); //Reset shader

				return OS_OK;
			}
            
            ReturnState DX11Shader::SetParameterMatrices(OMatrix4x4 projection_or_orthoMatrix, OMatrix4x4 worldMatrix, OMatrix4x4 viewMatrix) {
				ID3D11Device* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDevice();
				ID3D11DeviceContext* devCon = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDeviceContext();

                //Make sure to transpose matrices before sending them into the shader, this is a requirement for DirectX 11.
                // Transpose the matrices to prepare them for the shader.
                worldMatrix.Transpose();
                viewMatrix.Transpose();
                projection_or_orthoMatrix.Transpose();
                
	            D3D11_MAPPED_SUBRESOURCE mappedResource;
                MatrixBufferType* dataPtr;
                HRESULT hResult = devCon->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
                if(FAILED(hResult)) return OS_DIRECTXERROR;

                // Get a pointer to the data in the constant buffer.
                dataPtr = (MatrixBufferType*)mappedResource.pData;

                // Copy the matrices into the constant buffer.
                dataPtr->world = worldMatrix;
                dataPtr->view = viewMatrix;
                dataPtr->projection = projection_or_orthoMatrix;
                
                // Unlock the constant buffer.
                devCon->Unmap(matrixBuffer, 0);

                devCon->VSSetConstantBuffers(0, 1, &matrixBuffer);

                return OS_OK;
            }

			ReturnState DX11Shader::LoadShaderInfo() {
				ID3D11Device* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDevice();
				ID3D11DeviceContext* devCon = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDeviceContext();
				for(unsigned int j = 0; j < this->infoList.size(); j++) {
					ShaderInfo* info = infoList[j];
					int textureIndex = 0, bufferCount = 0;
                    for(unsigned int i = 0; i < info->orderCount; i++) {
						ReturnState state = OS_OK;
						switch(info->order[i][0]) {
							case 0: { //ITexture*
                                DX11Texture* texture = (DX11Texture*)info->textures[info->order[i][1]];
								ID3D11ShaderResourceView* shaderTex = texture->GetRawTexture();
								if(info->affectedShaderType == DX11_COMPUTE_SHADER) devCon->CSSetShaderResources(textureIndex, 1, &shaderTex);
								else if(info->affectedShaderType == DX11_VERTEX_SHADER) devCon->VSSetShaderResources(textureIndex, 1, &shaderTex);
								else if(info->affectedShaderType == DX11_HULL_SHADER) devCon->HSSetShaderResources(textureIndex, 1, &shaderTex);
								else if(info->affectedShaderType == DX11_DOMAIN_SHADER) devCon->DSSetShaderResources(textureIndex, 1, &shaderTex);
								else if(info->affectedShaderType == DX11_GEOMETRY_SHADER) devCon->GSSetShaderResources(textureIndex, 1, &shaderTex);
								else if(info->affectedShaderType == DX11_PIXEL_SHADER) devCon->PSSetShaderResources(textureIndex, 1, &shaderTex);
								textureIndex++;
								break;
							}
							case 1: { //std::function<char*(unsigned int*)>
								unsigned int* dataSize = 0;
                                OS_CHECKSTATE(SetShaderConstantBuffer(info->lambdas[info->order[i][1]](dataSize), *dataSize, i, info->affectedShaderType));
                                bufferCount++;
                                break;
                            }
                            case 2: { //char*
                                OS_CHECKSTATE(SetShaderConstantBuffer(info->customStructs[info->order[i][1]], info->customStructsSize[info->order[i][1]], bufferCount, info->affectedShaderType));
                                bufferCount++;
                                break;
							}
							default: break;
						}
					}
				}
				return OS_OK;
			}

			ReturnState DX11Shader::SetShaderConstantBuffer(char* data, unsigned int dataSize, unsigned int loopCount, ShaderTypes type) {
				ID3D11Device* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDevice();
				ID3D11DeviceContext* devCon = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDeviceContext();
				HRESULT result;

				if(buffers.size() == loopCount) { //Only create buffer if there isnt already one for this round
					ID3D11Buffer* tmpBuffer;
					D3D11_BUFFER_DESC* tmpBufferDesc = new D3D11_BUFFER_DESC();
					tmpBufferDesc->Usage = D3D11_USAGE_DYNAMIC;
					tmpBufferDesc->ByteWidth = dataSize;
					tmpBufferDesc->BindFlags = D3D11_BIND_CONSTANT_BUFFER;
					tmpBufferDesc->CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
					tmpBufferDesc->MiscFlags = 0;
                    tmpBufferDesc->StructureByteStride = 0;

					result = dev->CreateBuffer(tmpBufferDesc, NULL, &tmpBuffer);
                    delete tmpBufferDesc;
					if(FAILED(result)) return OS_BUFFER_NOT_CREATED; 
					buffers.push_back(tmpBuffer);
				}

				ID3D11Buffer* buffer = buffers[loopCount];
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				// Lock the constant buffer so it can be written to.
				result = devCon->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				if(FAILED(result)) return OS_RESOURCEMAPPING_FAILED;

				//Copy data to the pointer of the constant buffer.
                memcpy_s(mappedResource.pData, dataSize, data, dataSize);

				// Unlock the constant buffer.
				devCon->Unmap(buffer, 0);

				//Set buffer
				if(type == DX11_COMPUTE_SHADER) devCon->CSSetConstantBuffers(loopCount, 1, &buffer);
				else if(type == DX11_VERTEX_SHADER) devCon->VSSetConstantBuffers(loopCount, 1, &buffer);
				else if(type == DX11_HULL_SHADER) devCon->HSSetConstantBuffers(loopCount, 1, &buffer);
				else if(type == DX11_DOMAIN_SHADER) devCon->DSSetConstantBuffers(loopCount, 1, &buffer);
				else if(type == DX11_GEOMETRY_SHADER) devCon->GSSetConstantBuffers(loopCount, 1, &buffer);
				else if(type == DX11_PIXEL_SHADER) devCon->PSSetConstantBuffers(loopCount, 1, &buffer);
				return OS_OK;
			}

			void DX11Shader::SetInputLayout (ID3D11InputLayout* layout) {
				this->inputLayout = layout;
			}

			ReturnState DX11Shader::CreateInputLayoutFromShader(ID3DBlob* shader) {
				ID3D11Device* dev = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDevice();
				ID3D11DeviceContext* devCon = ((OsirisI::Graphics::Devices::Direct3DDevice*)IGraphicsDevice::g_graphicDevice)->GetDeviceContext();

				// Reflect shader info
				ID3D11ShaderReflection* pShaderReflection = NULL;	
				if(FAILED(D3DReflect(shader->GetBufferPointer(), shader->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pShaderReflection))) {
					LOG_ERROR("[0x26] Reflection failed for extracting ID3D11InputLayout from shader blob");
					return 0x26;
				}

				// Get shader info
				D3D11_SHADER_DESC shaderDesc;
				pShaderReflection->GetDesc(&shaderDesc);

				// Read input layout description from shader info
				unsigned int byteOffset = 0;
				std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
				for ( unsigned int i = 0; i < shaderDesc.InputParameters; i++ )	{
					D3D11_SIGNATURE_PARAMETER_DESC paramDesc;		
					pShaderReflection->GetInputParameterDesc(i, &paramDesc );

					// fill out input element desc
					D3D11_INPUT_ELEMENT_DESC elementDesc;	
					elementDesc.SemanticName = paramDesc.SemanticName;		
					elementDesc.SemanticIndex = paramDesc.SemanticIndex;
					elementDesc.InputSlot = 0;
					elementDesc.AlignedByteOffset = byteOffset;
					elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
					elementDesc.InstanceDataStepRate = 0;	

					// determine DXGI format
					if ( paramDesc.Mask == 1 ) {
						if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32_UINT;
						else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32_SINT;
						else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
						byteOffset += 4;
					}
					else if ( paramDesc.Mask <= 3 ) {
						if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
						else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
						else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
						byteOffset += 8;
					}
					else if ( paramDesc.Mask <= 7 ) {
						if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
						else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
						else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
						byteOffset += 12;
					}
					else if ( paramDesc.Mask <= 15 ) {
						/*if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
						else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
						else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
						byteOffset += 16;*/

                        //The following code is the same as in the else if block above...
                        //Due to the internal structure of the OVertex struct (OVector3 for position of the vertex)
						if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
						else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
						else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
						byteOffset += 12;
					}

					//save element desc
					inputLayoutDesc.push_back(elementDesc);
				}		


                ReturnState state = OS_OK;

				// Try to create Input Layout
				HRESULT hr = dev->CreateInputLayout( &inputLayoutDesc[0], inputLayoutDesc.size(), shader->GetBufferPointer(), shader->GetBufferSize(), &this->inputLayout );
                if(FAILED(hr)) {
                    LOG_ERROR("CreateInputLayout failed - HRESULT: " << std::hex << hr);
                    state = OS_DIRECTXERROR;
                }

				//Free allocation shader reflection memory
				pShaderReflection->Release();

				return state;
			}
			
			void DX11Shader::CSRegisterSamplerState(ID3D11SamplerState* states) {
				this->samplerStates[DX11_COMPUTE_SHADER].push_back(states);
			}
			void DX11Shader::VSRegisterSamplerState(ID3D11SamplerState* states) {
				this->samplerStates[DX11_VERTEX_SHADER].push_back(states);
			}
			void DX11Shader::HSRegisterSamplerState(ID3D11SamplerState* states) {
				this->samplerStates[DX11_HULL_SHADER].push_back(states);
			}
			void DX11Shader::DSRegisterSamplerState(ID3D11SamplerState* states) {
				this->samplerStates[DX11_DOMAIN_SHADER].push_back(states);
			}
			void DX11Shader::GSRegisterSamplerState(ID3D11SamplerState* states) {
				this->samplerStates[DX11_GEOMETRY_SHADER].push_back(states);
			}
			void DX11Shader::PSRegisterSamplerState(ID3D11SamplerState* states) {
				this->samplerStates[DX11_PIXEL_SHADER].push_back(states);
			}
		}
	}
}
