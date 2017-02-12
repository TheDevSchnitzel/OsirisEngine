#include "stdafx.h"

using namespace OsirisI::Utilities;

namespace OsirisI {
    IOsirisObject::~IOsirisObject()
    {
        LOG_DEBUG_SMALL("Object destructed at " << std::hex << this);
    }
}
