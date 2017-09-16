#ifndef OSIRISI_ENGINE_HPP_
#define OSIRISI_ENGINE_HPP_

#include "STD_IncludeHeader.hpp"
#include "DirectXUsings.hpp"
#include "OsirisI_ExternComponents.hpp"


#include "HelperFunctions.hpp"
#include "Converter.hpp"
#include "FileIOHelper.hpp"

#include "OsirisIInternStructs.hpp"
#include "Logger.hpp"
#include "EngineConfiguration.hpp"
#include "OsirisIExternStructs.hpp"
#include "Persistor.hpp"
#include "HashedString.hpp"

#include "IOsirisObject.hpp"
#include "IActor.hpp"
#include "MovingActor.hpp"
#include "CameraActor.hpp"

#include "ShaderInfo.hpp"
#include "ShaderInfoFactory.hpp"
#include "RenderInfo.hpp"
#include "RenderInfoFactory.hpp"

#include "IMesh.hpp"
#include "ITexture.hpp"
#include "ISprite.hpp"
#include "IText.hpp"
#include "IGameModel.hpp"

#include "IComponent.hpp"
#include "IProgressBar.hpp"

#include "ISceneNode.hpp"
#include "IScene.hpp"

#include "IGraphicsDevice.hpp"
#include "IRenderer.hpp"

#include "IWindow.hpp"
#include "OsirisIHWND.hpp"

#include "IShader.hpp"

#include "Resource.hpp"
#include "IResourceFile.hpp"
#include "ResourceCache.hpp"
#include "ZIPResourceFile.hpp"

#include "MemoryPool.hpp"
#include "IEvent.hpp"
#include "IEventListener.hpp"

#pragma region Manager
#include "MemoryManager.hpp"
#include "EventManager.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"
#include "LoaderManager.hpp"
#include "ObjectNameManager.hpp"
#include "ResourceManager.hpp"
#include "SceneManager.hpp"
#include "ScriptManager.hpp"
#include "ShaderManager.hpp"
#include "TaskManager.hpp"
#pragma endregion


#include "IOsirisScript.hpp"
#include "LuaScript.hpp"
#include "PythonScript.hpp"
#include "JsScript.hpp"

#pragma region Events
#include "OsirisI_SystemEvent.hpp"
#include "OsirisI_InputEvents.hpp"
#include "OsirisI_NetworkEvents.hpp"
#pragma endregion

#pragma region Scenes/Nodes
#include "ActorNode.hpp"
#include "RootNode.hpp"
#include "SkyNode.hpp"
#include "ComponentNode.hpp"

#include "DebugScene.hpp"
#pragma endregion

#pragma region DX-Specific
    #include "DX11Shader.hpp"
    #include "Direct3DDevice.hpp"
#pragma region Actors
#include "DX11GameModel.hpp"
#include "DX11Mesh.hpp"
#include "DX11Sprite.hpp"
#include "DX11Text.hpp"
#include "DX11Texture.hpp"
#pragma endregion
    #include "DirectX11Renderer.hpp"

#pragma endregion

#include "DX11ProgressBar.hpp"
#include "ActorCreator.hpp"

#include "OsirisI_Client.hpp"
#include "OsirisI_Server.hpp"

#include "OsirisIDevice.hpp"

#endif /* OSIRISI_ENGINE_HPP_ */
