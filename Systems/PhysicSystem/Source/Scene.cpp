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

#include <boost/functional/factory.hpp>

#include "System.h"
#include "Scene.h"
#include "Task.h"
#include "Object/PhysicObject.h"
#include "Object/MovablePhysicObject.h"

///////////////////////////////////////////////////////////////////////////////
// HavokPhysicsScene - Default constructor
PhysicScene::PhysicScene(ISystem* pSystem): ISystemScene(pSystem) {
    m_TaskFactory = boost::factory<PhysicTask*>();

    m_ObjectFactories["Movable"] = boost::factory<MovablePhysicObject*>();
}

///////////////////////////////////////////////////////////////////////////////
// ~HavokPhysicsScene - Default destructor
PhysicScene::~PhysicScene(void) {
    
}

///////////////////////////////////////////////////////////////////////////////
// Initialize - Initializes this Scene with the given properties
Error PhysicScene::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void PhysicScene::Update(f32 DeltaTime) {
    for (auto object : m_pObjects) {
        // TODO handle the modified / postChanges differently
        PhysicObject* pObject = static_cast<PhysicObject*>(object.second);
        pObject->Update(DeltaTime);

        if (pObject->GetPosition()->x > 40 || pObject->GetPosition()->x < -40 || pObject->GetPosition()->y > 30 || pObject->GetPosition()->y < -30) {
            Proto::Object object;
            object.set_id(pObject->getEntity()->getId());
            object.set_name(pObject->getEntity()->getName());
            m_deleteObjectQueue->push_back(object);
            PostChanges(System::Changes::Generic::DeleteObject);
        }
    }
}
