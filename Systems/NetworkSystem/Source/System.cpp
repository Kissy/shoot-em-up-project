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

#include <boost/lexical_cast.hpp>
#include <boost/functional/factory.hpp>
#include <boost/bind.hpp>

#include "Proto/Server/UpstreamMessage.pb.h"

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
    boost::asio::ip::tcp::resolver::iterator endpointIterator = resolver.resolve(query);
    boost::asio::async_connect(*m_pSocket, endpointIterator, 
        boost::bind(&NetworkSystem::handleConnect, this, boost::asio::placeholders::error));
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void NetworkSystem::handleConnect(const boost::system::error_code& error) {
    if (error) {
        // Handle
        return;
    }

    boost::asio::async_read(*m_pSocket, boost::asio::buffer(m_messageHeader), boost::asio::transfer_exactly(m_lenghtFieldSize), 
        boost::bind(&NetworkSystem::handleReadHeader, this, _1));
}

/**
 * @inheritDoc
 */
void NetworkSystem::handleReadHeader(const boost::system::error_code& error) {
    int nextMessageLength = (m_messageHeader[0] << 24) + (m_messageHeader[1] << 16) + (m_messageHeader[2] << 8) + m_messageHeader[3];
    boost::asio::async_read(*m_pSocket, m_messageBody, boost::asio::transfer_exactly(nextMessageLength), 
        boost::bind(&NetworkSystem::handleReadBody, this, _1));
}

/**
 * @inheritDoc
 */
void NetworkSystem::handleReadBody(const boost::system::error_code& error) {
    std::istream response_stream(&m_messageBody);
    UpstreamMessageProto upstreamMessageProto;
    upstreamMessageProto.ParseFromIstream(&response_stream);
    Log::LogNetwork("Received");
}

