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

#include "Proto/Debug/Debug.pb.h"

#include "Defines.h"
#include "Singleton.h"
#include "SceneChangesDebugger.h"
#include "ObjectChangesDebugger.h"

#if defined DEBUG_BUILD

#include <zmq.hpp>

class IChangeManager;
class UScene;

/**
 * Debugger.
 *
 * @sa  Singleton
 */
class Debugger : public Singleton {
public:

    /**
     * Default constructor.
     */
    Debugger(void);

    /**
     * Destructor.
     */
    ~Debugger(void);

    /**
     * Initialises this object.
     *
     * @param   debuggerActive  true to debugger active.
     */
    void init(bool debuggerActive);

    /**
     * Sets a ccm.
     *
     * @param [in,out]  pSceneCCM   If non-null, the scene ccm.
     * @param [in,out]  pObjectCCM  If non-null, the object ccm.
     */
    void setCCM(IChangeManager* pSceneCCM, IChangeManager* pObjectCCM);

    /**
     * Sets u scene.
     *
     * @param [in,out]  pUScene If non-null, the u scene.
     */
    void setUScene(UScene* pUScene);

    /**
     * Cleans this object.
     */
    void clean(void);

    /**
     * Send this message.
     *
     * @param [in,out]  debugHolderProto    If non-null, the debug holder prototype.
     */
    void send(DebugProto* debugHolderProto);

private:

    IChangeManager*         m_pSceneCCM;
    IChangeManager*         m_pObjectCCM;
    
    SceneChangesDebugger*   m_pSceneChangesDebugger;
    ObjectChangesDebugger*  m_pObjectChangesDebugger;

    UScene*                 m_pUScene;
    
    zmq::context_t*         m_pContext;
    zmq::socket_t*          m_pSocket;
    
    bool                    m_bDebuggerActive;

};

DeclareSingleton(Debugger);

#endif
