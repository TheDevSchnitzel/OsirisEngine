#ifndef ISPRITE_HPP_
#define ISPRITE_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Actors {
		class ISprite : public IActor {
			protected:
				ISprite(OsirisI::Utilities::HashedString name) : IActor(name) {}

				unsigned int positionX, positionY, prevPositionX, prevPositionY, vertexCount, indexCount;
				float topLeftU, topLeftV, bottomRightU, bottomRightV;		
                OVector2 size, prevSize;
				ITexture* texture;
                
                bool isColorOnly;
                OVector4 color;

				/**
				If set to true will repeat the texture if width or height value is greater than the size of the texture.
				If set to false will stretch the texture to the defined size(width, height)
				*/
				bool repeat;

			public:
				virtual ~ISprite() { }

				unsigned int GetVertexCount() {
					return this->vertexCount;
				}
				void SetVertexCount(unsigned int vertexCount) {
					this->vertexCount = vertexCount;
				}

				unsigned int GetIndexCount() {
					return this->indexCount;
				}
				void SetIndexCount(unsigned int indexCount) {
					this->indexCount = indexCount;
				}

				unsigned int GetPositionX() {
					return this->positionX;
				}
				unsigned int GetPositionY() {
					return this->positionY;
				}
				void SetPositionX(unsigned int x) {
					this->positionX = x;
				}
				void SetPositionY(unsigned int y) {
					this->positionY = y;
				}

				unsigned int GetWidth() {
					return static_cast<unsigned int>(this->size.x);
				}

				unsigned int GetHeight() {
					return static_cast<unsigned int>(this->size.y);
				}

				void SetWidth(unsigned int width) {
					this->size.x = static_cast<float>(width);
				}

				void SetHeight(unsigned int height) {
					this->size.y = static_cast<float>(height);
				}	

				ITexture* GetTexture() {
					return this->texture;
				}


                
            protected:
				float GetTopLeftU() {
					return this->topLeftU;
				}
				float GetTopLeftV() {
					return this->topLeftV;
				}
				float GetBottomRightU() {
					return this->bottomRightU;
				}
				float GetBottomRightV() {
					return this->bottomRightV;
				}

				void SetTopLeftU(float topLeftU) {
					this->topLeftU = topLeftU;
				}
				void SetTopLeftV(float topLeftV) {
					this->topLeftV = topLeftV;
				}
				void SetBottomRightU(float bottomRightU) {
					this->bottomRightU = bottomRightU;
				}
				void SetBottomRightV(float bottomRightV) {
					this->bottomRightV = bottomRightV;
				}

            public:
				void SetRepeat(bool value) {
					this->repeat = value;
				}

				bool GetRepeat() {
					return this->repeat;
				}

				unsigned int GetHeightInPixel() {
					return static_cast<unsigned int>((this->texture->GetHeight()*this->bottomRightV) - (this->texture->GetHeight()*this->topLeftV));
				}

				unsigned int GetWidthInPixel() {
					return static_cast<unsigned int>((this->texture->GetWidth()*this->bottomRightU) - (this->texture->GetWidth()*this->topLeftU));
                }
		};
		}
	}
}

#endif /* ISPRITE_HPP_ */
