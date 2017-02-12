#ifndef ISCENE_HPP_
#define ISCENE_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Scenes {
			class ISceneNode;
			class RootNode;
			class ActorNode;
			class IScene : public IOsirisObject {
				protected:
					RootNode* rootNode; //RootSceneNode
					ActorNode* cameraNode; //CameraSceneNode

				public:
					IScene(OsirisI::Utilities::HashedString name) : IOsirisObject(name) {
					}
					virtual ~IScene() {};

                    virtual ReturnState PrepareForActivation() = 0;
                    virtual ReturnState Update(float delta) = 0;
					virtual ReturnState Recover() = 0;
					virtual ReturnState Release() = 0;

					RootNode* GetRootNode();
					void SetRootNode(RootNode* node);

					ActorNode* GetCameraNode();
					void SetCameraNode(ActorNode* node);
			};
		}
	}
}

#endif /* ISCENE_HPP_ */
