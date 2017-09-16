#include "stdafx.h"

#ifndef OSIRISI_BASICHEADER_HPP_
#define OSIRISI_BASICHEADER_HPP_

#include "Timer.hpp"
#include "HelperFunctions.hpp"
#include "Converter.hpp"
#include "FileIOHelper.hpp"

#include "OsirisIInternStructs.hpp"
#include "Logger.hpp"
#include "EngineConfiguration.hpp"
#include "OsirisIExternStructs.hpp"
#include "Persistor.hpp"
#include "HashedString.hpp"

#include "Resource.hpp"
#include "IResourceFile.hpp"
#include "ResourceCache.hpp"
#include "ZIPResourceFile.hpp"

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
#include "IGraphicsDevice.hpp"
#include "Direct3DDevice.hpp"

#include "ActorCreator.hpp"
#include "DX11GameModel.hpp"
#include "DX11Mesh.hpp"
#include "DX11Texture.hpp"
#include "DX11Sprite.hpp"
#include "DX11ProgressBar.hpp"
#include "DX11Text.hpp"

#include "IWindow.hpp"
#include "ISceneNode.hpp"
#include "IRenderer.hpp"
#include "IShader.hpp"
#include "DebugScene.hpp"

#include "DX11Shader.hpp"
#include "RootNode.hpp"
#include "SkyNode.hpp"
#include "ComponentNode.hpp"
#include "ActorNode.hpp"
#include "DirectX11Renderer.hpp"

#include "IEvent.hpp"
#include "IEventListener.hpp"
#include "OsirisIEngineEventListener.hpp"

#include "Font.hpp"
#include "ILoader.hpp"
#include "FontLoader.hpp"
#include "DXTextureLoader.hpp"
#include "WaveFrontObjectModelLoader.hpp"

#include "MemoryPool.hpp"
#include "ThreadPool.hpp"

#include "MemoryManager.hpp"
#include "EventManager.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"
#include "LoaderManager.hpp"
#include "ObjectNameManager.hpp"
#include "ResourceManager.hpp"
#include "SceneManager.hpp"
#include "ShaderManager.hpp"
#include "TaskManager.hpp"
#include "ScriptManager.hpp"

#include "IOsirisScript.hpp"
#include "LuaScript.hpp"
#include "PythonScript.hpp"
#include "JsScript.hpp"

#include "OsirisI_SystemEvent.hpp"
#include "OsirisI_InputEvents.hpp"
#include "OsirisI_NetworkEvents.hpp"
#include "OsirisI_ScriptEvents.hpp"

#include "OsirisI_Client.hpp"
#include "OsirisI_Server.hpp"

#include "OsirisIHWND.hpp"
#include "OsirisIDevice.hpp"

#endif /* OSIRISI_BASICHEADER_HPP_ */
