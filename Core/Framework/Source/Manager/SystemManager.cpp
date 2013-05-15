// Copyright � 2008-2009 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.

#include "Interface.h"

#include "Universal/UScene.h"
#include "SystemManager.h"


SystemManager::SystemManager(void) {
}


SystemManager::~SystemManager(void) {
}


Error SystemManager::Add(ISystem* pSystem) {
    Proto::SystemType SystemType = pSystem->GetSystemType();
    ASSERT(m_Systems.find(SystemType) == m_Systems.end());
    m_Systems[ SystemType ] = pSystem;
    return Errors::Success;
}


Error SystemManager::Remove(const Proto::SystemType SystemType) {
    Error   Err = Errors::Success;
    UScene::SystemsIt it = m_Systems.find(SystemType);

    if (it != m_Systems.end()) {
        m_Systems.erase(it);
    }

    return Err;
}

ISystem* SystemManager::Get(const Proto::SystemType SystemType) {
    ISystem* pSystem = NULL;
    UScene::SystemsIt it = m_Systems.find(SystemType);

    if (it != m_Systems.end()) {
        pSystem = (*it).second;
    }

    return pSystem;
}

ISystem* SystemManager::GetFirst(void) {
    ISystem* pSystem = NULL;
    m_SystemIt = m_Systems.begin();

    if (m_SystemIt != m_Systems.end()) {
        pSystem = (*m_SystemIt).second;
    }

    return pSystem;
}


ISystem* SystemManager::GetNext(void) {
    ISystem* pSystem = NULL;

    if (++m_SystemIt != m_Systems.end()) {
        pSystem = (*m_SystemIt).second;
    }

    return pSystem;
}
