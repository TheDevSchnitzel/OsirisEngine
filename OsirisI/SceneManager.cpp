#include "stdafx.h"

using namespace OsirisI::Graphics::Actors;
using namespace OsirisI::Graphics::Scenes;
using namespace OsirisI::Graphics::Renderer;
using namespace OsirisI::Utilities;

namespace OsirisI {
    namespace Manager {
        std::map<long, Graphics::Scenes::IScene*> SceneManager::scenes;
        Graphics::Scenes::IScene* SceneManager::activeScene;
        IRenderer *SceneManager::renderer;
        std::mutex SceneManager::lock;

        ReturnState SceneManager::Initialize(IRenderer* renderer) {
            SceneManager::renderer = renderer;
            return OS_OK;
        }

        ReturnState SceneManager::AddScene(Graphics::Scenes::IScene* scene) {
            {// Wait till you get a lock - then cover it with a lock_guard so it's released when this block goes out of scope
                lock.lock();
                std::lock_guard<std::mutex> lockG(lock, std::adopt_lock); 
                if (scenes.find(scene->GetNameHash()) != scenes.end()) return OS_OBJECT_ALREADY_EXISTS;
                scenes.insert(std::pair<long, Graphics::Scenes::IScene*>(scene->GetNameHash(), scene));
                scene->GetRootNode()->SetRenderer(renderer);
                ((CameraActor*)scene->GetCameraNode()->GetActor())->SetRenderer(renderer);
            
                if (activeScene != nullptr) return OS_OK;
            }

            ActivateScene(scene);

            return OS_OK;
        }

        ReturnState SceneManager::RemoveScene(Graphics::Scenes::IScene* scene) {
            {// Wait till you get a lock - then cover it with a lock_guard so it's released when this block goes out of scope
                lock.lock();
                std::lock_guard<std::mutex> lockG(lock, std::adopt_lock); 
                std::map<long, Graphics::Scenes::IScene*>::iterator it = scenes.find(scene->GetNameHash());
                if (it == scenes.end()) return OS_OBJECT_NOT_FOUND;
                if (activeScene == scene) return OS_OBJECT_STILL_IN_USE;
                scenes.erase(it);
            }

            return OS_OK;
        }

        ReturnState SceneManager::RemoveScene(long sceneID) {
            {// Wait till you get a lock - then cover it with a lock_guard so it's released when this block goes out of scope
                lock.lock();
                std::lock_guard<std::mutex> lockG(lock, std::adopt_lock); 
                std::map<long, Graphics::Scenes::IScene*>::iterator it = scenes.find(sceneID);
                if (it == scenes.end()) return OS_OBJECT_NOT_FOUND;
                if (activeScene->GetNameHash() == sceneID) return OS_OBJECT_STILL_IN_USE;
                scenes.erase(it);
            }

            return OS_OK;
        }

        ReturnState SceneManager::ActivateScene(Graphics::Scenes::IScene* scene) {
            {// Wait till you get a lock - then cover it with a lock_guard so it's released when this block goes out of scope
                lock.lock();
                std::lock_guard<std::mutex> lockG(lock, std::adopt_lock); 
                if (scenes.find(scene->GetNameHash()) == scenes.end()) return OS_OBJECT_NOT_FOUND;
                if (scene->PrepareForActivation() == OS_OK) {
                    activeScene = scene;
                    renderer->SetRootNode(activeScene->GetRootNode());
                    renderer->SetCameraNode(activeScene->GetCameraNode());
                }
                else {
                    return OS_SCENE_ACTIVATION_FAILED;
                }
            }

            return OS_OK;
        }

        ReturnState SceneManager::ActivateScene(long sceneID) {
            {// Wait till you get a lock - then cover it with a lock_guard so it's released when this block goes out of scope
                lock.lock();
                std::lock_guard<std::mutex> lockG(lock, std::adopt_lock); 
                if (scenes.find(sceneID) == scenes.end()) return OS_OBJECT_NOT_FOUND;

                Graphics::Scenes::IScene* tmpScene = scenes[sceneID];
                if (tmpScene->PrepareForActivation() == OS_OK) activeScene = tmpScene;
                return OS_OK;
            }
        }

        ReturnState SceneManager::Update(float delta) {
            {// Wait till you get a lock - then cover it with a lock_guard so it's released when this block goes out of scope
                lock.lock();
                std::lock_guard<std::mutex> lockG(lock, std::adopt_lock); 

                if (activeScene == nullptr) {
                    return OS_OBJECT_NOT_FOUND;
                }
                
                if(activeScene->GetRootNode() != nullptr) {
					OS_CHECKSTATE(activeScene->GetRootNode()->Update(delta));
                }
				
				return activeScene->Update(delta);
            }
        }

        ReturnState SceneManager::Render() {
            {// Wait till you get a lock - then cover it with a lock_guard so it's released when this block goes out of scope
                lock.lock();
                std::lock_guard<std::mutex> lockG(lock, std::adopt_lock); 
                if (activeScene == nullptr) return OS_OBJECT_NOT_FOUND;

                OS_CHECKSTATE(renderer->BeginRendering());
                OS_CHECKSTATE(renderer->RenderCamera());

                OMatrix4x4 tmpViewMatrix;
				((CameraActor*)((ActorNode*)renderer->GetCameraNode())->GetActor())->GetViewMatrix(tmpViewMatrix);
                renderer->SetBaseViewMatrix(tmpViewMatrix);

                std::vector<RenderInfo*> renderInfos = activeScene->GetRootNode()->GetRenderInfos();
                for (unsigned int i = 0; i < renderInfos.size(); i++) {
                    RenderInfo* ri = renderInfos[i];
                    if (ri != nullptr) {
                        OS_CHECKSTATE(renderer->Render(ri));
                    }
                }

                OS_CHECKSTATE(renderer->EndRendering());
            }

            return OS_OK;
        }

        ReturnState SceneManager::Release() {
            ReturnState tmp = OS_OK;

            {// Wait till you get a lock - then cover it with a lock_guard so it's released when this block goes out of scope
                lock.lock();
                std::lock_guard<std::mutex> lockG(lock, std::adopt_lock); 

                std::map<long, Graphics::Scenes::IScene*>::iterator it = scenes.begin();
                for (; it != scenes.end(); ++it) {
                    if (it->second != nullptr) {
                        if(it->second->Release() != OS_OK) tmp = OS_ERROR_NOT_SPECIFIED;
                        it->second = nullptr;
                    }
                }

                scenes.clear();
                renderer = nullptr;
                activeScene = nullptr;
            }

            return tmp;
        }
    }
}
