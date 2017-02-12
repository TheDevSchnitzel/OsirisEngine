#ifndef GAMESTATEMANAGER_HPP_
#define GAMESTATEMANAGER_HPP_

////////////////NEEDED FOR USER, OTHERWISE THESE HEADERS ARE MISSING IN NORMAL BUILD////////////////
#include <map>
#include <mutex>
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "IGameState.hpp"

namespace OsirisI {
	namespace Manager {
		class GameStateManager {
			protected:
				static OsirisI::Interfaces::IGameState* activeState;
				static std::map<long,OsirisI::Interfaces::IGameState*> gameStates;
				static std::mutex activeStateMutex;

			public:
				static OsirisI::Interfaces::IGameState* GetActiveGameState();
				static void SetActiveGameState(OsirisI::Interfaces::IGameState* gameState);
				static void AddGameState(OsirisI::Interfaces::IGameState* gameState);
				static ReturnState RemoveGameState(long nameHash);

				static ReturnState Update(float delta);
				static ReturnState Render();
				static ReturnState Release();
		};
	}
}

#endif /* GAMESTATEMANAGER_HPP_ */