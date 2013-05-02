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

#include "Debugger.h"

#ifdef DEBUG_BUILD

#include "Proto/Debug/DebugEntity.pb.h"

#include "BaseTypes.h"
#include "Interface.h"
#include "Universal.h"

void protobuf_uint8_free(void* data, void* hint) {
    delete[] data;
}

Debugger::Debugger(void) 
    : m_bDebuggerActive(false) {    
}

Debugger::~Debugger(void) {
    if (!m_bDebuggerActive) {
        return;
    }

    SAFE_DELETE(m_pSocket);
    SAFE_DELETE(m_pObjectChangesDebugger);
}

void Debugger::init(bool debuggerActive) {
    if (!debuggerActive) {
        return;
    }

    m_bDebuggerActive = debuggerActive;
    m_pObjectChangesDebugger = new ObjectChangesDebugger(this);    

    m_pContext = new zmq::context_t(1);
    m_pSocket = new zmq::socket_t(*m_pContext, ZMQ_PUSH);
    m_pSocket->connect("tcp://kissy.synology.me:26901");
}

void Debugger::setCCM(IChangeManager* pSceneCCM, IChangeManager* pObjectCCM) {
    m_pSceneCCM = pSceneCCM;
    m_pObjectCCM = pObjectCCM;
}

void Debugger::setUScene(UScene* pUScene) {
    m_pUScene = pUScene;

    if (!m_bDebuggerActive) {
        return;
    }

    DebugProto debugProto;
    UScene::SystemScenes Scenes = m_pUScene->GetSystemScenes();
    for (UScene::SystemScenesConstIt it = Scenes.begin(); it != Scenes.end(); it++) {
        ISystemScene* pScene = it->second;
        DebugEntityProto* debugEntityProto = debugProto.add_entities();
        debugEntityProto->set_id(System::Types::getName(pScene->GetSystemType()));
        debugEntityProto->set_name(System::Types::getName(pScene->GetSystemType()));
        debugEntityProto->set_category(System::getComponentName(System::Components::Scene));
        //m_pSceneCCM->Register(pScene, System::Changes::All, this);
    }

    UScene::Objects Objects = m_pUScene->GetObjects();
    for (UScene::ObjectsConstIt it = Objects.begin(); it != Objects.end(); it++) {
        UObject* pUObject = *it;
        UObject::SystemObjects SystemObjects = pUObject->GetExtensions();
        
        DebugEntityProto* debugEntityProto = debugProto.add_entities();
        debugEntityProto->set_id(pUObject->GetName());
        debugEntityProto->set_name(pUObject->GetName());
        debugEntityProto->set_category(System::getComponentName(System::Components::Object));
        for (UObject::SystemObjectsConstIt it = SystemObjects.begin(); it != SystemObjects.end(); it++) {
            ISystemObject* pObject = it->second;
            DebugPropertyProto* debugPropertyProto = debugEntityProto->add_properties();
            debugPropertyProto->set_category(System::Types::getName(pObject->GetSystemType()));
            m_pObjectCCM->Register(pObject, System::Changes::All, m_pObjectChangesDebugger);
        }
    }

    send(&debugProto);
}


void Debugger::clean(void) {
    if (!m_bDebuggerActive) {
        return;
    }

    UScene::SystemScenes Scenes = m_pUScene->GetSystemScenes();
    for (UScene::SystemScenesConstIt it = Scenes.begin(); it != Scenes.end(); it++) {
        ISystemScene* pScene = it->second;
        //m_pSceneCCM->Unregister(pSystemScene, this);

    }

    UScene::Objects Objects = m_pUScene->GetObjects();
    for (UScene::ObjectsConstIt it = Objects.begin(); it != Objects.end(); it++) {
        UObject* pUObject = *it;
        UObject::SystemObjects SystemObjects = pUObject->GetExtensions();
        for (UObject::SystemObjectsConstIt it = SystemObjects.begin(); it != SystemObjects.end(); it++) {
            ISystemObject* pObject = it->second;
            //m_pObjectCCM->Unregister(pSystemObject, this);
        }
    }
}

void Debugger::send(DebugProto* debugProto) {
    int size = debugProto->ByteSize(); 
    google::protobuf::uint8* buffer = new google::protobuf::uint8[size];
    debugProto->SerializeWithCachedSizesToArray(buffer);
    zmq::message_t* message = new zmq::message_t(buffer, size, protobuf_uint8_free);
    m_pSocket->send(*message);
}


#endif