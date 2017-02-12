#ifndef ACTORCREATOR_HPP_
#define ACTORCREATOR_HPP_

namespace OsirisI {
    namespace Graphics {
        namespace Actors {
            class ActorCreator {
                public:
                    static IMesh* CreateMesh(OVertex* vertices, unsigned long* indices, unsigned int vertexCount, unsigned int indexCount, bool dynamic = false);
                    //static ISprite* CreatePlane(OVector2 size, bool dynamic = false, OVector2 leftUpUV = OVector2(0.0f, 0.0f), OVector2 rightBottomUV = OVector2(0.0f, 0.0f));
                    static IMesh* CreateTriangle(unsigned int sideLength, bool dynamic = false);
                    static IMesh* CreateCube(unsigned int sideLength, bool dynamic = false);
                    static IMesh* CreateSphere(unsigned int radius, bool dynamic = false);
            };
        }
    }
}
#endif /* ACTORCREATOR_HPP_ */
