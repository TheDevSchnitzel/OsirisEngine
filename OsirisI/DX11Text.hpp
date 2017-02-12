#ifndef DX11TEXT_HPP_
#define DX11TEXT_HPP_

namespace OsirisI {
    namespace Graphics {
        namespace Actors {
            class DX11Text : public IText {
                protected:

                public:
                    DX11Text(OsirisI::OVector3 pos, OsirisI::Utilities::HashedString name, OsirisI::Utilities::HashedString fontContainerPath, std::string text);
                    ~DX11Text();

                    OsirisI::ReturnState Initialize();
                    OsirisI::ReturnState Update(float delta);
                    OsirisI::ReturnState Recover();
                    OsirisI::ReturnState Release();

                    OsirisI::Graphics::Renderer::RenderInfo* GetRenderInfo();
                    
                private:
                    OsirisI::ReturnState UpdateVerticesPositions();
            };
        }
    }
}


#endif /* DX11TEXT_HPP_ */
