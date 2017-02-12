#include "stdafx.h"

namespace OsirisI {
	namespace Manager {
		OsirisI::Interfaces::IGameState* GameStateManager::activeState;
		std::map<long,OsirisI::Interfaces::IGameState*> GameStateManager::gameStates;
		std::mutex GameStateManager::activeStateMutex;

		OsirisI::Interfaces::IGameState* GameStateManager::GetActiveGameState() {
			activeStateMutex.lock();
			OsirisI::Interfaces::IGameState* tmp = activeState;
			activeStateMutex.unlock();
			return tmp;
		}

		void GameStateManager::SetActiveGameState(OsirisI::Interfaces::IGameState* gameState) {
			activeStateMutex.lock();
			gameState->PrepareForActivation();
			if(activeState != NULL) activeState->PrepareForDeactivation();
			if(gameStates.find(gameState->GetName().GetHash()) == gameStates.end()) AddGameState(gameState);
			activeState = gameState;
			activeStateMutex.unlock();
		}

		void GameStateManager::AddGameState(OsirisI::Interfaces::IGameState* gameState) {
			activeStateMutex.lock();
			gameStates.insert(std::pair<long,OsirisI::Interfaces::IGameState*>(gameState->GetName().GetHash(), gameState));
			activeStateMutex.unlock();
		}

		ReturnState GameStateManager::RemoveGameState(long nameHash) {
			if(activeState->GetName().GetHash() == nameHash) return 0x1B;
			if(gameStates.find(nameHash) == gameStates.end()) return 0x1A;
			gameStates.erase(nameHash);
			return OS_OK;
		}

		ReturnState GameStateManager::Update(float delta) {
			return activeState->Update(delta);
		}

		ReturnState GameStateManager::Render() {
			return activeState->Render();
		}

		ReturnState GameStateManager::Release() {
			std::map<long,OsirisI::Interfaces::IGameState*>::iterator it;
			activeState->PrepareForDeactivation();
			for(it = gameStates.begin(); it != gameStates.end(); ++it) {
				it->second->Release();
			}
			return OS_OK;
		}
	}
}
