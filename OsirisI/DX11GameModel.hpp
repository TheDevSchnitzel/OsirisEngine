#ifndef DX11GAMEMODEL_HPP_
#define DX11GAMEMODEL_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
			class DX11GameModel : public IGameModel {
				public:
                    DX11GameModel(OVector3 pos, OsirisI::Utilities::HashedString name);
                    DX11GameModel(OVector3 pos, OsirisI::Utilities::HashedString name, OsirisI::Utilities::HashedString containerPath);
                     ~DX11GameModel();

                    ReturnState Initialize();
					ReturnState Update(float delta);
					ReturnState Recover();
					ReturnState Release();

                    void AddMesh(IMesh* mesh);
                    void RemoveMesh(IMesh* mesh);
					void RemoveMesh(long hashedMeshName);
                    std::vector<IMesh*> GetMeshes();
					
					OsirisI::Graphics::Renderer::RenderInfo* GetRenderInfo();
			};
		}
	}
}

#endif /* DX11GAMEMODEL_HPP_ */