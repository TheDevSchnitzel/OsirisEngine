#include "stdafx.h"

using namespace OsirisI::Utilities;
using namespace OsirisI::Graphics::Renderer;
using namespace OsirisI::Graphics::Shader;
using namespace OsirisI::Resources;
using namespace OsirisI::Manager;

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
            DX11GameModel::DX11GameModel(OVector3 pos, OsirisI::Utilities::HashedString name) : IGameModel(name) {
                this->position = pos;
                this->containerPath = HashedString("");
                this->gameModel = this;
            }

			DX11GameModel::DX11GameModel(OVector3 pos, OsirisI::Utilities::HashedString name, OsirisI::Utilities::HashedString containerPath) : IGameModel(name) {
                this->position = pos;
                this->name = name;
                this->containerPath = containerPath;
                this->gameModel = this;
            }
                   
			DX11GameModel::~DX11GameModel() {
			}
			
            ReturnState DX11GameModel::Initialize() {
                if(this->containerPath.GetString() == "") return OS_INITIALIZATION_FAILED;

                ResourcePtr<IGameModel*>* res = ((ResourcePtr<IGameModel*>*)ResourceManager::GetResource<IGameModel>(this->containerPath));
				if(res == nullptr) {
                    res = ((ResourcePtr<IGameModel*>*)ResourceManager::LoadAndCache<IGameModel>(HashedString(this->containerPath), HashedString(name)));
				}
                this->meshes = res->GetValue()->GetMeshes();
                this->texture = res->GetValue()->GetTexture();
                

/*
                ShaderInfoFactory::AddTexture(this->texture);
                ShaderInfoFactory::SetAffectedShader(HashedString("TextureShader").GetHash());
                ShaderInfoFactory::SetAffectedShaderType(DX11_PIXEL_SHADER);
                RenderInfoFactory::AddShaderInfo(ShaderInfoFactory::CreateShaderInformation());
*/
                struct LightBufferType
                {
                    OVector4 ambientColor;
                    OVector4 diffuseColor;
                    OVector3 lightDirection;
                    float padding;
                };
                LightBufferType* a = new LightBufferType();
                a->ambientColor = OVector4(0.15f, 0.15f, 0.15f, 1.0f);
                a->diffuseColor = OVector4(1.0f, 1.0f, 1.0f, 1.0f);
                a->lightDirection = OVector3(5.0f, 0.0f, 0.0f);

                ShaderInfoFactory::AddTexture(this->texture);
                ShaderInfoFactory::SetAffectedShader(HashedString("LightShader").GetHash());
                ShaderInfoFactory::SetAffectedShaderType(DX11_PIXEL_SHADER);
                ShaderInfoFactory::AddCustomStruct(((char*)a), sizeof(LightBufferType));
                RenderInfoFactory::AddShaderInfo(ShaderInfoFactory::CreateShaderInformation());

                RenderInfoFactory::AddActor(this);
                this->renderInfo = RenderInfoFactory::CreateRenderInformation();
                return OS_OK;
            }

			ReturnState DX11GameModel::Update(float delta) {
				for(unsigned int i = 0; i < this->meshes.size(); i++) {
                    OS_CHECKSTATE(this->meshes[i]->Update(delta));
				}
				this->rotation.x = (this->rotation.x + 0.001 > 360) ? 0 : this->rotation.x + 0.01;
				//this->rotation.y = (this->rotation.y + 0.004 > 360) ? 0 : this->rotation.y + 0.004;
				//this->rotation.z = (this->rotation.z + 0.008 > 360) ? 0 : this->rotation.z + 0.008;
				return OS_OK;
			}

			ReturnState DX11GameModel::Recover() {
				for(unsigned int i = 0; i < this->meshes.size(); i++) {
					OS_CHECKSTATE(this->meshes[i]->Recover());
				}
				return OS_OK;
			}

			ReturnState DX11GameModel::Release() {
				this->meshes.clear();
                if (renderInfo != nullptr) delete renderInfo;
				return OS_OK;
			}
			
			void DX11GameModel::AddMesh(IMesh* mesh) {
				this->meshes.push_back(mesh);
			}

            void DX11GameModel::RemoveMesh(IMesh* mesh) {
				//Evt. RemoveMesh(mesh->GetName().GetHash()); Performance prüfen
				unsigned int iterPoint = -1;
				for(unsigned int i = 0; i < this->meshes.size(); i++) {
					if(this->meshes[i] == mesh) {
						iterPoint = i;
						i = this->meshes.size();
						break;
					}
				}
				if(iterPoint == -1) return;
				this->meshes.erase(this->meshes.begin() + iterPoint);
			}

			void DX11GameModel::RemoveMesh(long hashedMeshName) {
				unsigned int iterPoint = -1;
				for(unsigned int i = 0; i < this->meshes.size(); i++) {
					if(this->meshes[i]->GetName().GetHash() == hashedMeshName) {
						iterPoint = i;
						i = this->meshes.size();
						break;
					}
				}
				if(iterPoint == -1) return;
				this->meshes.erase(this->meshes.begin() + iterPoint);
			}

            std::vector<IMesh*> DX11GameModel::GetMeshes() {
				return this->meshes;
			}			
            
            RenderInfo* DX11GameModel::GetRenderInfo() {
                return this->renderInfo;
			}
		}
	}
}