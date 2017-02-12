#include "stdafx.h"

using namespace OsirisI::Graphics::Actors;
using namespace OsirisI::Graphics::Devices;
using namespace OsirisI::Manager;
using namespace OsirisI::Utilities;

IMesh* ActorCreator::CreateMesh(OVertex* vertices, unsigned long* indices, unsigned int vertexCount, unsigned int indexCount, bool dynamic) {
    if (IGraphicsDevice::g_graphicDevice->GetDeviceType() == GraphicsDeviceType::DirectX11) {
        DX11Mesh* mesh = new DX11Mesh(HashedString(ObjectNameManager::CreateObjectName()));
        if (mesh->CreateVertexAndIndexBuffer(vertices, indices, vertexCount, indexCount, dynamic) == OS_OK) {
            return mesh;
        }
        return nullptr;
    }
    else if (IGraphicsDevice::g_graphicDevice->GetDeviceType() == GraphicsDeviceType::OpenGL) {
        return nullptr;
    }
    else {
        LOG_ERROR_SMALL("ActorCreator::CreateMesh catastrophic error: Unknown DeviceType");
        return nullptr;
    }
}
//
//ISprite* ActorCreator::CreatePlane(OVector2 size, bool dynamic, OVector2 leftUpUV, OVector2 rightBottomUV) {
//	unsigned long* indices = new unsigned long[6];
//    OVertex* vertices = new OVertex[6];
//    memset(vertices, 0, (sizeof(OVertex)* 6));
//
//    float fLeftUpU = 0.0f, fLeftUpV = 0.0f, fRightBottomU = 0.0f, fRightBottomV = 0.0f;
//    if (leftUpUV.x == leftUpUV.y == rightBottomUV.x == rightBottomUV.y) {
//        fRightBottomU = 1.0f;
//        fRightBottomV = 1.0f;
//    }
//    else {
//        fLeftUpU = leftUpUV.x;
//        fLeftUpV = leftUpUV.y;
//        fRightBottomU = rightBottomUV.x;
//        fRightBottomV = rightBottomUV.y;
//    }
//
//    float left = (float)((IGraphicsDevice::g_graphicDevice->GetWidth() / 2) * -1);
//    float right = left + size.x;
//    float top = (float)(IGraphicsDevice::g_graphicDevice->GetHeight() / 2);
//    float bottom = top - size.y;
//    
//    // Triangle-1
//    vertices[0].SetPosition(OVector3(left, top, 0.0f));  // Top left.
//    vertices[0].SetTexture(OVector2(fLeftUpU, fLeftUpV));
//    vertices[0].SetNormal(OVector3(0.f, 0.f, -1.0f));
//
//    vertices[1].SetPosition(OVector3(right, bottom, 0.0f));  // Bottom right.
//    vertices[1].SetTexture(OVector2(fRightBottomU, fRightBottomV));
//    vertices[1].SetNormal(OVector3(0.f, 0.f, -1.0f));
//
//    vertices[2].SetPosition(OVector3(left, bottom, 0.0f));  // Bottom left.
//    vertices[2].SetTexture(OVector2(fLeftUpU, fRightBottomV));
//    vertices[2].SetNormal(OVector3(0.f, 0.f, -1.0f));
//
//    // Triangle-2
//    vertices[3].SetPosition(OVector3(left, top, 0.0f));  // Top left.
//    vertices[3].SetTexture(OVector2(fLeftUpU, fLeftUpV));
//    vertices[3].SetNormal(OVector3(0.f, 0.f, -1.0f));
//
//    vertices[4].SetPosition(OVector3(right, top, 0.0f));  // Top right.
//    vertices[4].SetTexture(OVector2(fRightBottomU, fLeftUpV));
//    vertices[4].SetNormal(OVector3(0.f, 0.f, -1.0f));
//
//    vertices[5].SetPosition(OVector3(right, bottom, 0.0f));  // Bottom right.
//    vertices[5].SetTexture(OVector2(fRightBottomU, fRightBottomV));
//    vertices[5].SetNormal(OVector3(0.f, 0.f, -1.0f));
//
//
//    for (unsigned int i = 0; i < 6; i++) indices[i] = i;
//    IMesh* m = CreateMesh(vertices, indices, 6, 6, dynamic);
//
//    OS_SAFEDELETE_ARRAY(vertices);
//    OS_SAFEDELETE_ARRAY(indices);
//
//    return m;
//}

IMesh* ActorCreator::CreateTriangle(unsigned int sideLength, bool dynamic) {
	unsigned long* indices = new unsigned long[3];
    OVertex* vertices = new OVertex[3];
    memset(vertices, 0, (sizeof(OVertex)* 3));

    // Triangle-1
    vertices[0].SetPosition(OVector3(0.0f, 0.0f, 0.0f));  // Top Center.
    vertices[0].SetTexture(OVector2(0.0f, 0.0f));
    vertices[0].SetNormal(OVector3(0.f, 0.f, -1.0f));

	vertices[1].SetPosition(OVector3(static_cast<float>((sideLength / 2)), static_cast<float>((-1)*sideLength), 0.0f));  // Bottom right.
    vertices[1].SetTexture(OVector2(1.0f, 1.0f));
    vertices[1].SetNormal(OVector3(0.f, 0.f, -1.0f));

	vertices[2].SetPosition(OVector3(static_cast<float>((-1)*(sideLength / 2)), static_cast<float>((-1)*sideLength), 0.0f));  // Bottom left.
    vertices[2].SetTexture(OVector2(0.0f, 1.0f));
    vertices[2].SetNormal(OVector3(0.f, 0.f, -1.0f));
    
    for (unsigned int i = 0; i < 3; i++) indices[i] = i;
    IMesh* m = CreateMesh(vertices, indices, 3, 3, dynamic);

    OS_SAFEDELETE_ARRAY(vertices);
    OS_SAFEDELETE_ARRAY(indices);

    return m;
}

IMesh* ActorCreator::CreateCube(unsigned int sideLength, bool dynamic) {
	unsigned long* indices = new unsigned long[36];
    OVertex* vertices = new OVertex[36];
    memset(vertices, 0, (sizeof(OVertex)* 36));
    vertices[0].SetPosition(OVector3(1.0f, -1.0f, -1.0f));
    vertices[0].SetTexture(OVector2(0.0f, 0.0f));
    vertices[0].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[1].SetPosition(OVector3(1.0f, -1.0f, 1.0f));
    vertices[1].SetTexture(OVector2(1.0f, 0.0f));
    vertices[1].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[2].SetPosition(OVector3(-1.0f, -1.0f, 1.0f));
    vertices[2].SetTexture(OVector2(0.0f, 1.0f));
    vertices[2].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[3].SetPosition(OVector3(1.0f, -1.0f, -1.0f));
    vertices[3].SetTexture(OVector2(0.0f, 0.0f));
    vertices[3].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[4].SetPosition(OVector3(-1.0f, -1.0f, 1.0f));
    vertices[4].SetTexture(OVector2(1.0f, 0.0f));
    vertices[4].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[5].SetPosition(OVector3(-1.0f, -1.0f, -1.0f));
    vertices[5].SetTexture(OVector2(0.0f, 1.0f));
    vertices[5].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[6].SetPosition(OVector3(1.0f, 1.0f, -1.0f));
    vertices[6].SetTexture(OVector2(0.0f, 0.0f));
    vertices[6].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[7].SetPosition(OVector3(-1.0f, 1.0f, -1.0f));
    vertices[7].SetTexture(OVector2(1.0f, 0.0f));
    vertices[7].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[8].SetPosition(OVector3(-1.0f, 1.0f, 1.0f));
    vertices[8].SetTexture(OVector2(0.0f, 1.0f));
    vertices[8].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[9].SetPosition(OVector3(1.0f, 1.0f, -1.0f));
    vertices[9].SetTexture(OVector2(0.0f, 0.0f));
    vertices[9].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[10].SetPosition(OVector3(-1.0f, 1.0f, 1.0f));
    vertices[10].SetTexture(OVector2(1.0f, 0.0f));
    vertices[10].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[11].SetPosition(OVector3(1.0f, 1.0f, 1.0f));
    vertices[11].SetTexture(OVector2(0.0f, 1.0f));
    vertices[11].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[12].SetPosition(OVector3(1.0f, -1.0f, -1.0f));
    vertices[12].SetTexture(OVector2(0.0f, 0.0f));
    vertices[12].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[13].SetPosition(OVector3(1.0f, 1.0f, -1.0f));
    vertices[13].SetTexture(OVector2(1.0f, 0.0f));
    vertices[13].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[14].SetPosition(OVector3(1.0f, 1.0f, 1.0f));
    vertices[14].SetTexture(OVector2(0.0f, 1.0f));
    vertices[14].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[15].SetPosition(OVector3(1.0f, -1.0f, -1.0f));
    vertices[15].SetTexture(OVector2(0.0f, 0.0f));
    vertices[15].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[16].SetPosition(OVector3(1.0f, 1.0f, 1.0f));
    vertices[16].SetTexture(OVector2(1.0f, 0.0f));
    vertices[16].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[17].SetPosition(OVector3(1.0f, -1.0f, 1.0f));
    vertices[17].SetTexture(OVector2(0.0f, 1.0f));
    vertices[17].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[18].SetPosition(OVector3(1.0f, -1.0f, 1.0f));
    vertices[18].SetTexture(OVector2(0.0f, 0.0f));
    vertices[18].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[19].SetPosition(OVector3(1.0f, 1.0f, 1.0f));
    vertices[19].SetTexture(OVector2(1.0f, 0.0f));
    vertices[19].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[20].SetPosition(OVector3(-1.0f, 1.0f, 1.0f));
    vertices[20].SetTexture(OVector2(0.0f, 1.0f));
    vertices[20].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[21].SetPosition(OVector3(1.0f, -1.0f, 1.0f));
    vertices[21].SetTexture(OVector2(0.0f, 0.0f));
    vertices[21].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[22].SetPosition(OVector3(-1.0f, 1.0f, 1.0f));
    vertices[22].SetTexture(OVector2(1.0f, 0.0f));
    vertices[22].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[23].SetPosition(OVector3(-1.0f, -1.0f, 1.0f));
    vertices[23].SetTexture(OVector2(0.0f, 1.0f));
    vertices[23].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[24].SetPosition(OVector3(-1.0f, -1.0f, 1.0f));
    vertices[24].SetTexture(OVector2(0.0f, 0.0f));
    vertices[24].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[25].SetPosition(OVector3(-1.0f, 1.0f, 1.0f));
    vertices[25].SetTexture(OVector2(1.0f, 0.0f));
    vertices[25].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[26].SetPosition(OVector3(-1.0f, 1.0f, -1.0f));
    vertices[26].SetTexture(OVector2(0.0f, 1.0f));
    vertices[26].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[27].SetPosition(OVector3(-1.0f, -1.0f, 1.0f));
    vertices[27].SetTexture(OVector2(0.0f, 0.0f));
    vertices[27].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[28].SetPosition(OVector3(-1.0f, 1.0f, -1.0f));
    vertices[28].SetTexture(OVector2(1.0f, 0.0f));
    vertices[28].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[29].SetPosition(OVector3(-1.0f, -1.0f, -1.0f));
    vertices[29].SetTexture(OVector2(0.0f, 1.0f));
    vertices[29].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[30].SetPosition(OVector3(1.0f, 1.0f, -1.0f));
    vertices[30].SetTexture(OVector2(0.0f, 0.0f));
    vertices[30].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[31].SetPosition(OVector3(1.0f, -1.0f, -1.0f));
    vertices[31].SetTexture(OVector2(1.0f, 0.0f));
    vertices[31].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[32].SetPosition(OVector3(-1.0f, -1.0f, -1.0f));
    vertices[32].SetTexture(OVector2(0.0f, 1.0f));
    vertices[32].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[33].SetPosition(OVector3(1.0f, 1.0f, -1.0f));
    vertices[33].SetTexture(OVector2(0.0f, 0.0f));
    vertices[33].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[34].SetPosition(OVector3(-1.0f, -1.0f, -1.0f));
    vertices[34].SetTexture(OVector2(1.0f, 0.0f));
    vertices[34].SetNormal(OVector3(0.0f, 0.0f, -1.0f));
    vertices[35].SetPosition(OVector3(-1.0f, 1.0f, -1.0f));
    vertices[35].SetTexture(OVector2(0.0f, 1.0f));
    vertices[35].SetNormal(OVector3(0.0f, 0.0f, -1.0f));

    for (unsigned int i = 0; i < 36; i++) indices[i] = i;
    IMesh* m = CreateMesh(vertices, indices, 36, 36, dynamic);

    OS_SAFEDELETE_ARRAY(vertices);
    OS_SAFEDELETE_ARRAY(indices);

    return m;
}

IMesh* ActorCreator::CreateSphere(unsigned int radius, bool dynamic) {
    return nullptr;
}
