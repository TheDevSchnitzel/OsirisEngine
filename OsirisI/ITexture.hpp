#ifndef ITEXTURE_HPP_
#define ITEXTURE_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
			class ITexture : public IOsirisObject {	
				protected:
					ITexture(OsirisI::Utilities::HashedString name) : IOsirisObject(name) {}
					OsirisI::Utilities::HashedString containerPath;
					unsigned int width, height;
					//No Render-/ShaderInfo -> this class is a basic resource which is used in GameModels ans Sprites, so it will be defined in some of their RI's/SI's if needed

				public:
					virtual ~ITexture() { }

					OsirisI::Utilities::HashedString GetContainerPath() {
						return containerPath;
					}

					unsigned int GetWidth() {
						return this->width;
					}
					unsigned int GetHeight() {
						return this->height;
					}
					void SetWidth(unsigned int width) {
						this->width = width;
					}
					void SetHeight(unsigned int height) {
						this->height = height;
					}
					
					virtual ReturnState Recover() = 0;
					virtual ReturnState Update(float delta) = 0;
					virtual ReturnState Release() = 0;
			};
		}
	}
}

#endif /* ITEXTURE_HPP_ */
