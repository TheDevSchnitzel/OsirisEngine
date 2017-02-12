#ifndef IGAMEMODEL_HPP_
#define IGAMEMODEL_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
			class IMesh;
			class ITexture;
			class IGameModel : public MovingActor {
				protected:
					IGameModel(OsirisI::Utilities::HashedString name) : MovingActor(name) {}
					std::vector<IMesh*> meshes;
                    ITexture* texture;
                    
				public:
                    virtual void AddMesh(IMesh* mesh) = 0;
                    virtual void RemoveMesh(IMesh* mesh) = 0;
					virtual void RemoveMesh(long hashedMeshName) = 0;
                    virtual std::vector<IMesh*> GetMeshes() = 0;
					
                    ITexture* GetTexture() {
                        return this->texture;
                    }

                    void SetTexture(ITexture* tex) {
                        this->texture = tex;
                    }

                    IGameModel* GetGameModel() {
                        return this;
                    }
            };
        }
    }
}

#endif /* IGAMEMODEL_HPP_ */