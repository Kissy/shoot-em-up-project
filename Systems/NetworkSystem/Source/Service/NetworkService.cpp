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

#include <boost/bind.hpp>
#include <google/protobuf/io/coded_stream.h>

#include "NetworkService.h"

#include "MatchingVarintPrefix.h"

/**
 * @inheritDoc
 */
NetworkService::NetworkService(void) {
    m_pSocket = new boost::asio::ip::tcp::socket(m_ioService);
}

/**
 * @inheritDoc
 */
NetworkService::~NetworkService(void) {
    SAFE_DELETE(m_pSocket);
}

void NetworkService::connect(std::string host, std::string port) {
    boost::asio::ip::tcp::resolver resolver(m_ioService);
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), host, port);
    boost::asio::async_connect(*m_pSocket, resolver.resolve(query), 
        boost::bind(&NetworkService::onConnected, this, boost::asio::placeholders::error));
}

/**
 * @inheritDoc
 */
void NetworkService::send(const DownstreamMessageProto downstreamMessageProto) {
    int serializedSize = downstreamMessageProto.ByteSize();
    size_t totalOutputSize = google::protobuf::io::CodedOutputStream::VarintSize32(serializedSize) + serializedSize;

    if (m_writeBuffer.size() < totalOutputSize) {
        m_writeBuffer.resize(totalOutputSize);
    }

    google::protobuf::uint8* offset = (google::protobuf::uint8*) &m_writeBuffer[0];
    offset = google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(serializedSize, offset);
    downstreamMessageProto.SerializeToArray(offset, serializedSize);
    boost::asio::write(*m_pSocket, boost::asio::buffer(m_writeBuffer, totalOutputSize));
}

/**
 * @inheritDoc
 */
void NetworkService::receive(void) {
    m_ioService.poll();
}

/**
 * @inheritDoc
 */
void NetworkService::onConnected(const boost::system::error_code& error) {
    if (error) {
        // Handle
        return;
    }

    boost::asio::async_read_until(*m_pSocket, m_readBuffer, MatchingVarintPrefix(), boost::bind(&NetworkService::onRead, this, _1)); 
}

/**
 * @inheritDoc
 */
void NetworkService::onRead(const boost::system::error_code& error) {
    if (error) {
        // Handle
        return;
    }

    const char* data = boost::asio::buffer_cast<const char*>(m_readBuffer.data());
    int startIndex = 0;
    while (data[startIndex] & 0x80) {
        startIndex++;
    }
    startIndex++;
    
    UpstreamMessageProto upstreamMessageProto;
    upstreamMessageProto.ParseFromArray(data + startIndex, m_readBuffer.size() - startIndex);
    
    // Get next message
    m_readBuffer.consume(m_readBuffer.size());
    onConnected(error);
}

