#include "stdafx.h"

using namespace OsirisI::Resources;
using namespace OsirisI::Resources::Loader;

namespace OsirisI {
	namespace Manager {
		std::map<size_t, ILoader*> LoaderManager::loaderMapping;
	}
}
