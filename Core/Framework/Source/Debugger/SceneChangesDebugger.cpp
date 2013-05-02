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

#include "Proto/Debug/Debug.pb.h"
#include "Proto/Debug/DebugEntity.pb.h"

#include "SceneChangesDebugger.h"
#include "Debugger.h"
#include "BaseTypes.h"
#include "Interface.h"
#include "Universal.h"

SceneChangesDebugger::SceneChangesDebugger(Debugger* debugger) :
    m_pDebugger(debugger) {
}


SceneChangesDebugger::~SceneChangesDebugger(void) {
}

Error SceneChangesDebugger::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    if (ChangeType & System::Changes::Generic::CreateObject) {
        ISceneObject* pScene = dynamic_cast<ISceneObject*>(pSubject);
        const ISceneObject::ObjectProtoQueue objectsToCreate = *pScene->getCreateObjects();
        for (auto objectProto : objectsToCreate) {
            DebugProto debugProto;
            DebugEntityProto* debugEntityProto = debugProto.add_entities();
            debugEntityProto->set_id(objectProto.name());
            debugEntityProto->set_name(objectProto.name());
            debugEntityProto->set_category(System::getComponentName(System::Components::Object));

            for (auto systemObject : objectProto.systemobjects()) {
                DebugPropertyProto* debugPropertyProto = debugEntityProto->add_properties();
                debugPropertyProto->set_category(systemObject.type());
                debugPropertyProto->mutable_properties()->CopyFrom(systemObject.properties());
            }

            m_pDebugger->send(&debugProto);
        }
        
        ISystemScene* systemScene = reinterpret_cast<ISystemScene*>(pSubject);
    }
    return Errors::Success;
}