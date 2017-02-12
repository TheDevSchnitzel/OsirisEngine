#ifndef ISCENENODE_HPP_
#define ISCENENODE_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Scenes {
			class ISceneNode {
				protected:
					OsirisI::Utilities::HashedString name;
					ISceneNode* parentNode;
					std::vector<ISceneNode*> childNodes;


				public:
					virtual ~ISceneNode() {};
                    virtual ReturnState Update(float delta) {
                        for(unsigned int i = 0; i < childNodes.size(); i++) {
                            OS_CHECKSTATE(childNodes[i]->Update(delta));             
                        }
                        return OS_OK;
                    }

					virtual ReturnState Recover() = 0;
					virtual ReturnState Release() = 0;
					
					virtual std::vector<OsirisI::Graphics::Renderer::RenderInfo*> GetRenderInfos() = 0;

					std::vector<ISceneNode*> GetChildNodes() {
						return this->childNodes;
					}
					void AddChildNodes(std::vector<ISceneNode*> nodes) {
						for(unsigned int i = 0; i < nodes.size(); i++) {
							AddChildNode(nodes[i]);
						}
					}
					void AddChildNode(ISceneNode* node) {
						if(std::find(this->childNodes.begin(), this->childNodes.end(), node) != this->childNodes.end()) return;
						this->childNodes.push_back(node);
						node->SetParentNode(this);
					}

					ISceneNode* GetParentNode() {
						return parentNode;
					}
					void SetParentNode(ISceneNode* node) {
						this->parentNode = node;
					}			
			};
		}
	}
}

#endif /* ISCENENODE_HPP_ */
