#ifndef RENDERINFOFACTORY_HPP_
#define RENDERINFOFACTORY_HPP_

using namespace OsirisI;
#include "RenderInfo.hpp"

namespace OsirisI {
	namespace Graphics {
		namespace Renderer {
			class RenderInfoFactory {
				private:
					static std::vector<OsirisI::Graphics::Shader::ShaderInfo*> shaderInfos; //0 - Shaders that will be rendered with passed parameters
                    static std::vector<OsirisI::Graphics::Actors::IActor*> actors; //1
					static std::vector<ID3D11RenderTargetView*> renderTargets; //2
                    static bool turnAlphaBlendingOff, turnZBufferOff, isText;
			
				public:
					static void AddShaderInfo(OsirisI::Graphics::Shader::ShaderInfo* info);
					static void AddActor(OsirisI::Graphics::Actors::IActor* actor);
					static void AddRenderTarget(ID3D11RenderTargetView* renderTarget);

                    static void TurnZBufferOff(bool var);
                    static void TurnAlphaBlendingOff(bool var);
                    static void IsText(bool var);
                    
					static RenderInfo* CreateRenderInformation();
					static void LoadRenderInformation(RenderInfo* info);
			};
		}
	}
}
#endif /* RENDERINFOFACTORY_HPP_ */
