#include "ObjectChangesDebugger.h"
#include "Proto/Debug/DebugHolderDTO.pb.h"
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

    DebugHolderProto debugHolderProto;
    DebugObjectProto* debugObjectProto = debugHolderProto.add_objects();
    debugObjectProto->set_id(systemObject->GetName());
    debugObjectProto->set_name(systemObject->GetName());
    debugObjectProto->set_category(System::getComponentName(System::Components::Object));

    std::string category = System::Types::getName(systemObject->GetSystemType());
    IProperty::PropertiesValues properties = systemObject->getProperties();
    for (auto iter = properties.begin(); iter != properties.end(); iter++) {
        DebugPropertyProto* debugPropertyProto = debugObjectProto->add_properties();
        debugPropertyProto->set_category(category);
        debugPropertyProto->set_key(iter->first);
        debugPropertyProto->set_value(iter->second);
    }

    m_pDebugger->send(&debugHolderProto);
    return Errors::Success;
}