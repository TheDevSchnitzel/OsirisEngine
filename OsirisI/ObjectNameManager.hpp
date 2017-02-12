#ifndef OBJECTNAMEMANAGER_HPP_
#define OBJECTNAMEMANAGER_HPP_

#include "HashedString.hpp"

namespace OsirisI {
	namespace Manager {
		class ObjectNameManager {
			private:
                static std::mutex lock;
				static std::map<long, OsirisI::Utilities::HashedString> givenNames;
                static long objIDCounter;

			public:	
				static OsirisI::Utilities::HashedString GetObjectName(long nameID);
				static OsirisI::Utilities::HashedString CreateObjectName();
				static ReturnState RemoveObjectName(long nameID);
                static ReturnState RemoveObjectName(OsirisI::Utilities::HashedString name);

				static ReturnState Release();
		};
	}
}
#endif /* OBJECTNAMEMANAGER_HPP_ */
