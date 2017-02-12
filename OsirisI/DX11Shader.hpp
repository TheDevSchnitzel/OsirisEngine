#ifndef SHADER_HPP_
#define SHADER_HPP_

#include "IShader.hpp"

namespace OsirisI {
	namespace Graphics {
		namespace Shader {
			enum ShaderTypes;
			class DX11Shader : public IShader {
				private:
					ID3D11Buffer *vertexShaderBuffer, *pixelShaderBuffer;
					ID3D11InputLayout *inputLayout;
		            ID3D11Buffer * matrixBuffer;
					std::map<ShaderTypes, std::vector<ID3D11SamplerState*>> samplerStates;
					

					ID3D11ComputeShader * computeShader;
					ID3D11VertexShader * vertexShader;
					ID3D11HullShader * hullShader;
					ID3D11DomainShader * domainShader;
					ID3D11GeometryShader * geometryShader;
					ID3D11PixelShader * pixelShader;

					//ShaderParameter Buffers
					std::vector<ID3D11Buffer*> buffers;
					ReturnState SetShaderConstantBuffer(char* data, unsigned int dataSize, unsigned int loopCount, ShaderTypes type);

                    ReturnState CreateMatrixBuffer(D3D11_BUFFER_DESC bufferDesc);

				public:
					DX11Shader(OsirisI::Utilities::HashedString shaderName);
					DX11Shader(OsirisI::Utilities::HashedString shaderName, D3D11_BUFFER_DESC bufferDesc);
					~DX11Shader();

					ReturnState CreateShader(std::string shaderPath, std::string shaderMainFunctionName, std::string shaderVersion, ShaderTypes type);
			        ReturnState CreateSamplerState(D3D11_SAMPLER_DESC samplerDesc, ShaderTypes type);
			
					ReturnState LoadShaderInfo(); //Ehem. SetShaderParameter -> Überprüfe ob nur einmal auszuführen (1xLoad, NxRendern oder NxLoad, NxRendern)
					ReturnState Render();
					ReturnState SetActive();
                    ReturnState SetParameterMatrices(OMatrix4x4 projection_or_orthoMatrix, OMatrix4x4 worldMatrix, OMatrix4x4 viewMatrix);
					
					void SetInputLayout (ID3D11InputLayout* layout);

					/**
					  Using this function you can generate an InputLayout from the compiled VertexShader.
					*/
					ReturnState CreateInputLayoutFromShader(ID3DBlob* shader);
					

					#pragma region SamplerStates
					/**
					  This function allows to register multiple SamplerStates for the ComputeShader.
					  Every SamplerState will be added to the shader in the SetActive() phase.
					*/
					void CSRegisterSamplerState(ID3D11SamplerState* states);
					/**
					  This function allows to register multiple SamplerStates for the VertexShader.
					  Every SamplerState will be added to the shader in the SetActive() phase.
					*/
					void VSRegisterSamplerState(ID3D11SamplerState* states);
					/**
					  This function allows to register multiple SamplerStates for the HullShader.
					  Every SamplerState will be added to the shader in the SetActive() phase.
					*/
					void HSRegisterSamplerState(ID3D11SamplerState* states);
					/**
					  This function allows to register multiple SamplerStates for the DomainShader.
					  Every SamplerState will be added to the shader in the SetActive() phase.
					*/
					void DSRegisterSamplerState(ID3D11SamplerState* states);
					/**
					  This function allows to register multiple SamplerStates for the GeometryShader.
					  Every SamplerState will be added to the shader in the SetActive() phase.
					*/
					void GSRegisterSamplerState(ID3D11SamplerState* states);
					/**
					  This function allows to register multiple SamplerStates for the PixelShader.
					  Every SamplerState will be added to the shader in the SetActive() phase.
					*/
					void PSRegisterSamplerState(ID3D11SamplerState* states);

					
					/**
					  Clears the registered SamplerStates for the ComputeShader.
					*/
					void CSClearSamplerStates();
					/**
					  Clears the registered SamplerStates for the VertexShader.
					*/
					void VSClearSamplerStates();
					/**
					  Clears the registered SamplerStates for HullSahder ComputeShader.
					*/
					void HSClearSamplerStates();
					/**
					  Clears the registered SamplerStates for the DomainShader.
					*/
					void DSClearSamplerStates();
					/**
					  Clears the registered SamplerStates for the GeometryShader.
					*/
					void GSClearSamplerStates();
					/**
					  Clears the registered SamplerStates for the PixelShader.
					*/
					void PSClearSamplerStates();
					#pragma endregion
			};
		}
	}
}

#endif /* SHADER_HPP_ */
