#ifndef ILOADER_HPP_
#define ILOADER_HPP_

namespace OsirisI {
	namespace Resources {
		class Resource;
		namespace Loader {
			class ILoader {
				public:		
					virtual ~ILoader() {}
				
                    virtual Resource* Load(OsirisI::Utilities::HashedString path, OsirisI::Utilities::HashedString resourceName) = 0;
                    virtual Resource* Load(char* data, long size, OsirisI::Utilities::HashedString resourceName) = 0;
					virtual ReturnState Release() = 0;		
			};
		}
	}
}

#endif /* ILOADER_HPP_ */
