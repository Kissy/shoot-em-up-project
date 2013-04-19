
#include "Proto/Debug/Debug.pb.h"
#include "Proto/Debug/DebugEntity.pb.h"

#include "ObjectChangesDebugger.h"
#include "Debugger.h"
#include "BaseTypes.h"
#include "Interface.h"
#include "Universal.h"

ObjectChangesDebugger::ObjectChangesDebugger(Debugger* debugger) :
    m_pDebugger(debugger) {
}


ObjectChangesDebugger::~ObjectChangesDebugger(void) {
}

Error ObjectChangesDebugger::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ISystemObject* systemObject = dynamic_cast<ISystemObject*>(pSubject);

    DebugProto debugProto;
    DebugEntityProto* debugEntityProto = debugProto.add_entities();
    debugEntityProto->set_id(systemObject->GetName());
    debugEntityProto->set_name(systemObject->GetName());
    debugEntityProto->set_category(System::getComponentName(System::Components::Object));

    std::string category = System::Types::getName(systemObject->GetSystemType());
    DebugPropertyProto* debugPropertyProto = debugEntityProto->add_properties();
    debugPropertyProto->set_category(category);

    /*IProperty::PropertiesValues properties = systemObject->getProperties();
    for (auto iter = properties.begin(); iter != properties.end(); iter++) {
        debugPropertyProto->set_key(iter->first);
        debugPropertyProto->set_value(iter->second);
    }*/

    m_pDebugger->send(&debugProto);
    return Errors::Success;
}