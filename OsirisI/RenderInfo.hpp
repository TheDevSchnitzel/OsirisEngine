#ifndef RENDERINFO_HPP_
#define RENDERINFO_HPP_

struct ID3D11RenderTargetView;
namespace OsirisI {
	namespace Graphics {
		namespace Renderer {	
			struct RenderInfo { //Generated struct which is used for the renderer
				public:
                    RenderInfo() {
                        nestedRenderInfo = nullptr;
					    shaderInfos = nullptr;
					    renderTargets = nullptr;
                        shaderInfoCount = 0, renderTargetCount = 0;
                        turnAlphaBlendingOff = false, turnZBufferOff = false, isText = false;
                    }
                    RenderInfo( const RenderInfo& other ) : 
                        actors(other.actors),
                        shaderInfoCount( other.shaderInfoCount ),
                        renderTargetCount( other.renderTargetCount ), 
                        turnAlphaBlendingOff(other.turnAlphaBlendingOff), 
                        turnZBufferOff(other.turnZBufferOff),
                        isText(other.isText) {

					    shaderInfos = nullptr;
					    renderTargets = nullptr;
                        nestedRenderInfo = other.nestedRenderInfo;

                        if(shaderInfoCount > 0) shaderInfos = new OsirisI::Graphics::Shader::ShaderInfo*[shaderInfoCount];
                        if(renderTargetCount > 0) renderTargets = new ID3D11RenderTargetView*[renderTargetCount];
                        
                        for(unsigned int i = 0; i < shaderInfoCount; i++) shaderInfos[i] = other.shaderInfos[i];
                        for(unsigned int i = 0; i < renderTargetCount; i++) renderTargets[i] = other.renderTargets[i];
                    }

                    ~RenderInfo() {
                        nestedRenderInfo = nullptr;
                        if(shaderInfos != nullptr) {
                            delete[] shaderInfos;
                            shaderInfos = nullptr;
                        }
                        if(renderTargets != nullptr) {
                            delete[] renderTargets;
                            renderTargets = nullptr;
                        }
                    }

					OsirisI::Graphics::Shader::ShaderInfo** shaderInfos;
					std::vector<OsirisI::Graphics::Actors::IActor*> actors; //Render multiple models with the same settings
					ID3D11RenderTargetView** renderTargets;

                    unsigned int shaderInfoCount, renderTargetCount;

                    bool turnAlphaBlendingOff, turnZBufferOff, isText;

                    OsirisI::Graphics::Renderer::RenderInfo* nestedRenderInfo;
			};
		}
	}
}

#endif /* RENDERINFO_HPP_ */
