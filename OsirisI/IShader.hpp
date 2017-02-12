#ifndef ISHADER_HPP_
#define ISHADER_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Shader {
			class IShader {
				private:

				protected:
                    std::vector<ShaderInfo*> infoList;
					OsirisI::Utilities::HashedString shaderName;

				public:
					virtual ReturnState SetActive() = 0;

					std::vector<ShaderInfo*> GetShaderInfos() {
						return this->infoList;
					}
					void SetShaderInfos(std::vector<ShaderInfo*> infoList) {
						this->infoList = infoList;
					}
					
					OsirisI::Utilities::HashedString GetShaderName() {
						return this->shaderName;
					}
			};
		}
	}
}

#endif /* ISHADER_HPP_ */
