#include "stdafx.h"

using namespace OsirisI::Graphics::Shader;
using namespace OsirisI::Graphics::Actors;

namespace OsirisI {
	namespace Graphics {
		namespace Renderer {
			std::vector<OsirisI::Graphics::Shader::ShaderInfo*> RenderInfoFactory::shaderInfos; //0
            std::vector<IActor*> RenderInfoFactory::actors; //1
			std::vector<ID3D11RenderTargetView*> RenderInfoFactory::renderTargets; //2
            bool RenderInfoFactory::turnAlphaBlendingOff = false, RenderInfoFactory::turnZBufferOff = false, RenderInfoFactory::isText = false;

			void RenderInfoFactory::AddShaderInfo(OsirisI::Graphics::Shader::ShaderInfo* info) {
				shaderInfos.push_back(info);
			}
            void RenderInfoFactory::AddActor(IActor* actor) {
                actors.push_back(actor);
			}
            void RenderInfoFactory::AddRenderTarget(ID3D11RenderTargetView* renderTarget) {
				renderTargets.push_back(renderTarget);
			}
            
            void RenderInfoFactory::TurnZBufferOff(bool var) {
                turnZBufferOff = var;
            }

            void RenderInfoFactory::TurnAlphaBlendingOff(bool var) {
                turnAlphaBlendingOff = var;
            }
            void RenderInfoFactory::IsText(bool var){
                isText = var;
            }

			RenderInfo* RenderInfoFactory::CreateRenderInformation() {
				RenderInfo* info = new RenderInfo();

                info->turnAlphaBlendingOff = turnAlphaBlendingOff;
                info->turnZBufferOff = turnZBufferOff;
                info->isText = isText;
				info->shaderInfoCount = shaderInfos.size();
				info->renderTargetCount = renderTargets.size();

                if(info->shaderInfoCount > 0) {
                    info->shaderInfos = new ShaderInfo*[info->shaderInfoCount];
                    for(unsigned int i = 0; i < info->shaderInfoCount; i++) {
                        info->shaderInfos[i] = shaderInfos[i];
                    }
                }

                if(info->renderTargetCount > 0) {
                    info->renderTargets = new ID3D11RenderTargetView*[info->renderTargetCount];
                    for(unsigned int i = 0; i < info->renderTargetCount; i++) {
                        info->renderTargets[i] = renderTargets[i];
                    }
                }

                info->actors = actors;

                shaderInfos.clear();
                renderTargets.clear();
                actors.clear();
                turnAlphaBlendingOff = false;
                turnZBufferOff = false;

				return info;
			}

			void RenderInfoFactory::LoadRenderInformation(RenderInfo* info) {
                for(unsigned int i = 0; i < info->shaderInfoCount; i++) {
					shaderInfos.push_back(info->shaderInfos[i]);
				}

                for(unsigned int i = 0; i < info->renderTargetCount; i++) {
					renderTargets.push_back(info->renderTargets[i]);
				}

                actors = info->actors;
                turnAlphaBlendingOff = info->turnAlphaBlendingOff;
                turnZBufferOff = info->turnZBufferOff;
                isText = info->isText;
			}
        }
	}
}
