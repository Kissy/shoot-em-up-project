#pragma once

#include "Observer/IObserver.h"

class Debugger;

class ObjectChangesDebugger : public IObserver {
public:
    ObjectChangesDebugger(Debugger* debugger);
    ~ObjectChangesDebugger(void);

    Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

private:
    Debugger*       m_pDebugger;

};

