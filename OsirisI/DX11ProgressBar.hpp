#ifndef DX11ProgressBar_HPP_
#define DX11ProgressBar_HPP_

namespace OsirisI {
    namespace Graphics {
        namespace Components {
            class DX11ProgressBar : public OsirisI::Graphics::Components::IProgressBar {
                private:
                protected:

                public:
                    /**
                    * \brief This constructor uses default colors to display the OS_PrograssBar (Dark-/Lightblue)
                    * \param size Defines the width and height of the component
                    * \param pos Defines the position of the component
                    * \param name Defines the name of the component
                    */
                    DX11ProgressBar(OsirisI::OVector2 size, OsirisI::OVector3 pos, OsirisI::Utilities::HashedString name);

                    /**
                    * \brief This constructor uses the given texture (containerPath) to display the OS_PrograssBar
                    * \param size Defines the width and height of the component
                    * \param pos Defines the position of the component
                    * \param name Defines the name of the component
                    * \param containerPath Defines the path of the texture used for rendering -> Sprite-Mode is repeat
                    */
                    DX11ProgressBar(OsirisI::OVector2 size, OsirisI::OVector3 pos, OsirisI::Utilities::HashedString name, OsirisI::Utilities::HashedString containerPath);

                    
					ReturnState Initialize();
					ReturnState Update(float delta);
					ReturnState Recover();
					ReturnState Release();
					OsirisI::Graphics::Renderer::RenderInfo* GetRenderInfo();
            };
        }
    }
}

#endif /* DX11ProgressBar_HPP_ */
