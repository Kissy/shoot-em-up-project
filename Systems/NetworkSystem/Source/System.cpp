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

class MatchVarintPrefix {
public:
    explicit MatchVarintPrefix() : messageSize(0), bitsRead(0), decodedVarint(0) {}

    template <typename Iterator>
    std::pair<Iterator, bool> operator()(Iterator begin, Iterator end) {
        Iterator i = begin;
        while (i != end) {
            // header
            if (messageSize == 0) {
                unsigned char c = *i;
                decodedVarint = ((c & 127) << bitsRead) + decodedVarint;
                if ((c & (1<<7)) == 0)  {
                    messageSize = decodedVarint + bitsRead;
                    bitsRead = 0;
                    decodedVarint = 0;
                } else {
                    bitsRead += 7;
                }
            } else if (end - begin >= messageSize) {
                return std::make_pair(i, true);
            }
            i++;
        }
        return std::make_pair(i, false);
    }

private:
    int messageSize;
    int bitsRead;
    google::protobuf::uint32 decodedVarint;

};

namespace boost {
    namespace asio {
        template <> struct is_match_condition<MatchVarintPrefix> : public boost::true_type {};
    }
}

/**
 * @inheritDoc
 */
void NetworkSystem::handleConnect(const boost::system::error_code& error) {
    if (error) {
        // Handle
        return;
    }

    /*boost::asio::async_read(*m_pSocket, boost::asio::buffer(m_messageHeader), boost::asio::transfer_exactly(m_lenghtFieldSize), 
        boost::bind(&NetworkSystem::handleReadHeader, this, _1));*/
    boost::asio::async_read_until(*m_pSocket, m_messageBody, MatchVarintPrefix(), boost::bind(&NetworkSystem::handleReadBody, this, _1)); 
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
    const char* data = boost::asio::buffer_cast<const char*>(m_messageBody.data());
    int startIndex = 0;
    while (data[startIndex] & 0x80) {
        startIndex++;
    }
    startIndex++;
    
    UpstreamMessageProto upstreamMessageProto;
    upstreamMessageProto.ParseFromArray(data + startIndex, m_messageBody.size() - startIndex);

    Log::LogNetwork("Received");
}

