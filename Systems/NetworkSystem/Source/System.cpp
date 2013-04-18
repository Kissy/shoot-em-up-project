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

#include "boost/functional/factory.hpp"
#include "boost/bind.hpp"

#include "BaseTypes.h"
#include "Interface.h"

#include "System.h"
#include "Scene.h"
#include "Object/Object.h"


/**
 * @inheritDoc
 */
NetworkSystem::NetworkSystem(void) : ISystem() {
    m_SceneFactory = boost::factory<NetworkScene*>();
    m_pSocket = new boost::asio::ip::tcp::socket(m_ioService);
}

/**
 * @inheritDoc
 */
NetworkSystem::~NetworkSystem(void) {

}

/**
 * @inheritDoc
 */
Error NetworkSystem::initialize(void) {
    ASSERT(!m_bInitialized);
    
    boost::asio::ip::tcp::resolver resolver(m_ioService);
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), "localhost", "9999");
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
    //boost::asio::connect(*m_pSocket, iterator);
    
    m_bInitialized = true;
    return Errors::Success;
}
