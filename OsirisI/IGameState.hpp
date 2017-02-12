#ifndef IGAMESTATE_HPP_
#define IGAMESTATE_HPP_

#include "HashedString.hpp"
#include "OsirisIExternStructs.hpp"

namespace OsirisI {
	namespace Interfaces {
		class IGameState {
			private:
				OsirisI::Utilities::HashedString name;

			public:
				virtual ~IGameState() {};
				virtual ReturnState Initialize() = 0;
				virtual ReturnState Update(float time) = 0;
				virtual ReturnState Render() = 0;
				virtual ReturnState Release() = 0;

				virtual ReturnState PrepareForActivation() = 0;
				virtual ReturnState PrepareForDeactivation() = 0;

				OsirisI::Utilities::HashedString GetName() {
					return this->name;
				}
				void SetName(OsirisI::Utilities::HashedString name) {
					this->name = name;
				}
		};
	}
}

#endif /* IGAMESTATE_HPP_ */
