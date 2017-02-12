#include "stdafx.h"

using namespace OsirisI;
using namespace OsirisI::Manager;
using namespace OsirisI::Utilities;

std::mutex ObjectNameManager::lock;
std::map<long, OsirisI::Utilities::HashedString> ObjectNameManager::givenNames;
long ObjectNameManager::objIDCounter = 0;

OsirisI::Utilities::HashedString ObjectNameManager::GetObjectName(long nameID) {
    {
        lock.lock();
        std::lock_guard<std::mutex> lockG(lock, std::adopt_lock);
        if (givenNames.find(nameID) == givenNames.end()) return "";
        return givenNames[nameID];
    }
}

OsirisI::Utilities::HashedString ObjectNameManager::CreateObjectName() {
    HashedString str;
    {
        lock.lock();
        std::lock_guard<std::mutex> lockG(lock, std::adopt_lock);
        do {
            str = HashedString("OsirisI_OBJECT_" + objIDCounter);
            objIDCounter++;
        } while (givenNames.find(str.GetHash()) != givenNames.end());

        givenNames.insert(std::pair<long, HashedString>(str.GetHash(), str));
    }

    return str;
}

ReturnState ObjectNameManager::RemoveObjectName(long nameID) {
    {
        lock.lock();
        std::lock_guard<std::mutex> lockG(lock, std::adopt_lock);
        if (givenNames.find(nameID) == givenNames.end()) return OS_OBJECT_NOT_FOUND;

        givenNames.erase(nameID);
    }

    return OS_OK;
}

ReturnState ObjectNameManager::RemoveObjectName(OsirisI::Utilities::HashedString name) {
    return RemoveObjectName(name.GetHash());
}

ReturnState ObjectNameManager::Release() {
    {
        lock.lock();
        std::lock_guard<std::mutex> lockG(lock, std::adopt_lock);
        givenNames.clear();
    }

    return OS_OK;
}
