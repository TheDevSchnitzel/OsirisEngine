#ifndef IMESH_HPP_
#define IMESH_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
			/*
            This class is NOT derived from IActor!
            A Mesh is just a data structure containing the material of a model which is rendered -> IGameModel
            */
            class IMesh : public IOsirisObject {
				private:

				protected:
					IMesh(OsirisI::Utilities::HashedString name) : IOsirisObject(name) {}
					OsirisI::Graphics::Shader::ShaderInfo shaderInfo;
					std::string containerPath;

                    //This is needed to calculate the position inside the whiole gamemodel -> Both Vectors are relative to the gamemodels data -> (gamemodel.x + mesh.x) == mesh's x-pos
                    OVector3 position, rotation;

				public:
					std::string GetContainerPath() {
						return this->containerPath;
					}
                    
					OsirisI::Graphics::Shader::ShaderInfo GetShaderInfo() {
						return this->shaderInfo;
					}
					void SetShaderInfo(OsirisI::Graphics::Shader::ShaderInfo shaderInfo) {
						this->shaderInfo = shaderInfo;
					}
                                        
                    OsirisI::OVector3 GetPosition() {
                        return this->position;
                    }
                    void SetPosition(OsirisI::OVector3 position) {
                        this->position = position;
                    }

                    OsirisI::OVector3 GetRotation() {
                        return this->rotation;
                    }
                    void SetRotation(OsirisI::OVector3 rotation) {
                        this->rotation = rotation;
                    }

					//No rendering function, this will be handled by the renderer (Mesh packed into GameModel -> packed into RenderInfo)
					virtual ReturnState Update(float delta) = 0;
					virtual ReturnState Recover() = 0;
					virtual ReturnState Release() = 0;
                    
                    virtual OsirisI::OVertex* GetVertices() = 0;
			};
		}
	}
}

#endif /* IMESH_HPP_ */