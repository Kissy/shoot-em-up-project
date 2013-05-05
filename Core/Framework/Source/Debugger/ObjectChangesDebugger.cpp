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

#include "ObjectChangesDebugger.h"
#include "Debugger.h"
#include "BaseTypes.h"
#include "Interface.h"
#include "Universal/UScene.h"

ObjectChangesDebugger::ObjectChangesDebugger(Debugger* debugger)
    : m_pDebugger(debugger)
    , m_updateTimerDelay(100000000LL) /* 100ms */ {
}


ObjectChangesDebugger::~ObjectChangesDebugger(void) {
}

Error ObjectChangesDebugger::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    if (m_updateTimer.elapsed().wall >= m_updateTimerDelay) {

    }
    
    ISystemObject* systemObject = dynamic_cast<ISystemObject*>(pSubject);

    DebugProto debugProto;
    DebugEntityProto* debugEntityProto = debugProto.add_entities();
    debugEntityProto->set_id(systemObject->GetName());
    debugEntityProto->set_name(systemObject->GetName());
    debugEntityProto->set_category(System::getComponentName(System::Components::Object));

    DebugPropertyProto* debugPropertyProto = debugEntityProto->add_properties();
    debugPropertyProto->set_category(System::Types::getName(systemObject->GetSystemType()));
    
    const ProtoPropertyList properties = systemObject->getProperties();
    debugPropertyProto->mutable_properties()->CopyFrom(properties);

    m_pDebugger->send(&debugProto);
    return Errors::Success;
}