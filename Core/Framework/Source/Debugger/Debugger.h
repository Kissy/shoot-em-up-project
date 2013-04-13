// Copyright © 2008-2009 Intel Corporation
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

#pragma once

#include "Defines.h"
#include "Singleton.h"
#include "ObjectChangesDebugger.h"
#include "Proto/Debug/DebugHolderDTO.pb.h"

#if defined DEBUG_BUILD

#include <zmq.hpp>

class IChangeManager;
class UScene;

class Debugger : public Singleton {
public:

    Debugger(void);
    ~Debugger(void);

    void init(bool debuggerActive);
    void setCCM(IChangeManager* pSceneCCM, IChangeManager* pObjectCCM);
    void setUScene(UScene* pUScene);
    void clean(void);
    void send(DebugHolderProto* debugHolderProto);

private:

    IChangeManager*         m_pSceneCCM;
    IChangeManager*         m_pObjectCCM;
    
    ObjectChangesDebugger*  m_pObjectChangesDebugger;

    UScene*                 m_pUScene;
    
    zmq::context_t*         m_pContext;
    zmq::socket_t*          m_pSocket;
    
    bool                    m_bDebuggerActive;

};

DeclareSingleton(Debugger);

#endif
