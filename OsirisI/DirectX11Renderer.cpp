#include "stdafx.h"

using namespace OsirisI::Manager;
using namespace OsirisI::Utilities;
using namespace OsirisI::Graphics::Scenes;
using namespace OsirisI::Graphics::Shader;
using namespace OsirisI::Graphics::Actors;
using namespace OsirisI::Graphics::Devices;

namespace OsirisI {
    namespace Graphics {
        namespace Renderer {

            void DirectX11Renderer::SetDirect3DDevice(Direct3DDevice* direct3DDevice) {
                this->direct3DDevice = direct3DDevice;
            }
            Direct3DDevice* DirectX11Renderer::GetDirect3DDevice() {
                return this->direct3DDevice;
            }

            ReturnState DirectX11Renderer::BeginRendering() {
                ID3D11Device* dev = this->direct3DDevice->GetDevice();
                ID3D11DeviceContext* devCon = this->direct3DDevice->GetDeviceContext();

                // Clear the back buffer.
				devCon->ClearRenderTargetView(this->direct3DDevice->GetRenderTargetView(), new float[4] { 0.0f, 0.2f, 0.4f, 1.0f });

                // Clear the depth buffer.
                devCon->ClearDepthStencilView(this->direct3DDevice->GetDepthBufferView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
                return OS_OK;
            }


            ReturnState DirectX11Renderer::Render(RenderInfo* info) {
                do {
                    ID3D11Device* dev = this->direct3DDevice->GetDevice();
                    ID3D11DeviceContext* devCon = this->direct3DDevice->GetDeviceContext();

                    if (info->turnAlphaBlendingOff) this->direct3DDevice->TurnAlphaBlendingOff();
                    else this->direct3DDevice->TurnAlphaBlendingOn();

                    if (info->turnZBufferOff) this->direct3DDevice->TurnZBufferOff();
                    else this->direct3DDevice->TurnZBufferOn();

                    std::unordered_map<long, std::vector<ShaderInfo*>> shadersToRender;
                    for (unsigned int i = 0; i < info->shaderInfoCount; i++) {
                        long shaderHash = info->shaderInfos[i]->shaderNameHash;
                        if (shaderHash == 0) continue;

                        if (shadersToRender.find(shaderHash) == shadersToRender.end()) {
                            std::vector<ShaderInfo*> tmp;
                            tmp.push_back(info->shaderInfos[i]);
                            shadersToRender.insert(std::pair<long, std::vector<ShaderInfo*>>(shaderHash, tmp));
                        }
                        else {
                            shadersToRender[shaderHash].push_back(info->shaderInfos[i]);
                        }
                    }


                    UINT stride = sizeof(OVertex);
                    UINT offset = 0;


                    std::vector<IActor*> actorList = info->actors;
                    unsigned int modelCount = actorList.size();

                    for (unsigned int i = 0; i < modelCount; i++) {
                        IActor* actor = actorList[i];
                        unsigned int meshCount = actor->GetGameModel()->GetMeshes().size();
                        std::vector<IMesh*>meshes = actor->GetGameModel()->GetMeshes();

                        OMatrix4x4 projectionMatrix, worldMatrix, viewMatrix, orthoMatrix;
                        IGraphicsDevice::GetProjectionMatrix(projectionMatrix);
                        IGraphicsDevice::GetWorldMatrix(worldMatrix);
                        IGraphicsDevice::GetOrthoMatrix(orthoMatrix);
                        ((CameraActor*)((ActorNode*)this->cameraNode)->GetActor())->GetViewMatrix(viewMatrix);
                        OVector3 parentPos = actor->GetPosition(), parentRot = actor->GetRotation();

                        for (unsigned int j = 0; j < meshCount; j++) {
                            DX11Mesh* mesh = (DX11Mesh*)meshes[i];

							OMatrix4x4 tmpWorldMatrix = DX3DMatrixPositionTransformAndRotationYawPitchRoll(worldMatrix, (parentPos + mesh->GetPosition()), (parentRot + mesh->GetRotation()));

                            ID3D11Buffer* tmpVertexBuffer = mesh->GetVertexBuffer();
                            devCon->IASetVertexBuffers(0, 1, &tmpVertexBuffer, &stride, &offset);
                            devCon->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
                            devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                            std::unordered_map<long, std::vector<ShaderInfo*>>::iterator it = shadersToRender.begin();
                            for (; it != shadersToRender.end(); ++it) { //For all shaders included in the renderinfo
                                DX11Shader* shader = static_cast<DX11Shader*>(ShaderManager::GetShader(it->first));
                                if (shader == nullptr) {
                                    LOG_WARNING("Shader not set, nullptr in rendering pipeline! [Hash: " << it->second[0]->shaderNameHash << "]");
                                    continue;
                                }
                                shader->SetShaderInfos(it->second);

                                bool noRotationOrTransformation = false;
                                for (unsigned int i = 0; i < it->second.size(); i++) if (it->second[i]->noWorldRotation) { noRotationOrTransformation = true; break; }

                                if (info->turnZBufferOff) {
                                    OS_CHECKSTATE(shader->SetParameterMatrices(orthoMatrix, ((noRotationOrTransformation) ? worldMatrix : tmpWorldMatrix), this->baseViewMatrix));
                                }
                                else {
                                    OS_CHECKSTATE(shader->SetParameterMatrices(projectionMatrix, ((noRotationOrTransformation) ? worldMatrix : tmpWorldMatrix), viewMatrix));
                                }
                                OS_CHECKSTATE(shader->SetActive());

                                devCon->DrawIndexed(mesh->GetIndicesCount(), 0, 0);
                            }
                        }
                    }

                    if (info->turnAlphaBlendingOff) this->direct3DDevice->TurnAlphaBlendingOn();
                    else this->direct3DDevice->TurnAlphaBlendingOff();

                    if (info->turnZBufferOff)this->direct3DDevice->TurnZBufferOn();
                    else this->direct3DDevice->TurnZBufferOff();

                    info = info->nestedRenderInfo;
                } while (info != nullptr);
                return OS_OK;
            }

            ReturnState DirectX11Renderer::EndRendering() {
                HRESULT result = S_OK;
                if (this->direct3DDevice->IsVsync()) result = this->direct3DDevice->GetSwapChain()->Present(1, 0);		// Show on screen refresh	
                else result = this->direct3DDevice->GetSwapChain()->Present(0, 0);

				if (result == S_OK) { 
					return OS_OK; 
				}
                else {
                    LOG_ERROR_SMALL("[0x21]Could not end rendering: " << std::hex << result << "\n");
                    if (result == 0x887a0005) {
                        LOG_ERROR_SMALL("GetDeviceRemovedReason: " << std::hex << this->direct3DDevice->GetDevice()->GetDeviceRemovedReason());
                    }
                    return OS_DIRECTXERROR;
                }
            }

            ReturnState DirectX11Renderer::TransformCamera(OVector3* rotation, OVector3* lookAt, OVector3* up, OMatrix4x4* viewMatrix, OVector3* position) {
				XMMATRIX rotationMatrixDX = XMMatrixRotationRollPitchYaw(rotation->z, rotation->x, rotation->y);
				
				// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
				XMVECTOR lookAtDX = XMLoadFloat3(&XMFLOAT3(lookAt->x, lookAt->y, lookAt->z));
				XMVECTOR upDX = XMLoadFloat3(&XMFLOAT3(up->x, up->y, up->z));				
				lookAtDX = XMVector3TransformCoord(lookAtDX, rotationMatrixDX);
				upDX = XMVector3TransformCoord(upDX, rotationMatrixDX);

				// Translate the rotated camera position to the location of the viewer.
				XMVECTOR positionDX = XMLoadFloat3(&XMFLOAT3(position->x, position->y, position->z));
				lookAtDX = XMVectorAdd(positionDX, lookAtDX);

				// Finally create the view matrix from the three updated vectors.
				XMMATRIX viewMatrixDX = XMMatrixLookAtLH(positionDX, lookAtDX, upDX);
				
#pragma region Update Values
				XMFLOAT3 lookAtDXV;
				XMStoreFloat3(&lookAtDXV, lookAtDX);

				lookAt->x = lookAtDXV.x;
				lookAt->y = lookAtDXV.y;
				lookAt->z = lookAtDXV.z;

				XMFLOAT3 upDXV;
				XMStoreFloat3(&upDXV, upDX);
                up->x = upDXV.x;
                up->y = upDXV.y;
                up->z = upDXV.z;

				XMFLOAT3 positionDXV;
				XMStoreFloat3(&positionDXV, positionDX);
                position->x = positionDXV.x;
                position->y = positionDXV.y;
                position->z = positionDXV.z;
				
				XMFLOAT4X4 viewMatrixDXV;
				XMStoreFloat4x4(&viewMatrixDXV, viewMatrixDX);
                viewMatrix->f[0] = viewMatrixDXV._11;
                viewMatrix->f[1] = viewMatrixDXV._12;
                viewMatrix->f[2] = viewMatrixDXV._13;
                viewMatrix->f[3] = viewMatrixDXV._14;
                viewMatrix->f[4] = viewMatrixDXV._21;
                viewMatrix->f[5] = viewMatrixDXV._22;
                viewMatrix->f[6] = viewMatrixDXV._23;
                viewMatrix->f[7] = viewMatrixDXV._24;
                viewMatrix->f[8] = viewMatrixDXV._31;
                viewMatrix->f[9] = viewMatrixDXV._32;
                viewMatrix->f[10] = viewMatrixDXV._33;
                viewMatrix->f[11] = viewMatrixDXV._34;
                viewMatrix->f[12] = viewMatrixDXV._41;
                viewMatrix->f[13] = viewMatrixDXV._42;
                viewMatrix->f[14] = viewMatrixDXV._43;
                viewMatrix->f[15] = viewMatrixDXV._44;
#pragma endregion

                return OS_OK;
            }

            ReturnState DirectX11Renderer::RenderCamera() {
                OVector3 up, position, lookAt;
                float yaw, pitch, roll;
                OMatrix4x4 camViewMatrix;
                OVector3 camPosition = ((ActorNode*)this->cameraNode)->GetActor()->GetPosition();
				OVector3 camRotation = ((ActorNode*)this->cameraNode)->GetActor()->GetRotation();
				((CameraActor*)((ActorNode*)this->cameraNode)->GetActor())->GetViewMatrix(camViewMatrix);

                // Setup the vector that points upwards.
                up.x = 0.0f;
                up.y = 1.0f;
                up.z = 0.0f;

                // Setup the position of the camera in the world.
                position.x = camPosition.x;
                position.y = camPosition.y;
                position.z = camPosition.z;

                // Setup where the camera is looking by default.
                lookAt.x = 0.0f;
                lookAt.y = 0.0f;
                lookAt.z = 1.0f;

                // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
                pitch = Converter::GradToRad(camRotation.x);
                yaw = Converter::GradToRad(camRotation.y);
                roll = Converter::GradToRad(camRotation.z);
                OVector3 rotation = OVector3(pitch, yaw, roll);

                OS_CHECKSTATE(TransformCamera(&rotation, &lookAt, &up, &camViewMatrix, &position));

                ((CameraActor*)((ActorNode*)this->cameraNode)->GetActor())->SetViewMatrix(camViewMatrix);

                return OS_OK;
            }
			
            OMatrix4x4 DirectX11Renderer::DX3DMatrixPositionTransformAndRotationYawPitchRoll(OMatrix4x4 worldMatrix, OVector3 position, OVector3 rotationSpeed) {
				XMMATRIX worldMatrixDX = XMMATRIX(worldMatrix.f);
				XMMATRIX tmpRotMatrix = XMMatrixRotationRollPitchYaw(rotationSpeed.z, rotationSpeed.x, rotationSpeed.y);
				XMMATRIX b = XMMatrixTranslation(position.x, position.y, position.z);
				b = (worldMatrixDX * tmpRotMatrix) * b;

				XMFLOAT4X4 nwdmDX;
				XMStoreFloat4x4(&nwdmDX, b);

				OMatrix4x4 ret = OMatrix4x4(nwdmDX.m);

				return ret;
            }
        }
    }
}
