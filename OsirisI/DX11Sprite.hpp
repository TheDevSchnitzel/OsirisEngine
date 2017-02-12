#ifndef DX11SPRITE_HPP_
#define DX11SPRITE_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
		class DX11Sprite : public ISprite {
			protected:

			public:
				DX11Sprite(OVector3 pos, OVector2 size, OsirisI::Utilities::HashedString name, OsirisI::Utilities::HashedString containerPath);
				DX11Sprite(OVector3 pos, OVector2 size, OsirisI::Utilities::HashedString name, OVector4 color);
				~DX11Sprite();

				ReturnState Update(float time);
				ReturnState Release();
				ReturnState Recover();
				ReturnState Initialize();
				OsirisI::Graphics::Renderer::RenderInfo* GetRenderInfo();
            
            private:            
                void UpdateVerticesPositions();
		};
	}}
}


#endif /* DX11SPRITE_HPP_ */
