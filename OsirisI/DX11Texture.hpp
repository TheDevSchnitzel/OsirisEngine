#ifndef DX11TEXTURE_HPP_
#define DX11TEXTURE_HPP_

struct ID3D11ShaderResourceView;

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
		class DX11Texture : public ITexture {
			private:
				ID3D11ShaderResourceView* texture;

			public:
				DX11Texture(ID3D11ShaderResourceView* texture, OsirisI::Utilities::HashedString name, OsirisI::Utilities::HashedString containerPath);
				~DX11Texture();
				
				ID3D11ShaderResourceView* GetRawTexture();

				ReturnState Recover();
				ReturnState Update(float delta);
				ReturnState Release();
		};
	}}
}

#endif /* DX11TEXTURE_HPP_ */