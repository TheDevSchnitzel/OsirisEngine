#ifndef IACTOR_HPP_
#define IACTOR_HPP_

namespace OsirisI {
	namespace Events {
		struct IEvent;
	}
	namespace Graphics {
		namespace Renderer {
			struct RenderInfo;
		}
        namespace Actors {
            class IGameModel;
			class IActor : public IOsirisObject {
				private:
                    
				protected:
					OsirisI::Utilities::HashedString containerPath;
                    OVector3 position, rotation;
                    OsirisI::Graphics::Renderer::RenderInfo* renderInfo;
                    IGameModel* gameModel;

                public:
					IActor(OsirisI::Utilities::HashedString name) : IOsirisObject(name) {
						this->gameModel = nullptr;
						this->renderInfo = nullptr;
						this->position = OVector3(0.f, 0.f, 0.f);
						this->rotation = OVector3(0.f, 0.f, 0.f);
                    }

                    virtual ~IActor() { }

					virtual ReturnState Initialize() = 0;
					virtual ReturnState Update(float delta) = 0;
					virtual ReturnState Recover() = 0;
					virtual ReturnState Release() = 0;

					virtual OsirisI::Graphics::Renderer::RenderInfo* GetRenderInfo() = 0;
					
					ReturnState Persist(OsirisI::Utilities::Persistor* persistor) {
						OS_CHECKSTATE(this->containerPath.Persist(persistor));
						OS_CHECKSTATE(persistor->Persist(position, sizeof(OVector3)));
						OS_CHECKSTATE(persistor->Persist(rotation, sizeof(OVector3)));

						OS_CHECKSTATE(IOsirisObject::Persist(persistor));
						return OS_OK;
					}

					ReturnState Unpersist(OsirisI::Utilities::Persistor* persistor) {
						OS_CHECKSTATE(this->containerPath.Unpersist(persistor));
						OS_CHECKSTATE(persistor->Unpersist(position, sizeof(OVector3)));
						OS_CHECKSTATE(persistor->Unpersist(rotation, sizeof(OVector3)));

						OS_CHECKSTATE(IOsirisObject::Unpersist(persistor));
						return OS_OK;
					}

					virtual ReturnState HandleUserInput(std::shared_ptr<OsirisI::Events::IEvent> e) {
						return OS_OK;
					}

					OsirisI::Utilities::HashedString GetContainerPath() {
						return this->containerPath;
					}
                    
                    OsirisI::OVector3 GetPosition() {
						return OVector3(this->position);
                    }
                    void SetPosition(OsirisI::OVector3 position) {
						this->position = OVector3(position);
                    }
                    OsirisI::OVector3 GetRotation() {
						return OVector3(this->rotation);
                    }
                    void SetRotation(OsirisI::OVector3 rotation) {
						this->rotation = OVector3(rotation);
                    }

                    IGameModel* GetGameModel() {
                        return gameModel;
                    }
			};
		}
	}
}

#endif /*IACTOR_HPP_*/
